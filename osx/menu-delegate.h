/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#import <Cocoa/Cocoa.h>
#import "preference-delegate.h"

@interface MenuDelegate : NSObject <NSApplicationDelegate>
{
    NSStatusItem *statusItem;
    NSImage *menuIcon;

    IBOutlet NSPopover *statusPopover;

    IBOutlet NSMenu *statusMenu;
    IBOutlet NSMenuItem *connectionStatus;
    IBOutlet NSMenuItem *startstopButton;
    IBOutlet NSMenuItem *daemonStatus;
    IBOutlet NSMenuItem *routingStatus;
    IBOutlet NSMenuItem *trafficMap;
    IBOutlet NSMenuItem *ndnPreferences;
  
    BOOL daemonStarted;
    BOOL allowSoftwareUpdates;
    BOOL enableHubDiscovery;
  
    IBOutlet NSView *daemonStatusView;
    IBOutlet NSTextField *daemonStatusText;
  
    IBOutlet NSView *connectionStatusView;
    IBOutlet NSTextField *connectionStatusText;
  
    IBOutlet PreferenceDelegate *preferencesDelegate;
}

-(void)menu:(NSMenu *)menu willHighlightItem:(NSMenuItem *)item;
-(IBAction)switchDaemon:(id)sender;
-(IBAction)openDaemonStatus:(id)sender;
-(IBAction)openRoutingStatusPage:(id)sender;
-(IBAction)openTrafficMapPage:(id)sender;

@end
