/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "config.h"
#import "preference-delegate.h"

@implementation PreferenceDelegate

-(IBAction)showPreferencesPanel:(id)sender
{
  [preferencesPanel setContentView:generalSettingsView];
  [preferencesPanel makeKeyAndOrderFront:sender];
  [preferencesPanel setLevel: NSStatusWindowLevel];
  
  if(m_operationQueue == nil)
    m_operationQueue = [[NSOperationQueue alloc] init];
  
  tableController.m_tableView = fibTableView;
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

-(void)updateFibStatus:(NSXMLDocument*)status;
{
  [tableController loadStatus:status];
  [fibTableView reloadData];
}

-(IBAction)addFibEntry:(id)sender
{
  [NSApp endSheet:prefixRegistrationSheet];
  [prefixRegistrationSheet orderOut:sender];
  
  NSString *prefixName = [namePrefixText stringValue];
  NSString *tunnelType = [tunnelCombobox itemObjectValueAtIndex:[tunnelCombobox indexOfSelectedItem]];
  NSString *endpoint = [endpointText stringValue];
  
  [m_operationQueue addOperationWithBlock:^{
      NSTask *task = [[NSTask alloc] init];
      [task setLaunchPath: @NDND_FIB_COMMAND];
      [task setArguments: [NSArray arrayWithObjects: @"add", prefixName, tunnelType, endpoint, nil]];
      [task launch];
      [task waitUntilExit];
    }];

}

-(IBAction)removeFibEntry:(id)sender
{
  NSInteger selectedRow = [fibTableView selectedRow];
  
  if(selectedRow == -1)
    return;
  
  NSString *faceID = [tableController getFaceByRowIndex:selectedRow];
  if (faceID == nil)
    return;

  NSString *prefix = [tableController getPrefixByRowIndex:selectedRow];
  if (prefix == nil)
    return;

  [m_operationQueue addOperationWithBlock:^{
      NSTask *task = [[NSTask alloc] init];
      [task setLaunchPath: @NDND_FIB_COMMAND];
      [task setArguments: [NSArray arrayWithObjects: @"del", prefix, @"face", faceID, nil]];
      [task launch];
      [task waitUntilExit];
    }];
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


-(IBAction)openRoutingStatusPage:(id)sender
{
  NSURL *pageURL = [NSURL URLWithString:@"http://netlab.cs.memphis.edu/script/htm/status.htm"];

  [[NSWorkspace sharedWorkspace] openURL: pageURL];
}

-(IBAction)openTrafficMapPage:(id)sender
{

  NSURL *pageURL = [NSURL URLWithString:@"http://ndnmap.arl.wustl.edu"];

  [[NSWorkspace sharedWorkspace] openURL: pageURL];
}


@end
