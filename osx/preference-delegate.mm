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

#include "config.hpp"
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

  // NSOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
  //     NSTask *task = [[NSTask alloc] init];
  //     [task setLaunchPath: @NDND_FIB_COMMAND];
  //     [task setArguments: [NSArray arrayWithObjects: @"add", prefixName, tunnelType, endpoint, nil]];
  //     [task launch];
  //     [task waitUntilExit];
  //   }];

  // [(MenuDelegate*)[[NSApplication sharedApplication] delegate] updateStatusWithDependency:operation];
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

  // NSOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
  //     NSTask *task = [[NSTask alloc] init];
  //     [task setLaunchPath: @NDND_FIB_COMMAND];
  //     [task setArguments: [NSArray arrayWithObjects: @"del", prefix, @"face", faceID, nil]];
  //     [task launch];
  //     [task waitUntilExit];
  //   }];

  // [(MenuDelegate*)[[NSApplication sharedApplication] delegate] updateStatusWithDependency:operation];
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

-(IBAction)openCertificationPage:(id)sender
{
  NSURL *pageURL = [NSURL URLWithString:@"http://ndncert.named-data.net"];

  [[NSWorkspace sharedWorkspace] openURL: pageURL];
}


@end
