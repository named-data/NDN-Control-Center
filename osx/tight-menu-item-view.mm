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

#import "tight-menu-item-view.h"

@implementation TightMenuItemView

-(void)setStatus:(NSString*)value
{
  status = value;
}

- (void) drawRect:(NSRect)dirtyRect
{
  [self setSubviews:[NSArray array]];

  NSRect fullBounds = [self bounds];
  fullBounds.size.height += 4;
  [[NSBezierPath bezierPathWithRect:fullBounds] setClip];


  [[NSColor colorWithCalibratedRed:(70.0f/255.0f)
                              green:(70.0f/255.0f)
                              blue:(70.0f/255.0f)
                              alpha:1.0] set];
  NSRectFill(fullBounds);

  NSTextField *connectionStatusText = [[NSTextField alloc] initWithFrame:[self bounds]];
  [connectionStatusText setTextColor:[NSColor whiteColor]];
  [connectionStatusText setStringValue:status];
  [connectionStatusText setEditable:NO];
  [connectionStatusText setBezeled:NO];
  [connectionStatusText setDrawsBackground:NO];
  [connectionStatusText setSelectable:NO];
  [connectionStatusText setAlignment:NSCenterTextAlignment];
  NSFont *theFont = [NSFont fontWithName:@"Lucida Grande" size:14];
  [connectionStatusText setFont:theFont];

  [self addSubview: connectionStatusText];
}


@end
