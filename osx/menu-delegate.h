/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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
  NSOperationQueue *m_operationQueue;
  
  NSStatusItem *statusItem;

  NSImage *m_connectedIcon;
  NSImage *m_disconnectedIcon;
  NSData  *m_statusXslt;
  NSData  *m_statusToFibXslt;

  IBOutlet NSPopover *statusPopover;

  IBOutlet NSMenu *statusMenu;
  IBOutlet NSMenuItem *connectionStatus;
  IBOutlet NSMenuItem *startstopButton;
  IBOutlet NSMenuItem *daemonStatus;
  IBOutlet NSMenuItem *routingStatus;
  IBOutlet NSMenuItem *trafficMap;
  IBOutlet NSMenuItem *ndnPreferences;
  
  BOOL m_daemonStarted;
  BOOL allowSoftwareUpdates;
  BOOL enableHubDiscovery;
  
  IBOutlet NSView *daemonStatusView;
  IBOutlet NSTextField *daemonStatusHtml;
  IBOutlet NSView *daemonStatusHtmlView;
  NSAttributedString *m_statusString;
  
  IBOutlet NSView *connectionStatusView;
  IBOutlet NSTextField *connectionStatusText;
  
  IBOutlet PreferenceDelegate *preferencesDelegate;
}

-(void)menu:(NSMenu *)menu willHighlightItem:(NSMenuItem *)item;

-(IBAction)openDaemonStatus:(id)sender;
-(IBAction)showExitConfirmationWindow:(id)sender;

-(void)updateStatus;
-(void)updateStatusWithDependency:(NSOperation*)dependency;
-(void)statusUpdated:(NSXMLDocument*)document;
-(void)statusUnavailable:(id)none;

@end
