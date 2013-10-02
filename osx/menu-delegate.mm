/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "config.h"
#import "menu-delegate.h"

#define NDND_START_COMMAND @ NDNX_ROOT "/bin/ndndstart"
#define NDND_STOP_COMMAND @ NDNX_ROOT "/bin/ndndstop"
#define NDND_STATUS_COMMAND @ NDNX_ROOT "/bin/ndndstatus"

@implementation MenuDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
  daemonStarted = false;
  allowSoftwareUpdates = true;
  enableHubDiscovery = true;
  
  NSTimer *t = [NSTimer scheduledTimerWithTimeInterval: 1.0
                      target: self
                      selector:@selector(onTick:)
                      userInfo: nil repeats:YES];
  [[NSRunLoop mainRunLoop] addTimer:t forMode:NSRunLoopCommonModes];
  
  daemonStarted = true;
  [connectionStatusText setStringValue:@"Connected"];
    
  NSTask *task = [[NSTask alloc] init];
  [task setLaunchPath: NDND_START_COMMAND];
  [task launch];
    
  NSBundle *bundle = [NSBundle bundleForClass:[self class]];
  NSString *path = [bundle pathForResource:@"FlatConnected" ofType:@"png"];
  menuIcon = [[NSImage alloc] initWithContentsOfFile:path];
  [statusItem setImage:menuIcon];
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
}

-(IBAction)openDaemonStatus:(id)sender
{
}

-(IBAction)showExitConfirmationWindow:(id)sender
{
  [exitWindow makeKeyAndOrderFront:sender];
  [exitWindow setLevel: NSStatusWindowLevel];
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

-(void)onTick:(NSTimer *)timer
{
  if (daemonStarted)
  {
    NSTask *task = [[NSTask alloc] init];
    [task setLaunchPath: NDND_STATUS_COMMAND];
    
    NSPipe * out = [NSPipe pipe];
    [task setStandardOutput:out];

    [task launch];
    [task waitUntilExit];

    NSFileHandle * read = [out fileHandleForReading];
    NSData * dataRead = [read readDataToEndOfFile];
    NSString *stringRead = [[NSString alloc] initWithData:dataRead encoding:NSUTF8StringEncoding];
  
    [daemonStatusText setStringValue:stringRead];
  }
}

-(IBAction)confirmExit:(id)sender
{
  [NSApp terminate:self];
}

-(IBAction)cancelExit:(id)sender
{
  if([exitWindow isVisible])
   [exitWindow orderOut:self];
}

@end
