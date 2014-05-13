/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (c) 2013-2014, Regents of the University of California,
 *
 * This file is part of NFD Control Center.  See AUTHORS.md for complete list of NFD
 * authors and contributors.
 *
 * NFD Control Center is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD Control Center is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with NFD
 * Control Center, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 *
 * \author Alexander Afanasyev <http://lasr.cs.ucla.edu/afanasyev/index.html>
 * \author Ilya Moiseenko <http://ilyamoiseenko.com/>
 */

#include "config.hpp"
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
  // NSTask *task = [[NSTask alloc] init];
  // [task setLaunchPath: @NDND_STATUS_COMMAND];
  // [task setArguments: [NSArray arrayWithObjects: @"status",@"-x",nil]];

  // NSPipe * out = [NSPipe pipe];
  // [task setStandardOutput:out];

  // [task launch];
  // [task waitUntilExit];

  // NSFileHandle * read = [out fileHandleForReading];
  // NSData * dataRead = [read readDataToEndOfFile];
  // NSString *stringRead = [[NSString alloc] initWithData:dataRead encoding:NSUTF8StringEncoding];

  // if ([stringRead isEqualToString:@""]) {
  //   [m_delegate performSelectorOnMainThread:@selector(statusUnavailable:)
  //                                withObject:nil
  //                                waitUntilDone:YES];

  // } else {
  //   NSError *error = nil;
  //   NSXMLDocument *document = [[NSXMLDocument alloc]
  //                               initWithXMLString:stringRead
  //                                         options:0
  //                                         error:&error];

  //   [m_delegate performSelectorOnMainThread:@selector(statusUpdated:)
  //                                withObject:document
  //                                waitUntilDone:YES];
  // }
}

@end
