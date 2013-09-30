/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "config.h"
#import "preference-delegate.h"

#define NDND_FIB_COMMAND @ NDNX_ROOT "/bin/ndndc"

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

-(IBAction)addFibEntry:(id)sender
{
  [NSApp endSheet:prefixRegistrationSheet];
  [prefixRegistrationSheet orderOut:sender];
  
  NSString *operationName = @"add";
  
  NSString *prefixName = [namePrefixText stringValue];
  NSString *socketType = [tunnelCombobox itemObjectValueAtIndex:[tunnelCombobox indexOfSelectedItem]];
  NSString *address = [endpointText stringValue];

  NSArray *arguments = [NSArray arrayWithObjects: operationName, prefixName, socketType, address, nil];

  NSTask *task = [[NSTask alloc] init];
  [task setLaunchPath: NDND_FIB_COMMAND];
  [task setArguments: arguments];
  [task launch];
}

-(IBAction)removeFibEntry:(id)sender
{

}

- (IBAction) showFibEntrySheet:(id)sender
{
    [NSApp beginSheet:prefixRegistrationSheet
    modalForWindow:preferencesPanel
    modalDelegate:self
    didEndSelector:nil
    contextInfo:nil];
  
    [tunnelCombobox selectItemAtIndex:0];
}

-(IBAction)hideFibEntrySheet:(id)sender
{
    [NSApp endSheet:prefixRegistrationSheet];
    [prefixRegistrationSheet orderOut:sender];
}

@end
