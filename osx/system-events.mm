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

/*
  Potentially useful System Configuration regex patterns:

  (backslash quoting below is only to protect the C comment)
  State:/Network/Interface/.*\/Link
  State:/Network/Interface/.*\/IPv4
  State:/Network/Interface/.*\/IPv6

  State:/Network/Global/DNS
  State:/Network/Global/IPv4

  Potentially useful notifications from Darwin Notify Center:

  com.apple.system.config.network_change
*/

#import "system-events.h"
#import "menu-delegate.h"

#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SystemConfiguration.h>

@implementation SystemEvents

-(void)scheduleDaemonRestart
{
  [NSTimer scheduledTimerWithTimeInterval: 3.0
                                   target: (MenuDelegate*)[[NSApplication sharedApplication] delegate]
                                   selector:@selector(restartDaemon:)
                                   userInfo: nil
                                   repeats:NO];
}

- (void)wakeUpNotification:(NSNotification*) note
{
  [self scheduleDaemonRestart];
}

static void
NotificationCenterCallback(CFNotificationCenterRef center,
                           void *observer,
                           CFStringRef name,
                           const void *object,
                           CFDictionaryRef userInfo)
{
  [(__bridge SystemEvents*)observer scheduleDaemonRestart];
}

-(id)init
{
  if (![super init]) {
    return nil;
  }

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
            selector: @selector(wakeUpNotification:)
            name: NSWorkspaceDidWakeNotification object: NULL];

  CFNotificationCenterAddObserver(CFNotificationCenterGetDarwinNotifyCenter(),
                                  (__bridge void*)self,
                                  NotificationCenterCallback,
                                  CFSTR ("com.apple.system.config.network_change"), // name of notification
                                  NULL, // object to observe
                                  NSNotificationSuspensionBehaviorDeliverImmediately);

  return self;
}

-(void)disable
{
  [[[NSWorkspace sharedWorkspace] notificationCenter] removeObserver:self];

  CFNotificationCenterRemoveEveryObserver(CFNotificationCenterGetDarwinNotifyCenter(),
                                          (__bridge void*)self);
}

@end
