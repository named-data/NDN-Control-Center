/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

@interface TightMenuItemView : NSView
{
  NSString *status;
}

-(void)setStatus:(NSString*)value;


@end
