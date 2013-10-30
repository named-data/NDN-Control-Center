/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#import <Cocoa/Cocoa.h>
#import "preference-delegate.h"
#import "system-events.h"
#import "tight-menu-item-view.h"

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
  
  IBOutlet NSTextField *interestSentRecv;
  IBOutlet NSTextField *dataSentRecv;
  
  IBOutlet NSView *daemonStatusView;
  
  TightMenuItemView *connectionStatusView;
  
  IBOutlet PreferenceDelegate *preferencesDelegate;

  SystemEvents *m_systemEvents;
  bool m_autoconfInProgress;
}

@property (nonatomic, readwrite) NSString *interestSent;
@property (nonatomic, readwrite) NSString *interestRecv;
@property (nonatomic, readwrite) NSString *dataSent;
@property (nonatomic, readwrite) NSString *dataRecv;

-(IBAction)openDaemonStatus:(id)sender;
-(IBAction)showExitConfirmationWindow:(id)sender;

-(void)updateStatus;
-(void)updateStatusWithDependency:(NSOperation*)dependency;
-(void)statusUpdated:(NSXMLDocument*)document;
-(void)statusUnavailable:(id)none;

-(void)restartDaemon:(id)none;

@end
