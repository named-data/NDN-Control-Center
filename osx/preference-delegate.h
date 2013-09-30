/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#import <Cocoa/Cocoa.h>

@interface PreferenceDelegate : NSObject
{
  IBOutlet NSWindow *preferencesPanel;
  IBOutlet NSView *generalSettingsView;
  IBOutlet NSView *forwardingSettingsView;
  IBOutlet NSView *securitySettingsView;
}

@property BOOL allowSoftwareUpdates;
@property BOOL enableHubDiscovery;

-(IBAction)showPreferencesPanel:(id)sender;
-(IBAction)openGeneralSettings:(id)sender;
-(IBAction)openForwardingSettings:(id)sender;
-(IBAction)openSecuritySettings:(id)sender;

-(IBAction)switchSoftwareUpdates:(id)sender;
-(IBAction)switchHubDiscovery:(id)sender;
@end
