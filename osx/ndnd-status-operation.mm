/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "config.h"
#import "ndnd-status-operation.h"

@implementation NdndStatusOperation

-(id)initWithDelegate:(MenuDelegate*)delegate
{
  if (![super init]) return nil;
  m_delegate = delegate;
  return self;
}

-(void)main
{
  NSTask *task = [[NSTask alloc] init];
  [task setLaunchPath: @NDND_STATUS_COMMAND];
  [task setArguments: [NSArray arrayWithObjects: @"status",@"-x",nil]];

  NSPipe * out = [NSPipe pipe];
  [task setStandardOutput:out];

  [task launch];
  [task waitUntilExit];

  NSFileHandle * read = [out fileHandleForReading];
  NSData * dataRead = [read readDataToEndOfFile];
  NSString *stringRead = [[NSString alloc] initWithData:dataRead encoding:NSUTF8StringEncoding];

  if ([stringRead isEqualToString:@""]) {
    [m_delegate performSelectorOnMainThread:@selector(statusUnavailable:)
                                 withObject:nil
                                 waitUntilDone:YES];
    
  } else {
    NSError *error = nil;
    NSXMLDocument *document = [[NSXMLDocument alloc]
                                initWithXMLString:stringRead
                                          options:0
                                          error:&error];

    [m_delegate performSelectorOnMainThread:@selector(statusUpdated:)
                                 withObject:document
                                 waitUntilDone:YES];
  }
  
  // [daemonStatusText setStringValue:stringRead];
}

@end
