/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
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
