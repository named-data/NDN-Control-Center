/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#import "menu-delegate.h"

#define NDND_START_COMMAND @"/opt/local/bin/ndndstart"
#define NDND_STOP_COMMAND @"/opt/local/bin/ndndstop"
#define NDND_STATUS_COMMAND @"/opt/local/bin/ndndstatus"

@implementation MenuDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
  // Insert code here to initialize your application
}

-(void)awakeFromNib
{
  statusItem = [[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength];
  [statusItem setMenu:statusMenu];
  [statusItem setToolTip:@"NDN Control Center"];
  [statusItem setEnabled:YES];
  [statusItem setHighlightMode:YES];
  //[statusItem setTarget:self];

  NSBundle *bundle = [NSBundle bundleForClass:[self class]];
  NSString *path = [bundle pathForResource:@"FlatDisconnected" ofType:@"png"];
  menuIcon = [[NSImage alloc] initWithContentsOfFile:path];
  [statusItem setTitle:@""];
  [statusItem setImage:menuIcon];
  
  
  [connectionStatus setView: connectionStatusView];
  [connectionStatus setTarget:self];
  [daemonStatus setView: daemonStatusView];
  [daemonStatus setTarget:self];

  daemonStarted = false;
  //[NSApp activateIgnoringOtherApps:YES];
}

-(IBAction)switchDaemon:(id)sender
{
  if (daemonStarted)
  {
    daemonStarted = false;
    [sender setTitle:@"Start"];
    [connectionStatusText setStringValue:@"Disconnected"];
    
    NSTask *task = [[NSTask alloc] init];
    [task setLaunchPath: NDND_STOP_COMMAND];
    [task launch];
    
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *path = [bundle pathForResource:@"FlatDisconnected" ofType:@"png"];
    menuIcon = [[NSImage alloc] initWithContentsOfFile:path];
    [statusItem setImage:menuIcon];
  }
  else
  {
    daemonStarted = true;
    [sender setTitle:@"Stop"];
    [connectionStatusText setStringValue:@"Connected"];
    
    NSTask *task = [[NSTask alloc] init];
    [task setLaunchPath: NDND_START_COMMAND];
    [task launch];
    
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *path = [bundle pathForResource:@"FlatConnected" ofType:@"png"];
    menuIcon = [[NSImage alloc] initWithContentsOfFile:path];
    [statusItem setImage:menuIcon];
  }
}

-(IBAction)openDaemonStatus:(id)sender
{
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

-(void)menu:(NSMenu *)menu willHighlightItem:(NSMenuItem *)item
{

  if( ([item view]!=nil) && (item == daemonStatus) )
  {
    [statusPopover showRelativeToRect:[[item view] bounds]
                 ofView:[item view]
                 preferredEdge:NSMinXEdge];
  }
  else
  {
    [statusPopover performClose:nil];
  }
}

-(IBAction)openNDNDPreferences:(id)sender
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
    allowSoftwareUpdates = true;
  }
  else
  {
    allowSoftwareUpdates = false;
  }
}

-(IBAction)switchHubDiscovery:(id)sender
{
  if ([(NSButton*)sender state] == NSOnState)
  {
    enableHubDiscovery = true;
  }
  else
  {
    enableHubDiscovery = false;
  }
}

@end
