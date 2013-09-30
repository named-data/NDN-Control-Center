/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#import "preference-delegate.h"

@implementation PreferenceDelegate

-(IBAction)showPreferencesPanel:(id)sender
{
  [preferencesPanel setContentView:generalSettingsView];
  [preferencesPanel makeKeyAndOrderFront:sender];
  [preferencesPanel setLevel: NSStatusWindowLevel];
}

-(IBAction)openGeneralSettings:(id)sender
{
  [preferencesPanel setContentView:generalSettingsView];
}

-(IBAction)openForwardingSettings:(id)sender
{
  [preferencesPanel setContentView:forwardingSettingsView];
}

-(IBAction)openSecuritySettings:(id)sender
{
  [preferencesPanel setContentView:securitySettingsView];
}

-(IBAction)switchSoftwareUpdates:(id)sender
{
  if ([(NSButton*)sender state] == NSOnState)
  {
    _allowSoftwareUpdates = true;
  }
  else
  {
    _allowSoftwareUpdates = false;
  }
}

-(IBAction)switchHubDiscovery:(id)sender
{
  if ([(NSButton*)sender state] == NSOnState)
  {
    _enableHubDiscovery = true;
  }
  else
  {
    _enableHubDiscovery = false;
  }
}


@end
