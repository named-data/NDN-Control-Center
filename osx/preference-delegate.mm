/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "config.h"
#import "preference-delegate.h"
#import "menu-delegate.h"

@implementation PreferenceDelegate

-(IBAction)showPreferencesPanel:(id)sender
{
  [preferencesPanel setContentView:generalSettingsView];
  [preferencesPanel makeKeyAndOrderFront:sender];
  [preferencesPanel setLevel: NSStatusWindowLevel];
  
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

  NSOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
      NSTask *task = [[NSTask alloc] init];
      [task setLaunchPath: @NDND_FIB_COMMAND];
      [task setArguments: [NSArray arrayWithObjects: @"add", prefixName, tunnelType, endpoint, nil]];
      [task launch];
      [task waitUntilExit];
    }];

  [(MenuDelegate*)[[NSApplication sharedApplication] delegate] updateStatusWithDependency:operation];
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

  NSOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
      NSTask *task = [[NSTask alloc] init];
      [task setLaunchPath: @NDND_FIB_COMMAND];
      [task setArguments: [NSArray arrayWithObjects: @"del", prefix, @"face", faceID, nil]];
      [task launch];
      [task waitUntilExit];
    }];

  [(MenuDelegate*)[[NSApplication sharedApplication] delegate] updateStatusWithDependency:operation];
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
