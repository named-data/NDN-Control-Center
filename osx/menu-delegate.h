/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#import <Cocoa/Cocoa.h>

@interface MenuDelegate : NSObject <NSApplicationDelegate>
{
  IBOutlet NSMenu *statusMenu;
  NSStatusItem * statusItem;
}
 
@end
