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
#import "fib-table-controller.h"

@interface PreferenceDelegate : NSObject
{
  IBOutlet NSWindow *preferencesPanel;

  IBOutlet NSView *generalSettingsView;
  IBOutlet NSButton *checkAllowSoftwareUpdates;
  IBOutlet NSButton *checkEnableHubDiscovery;
  IBOutlet NSButton *checkShutdownNdndOnExit;

  IBOutlet NSView *forwardingSettingsView;
  IBOutlet NSTableView *fibTableView;

  IBOutlet NSView *securitySettingsView;
  IBOutlet NSView *testbedSettingsView;

  IBOutlet NSPanel *prefixRegistrationSheet;
  IBOutlet NSComboBox *tunnelCombobox;
  IBOutlet NSTextField *namePrefixText;
  IBOutlet NSTextField *endpointText;

  IBOutlet FibTableController *tableController;
}

-(IBAction)showPreferencesPanel:(id)sender;
-(IBAction)openGeneralSettings:(id)sender;
-(IBAction)openForwardingSettings:(id)sender;
-(IBAction)openSecuritySettings:(id)sender;

-(IBAction)addFibEntry:(id)sender;
-(IBAction)removeFibEntry:(id)sender;
-(IBAction)showFibEntrySheet:(id)sender;
-(IBAction)hideFibEntrySheet:(id)sender;

-(IBAction)openRoutingStatusPage:(id)sender;
-(IBAction)openTrafficMapPage:(id)sender;
-(IBAction)openCertificationPage:(id)sender;

-(void)updateFibStatus:(NSXMLDocument*)status;

@end
