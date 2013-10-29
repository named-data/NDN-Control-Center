//
/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
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
