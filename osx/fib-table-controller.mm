/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "config.h"
#import "fib-table-controller.h"

@implementation FibTableController

- (void)loadStatus:(NSXMLDocument *)document
{
  m_document = document;
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
  if (m_document == nil)
    {
      return 0;
    }
  else
    {
      return [[m_document rootElement] childCount];
    }
}

-(id)tableView:(NSTableView *)aTableView
  objectValueForTableColumn:(NSTableColumn *)aTableColumn 
  row:(NSInteger)rowIndex
{
  NSXMLNode *element = [[m_document rootElement] childAtIndex:rowIndex];

  if([[[aTableColumn headerCell] stringValue] isEqual:@"NDN prefix"])
  {
    return [[element childAtIndex:2] stringValue];
  }
  else if([[[aTableColumn headerCell] stringValue] isEqual:@"Face ID"])
  {
    return [[element childAtIndex:0] stringValue];
  }
  else if([[[aTableColumn headerCell] stringValue] isEqual:@"Endpoint"])
  {
    return [[element childAtIndex:1] stringValue];
  }
  
  return nil;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

- (NSString *)getFaceByRowIndex:(NSInteger)index
{
  NSXMLNode *element = [[m_document rootElement] childAtIndex:index];
  return [[element childAtIndex:0] stringValue];
}

- (NSString *)getPrefixByRowIndex:(NSInteger)index
{
  NSXMLNode *element = [[m_document rootElement] childAtIndex:index];
  return [[element childAtIndex:2] stringValue];
}

@end
