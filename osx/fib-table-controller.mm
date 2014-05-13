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
