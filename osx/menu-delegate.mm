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
  NSString *path = [bundle pathForResource:@"IconDisconnected" ofType:@"png"];
  menuIcon = [[NSImage alloc] initWithContentsOfFile:path];
  [statusItem setTitle:@""];
  [statusItem setImage:menuIcon];
}

-(IBAction)openDaemonStatus:(id)sender
{
  [statusPopover showRelativeToRect:[[daemonStatus view] bounds]
                 ofView:[daemonStatus view]
                 preferredEdge:NSMinXEdge];

  //[statusPopover showRelativeToRect:[sender bounds] ofView:sender preferredEdge:NSMinYEdge];
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
