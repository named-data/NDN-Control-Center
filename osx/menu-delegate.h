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
