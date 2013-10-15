/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
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
