/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#import "menu-delegate.h"

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
  //[statusItem setTitle:@"Status"];
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
}

-(IBAction)switchDaemon:(id)sender
{
  if (daemonStarted)
  {
    daemonStarted = false;
    [sender setTitle:@"Start"];
    //[connectionStatus setTitle:@"NDN disconnected"];
    [connectionStatusText setStringValue:@"Disconnected"];
    
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *path = [bundle pathForResource:@"FlatDisconnected" ofType:@"png"];
    menuIcon = [[NSImage alloc] initWithContentsOfFile:path];
    [statusItem setImage:menuIcon];
  }
  else
  {
    daemonStarted = true;
    [sender setTitle:@"Stop"];
    //[connectionStatus setTitle:@"NDN connected"];
    [connectionStatusText setStringValue:@"Connected"];
    
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

@end
