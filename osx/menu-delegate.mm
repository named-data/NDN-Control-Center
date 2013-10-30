/* -*- Mode: obj; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "config.h"
#import "menu-delegate.h"
#import "ndnd-status-operation.h"
#import "tight-menu-item-view.h"

@implementation MenuDelegate

@synthesize interestSent;
@synthesize interestRecv;
@synthesize dataSent;
@synthesize dataRecv;

-(id)init
{
  if (![super init]) {
    return nil;
  }

  interestSent = @"N/A";
  interestRecv = @"N/A";
  dataSent = @"N/A";
  dataRecv = @"N/A";
  
  m_autoconfInProgress = false;
  m_operationQueue = [[NSOperationQueue alloc] init];
  return self;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
  // Register the preference defaults early.
  NSDictionary *appDefaults =
    [NSDictionary dictionaryWithObjectsAndKeys:
                      [NSNumber numberWithBool:YES], @"allowSoftwareUpdates",
                      [NSNumber numberWithBool:YES], @"enableHubDiscovery",
                      [NSNumber numberWithBool:NO],  @"shutdownNdndOnExit",
                  nil
     ];
  [[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];
 
   // Other initialization...

  m_daemonStarted = false; 

  NSBundle *bundle = [NSBundle bundleForClass:[self class]];
  m_connectedIcon = [[NSImage alloc] initWithContentsOfFile:[bundle pathForResource:@"FlatConnected" ofType:@"png"]];
  m_disconnectedIcon = [[NSImage alloc] initWithContentsOfFile:[bundle pathForResource:@"FlatDisconnected" ofType:@"png"]];
  m_statusXslt = [NSData dataWithContentsOfFile:[bundle pathForResource:@"status" ofType:@"xslt"]];
  m_statusToFibXslt = [NSData dataWithContentsOfFile:[bundle pathForResource:@"status-to-fib" ofType:@"xslt"]];
  
  [NSTimer scheduledTimerWithTimeInterval: 1.0
           target: self
           selector:@selector(onTick:)
           userInfo: nil
           repeats:YES];
  [self updateStatus];

  m_systemEvents = [[SystemEvents alloc] init];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
  [m_systemEvents disable];
}

-(void)awakeFromNib
{
  statusItem = [[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength];
  [statusItem setMenu:statusMenu];
  [statusItem setToolTip:@"NDN Control Center"];
  [statusItem setEnabled:YES];
  [statusItem setHighlightMode:YES];
  //[statusItem setTarget:self];

  [statusItem setTitle:@""];
  [statusItem setImage:m_disconnectedIcon];
  
  float menuItemHeight = 20;

  NSRect viewRect = NSMakeRect(0, 0, /* width autoresizes */ 1, menuItemHeight);
  connectionStatusView = [[TightMenuItemView alloc] initWithFrame:viewRect];
  connectionStatusView.autoresizingMask = NSViewWidthSizable;

  [connectionStatus setView:connectionStatusView];
  [connectionStatus setTarget:self];
  
  [daemonStatus setView: daemonStatusView];
  [daemonStatus setTarget:self];
}

-(IBAction)openDaemonStatus:(id)sender
{
}

-(IBAction)showExitConfirmationWindow:(id)sender
{
  if ([[NSUserDefaults standardUserDefaults] boolForKey:@"shutdownNdndOnExit"]) {
    [m_operationQueue cancelAllOperations];

    [m_operationQueue addOperationWithBlock:^{
        NSTask *task = [[NSTask alloc] init];
        [task setLaunchPath: @NDND_STOP_COMMAND];
        [task launch];
        [task waitUntilExit];
      }];

    [m_operationQueue waitUntilAllOperationsAreFinished];
    [NSApp terminate:self];
  }
  else {
    NSAlert *alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:@"Yes"];
    [alert addButtonWithTitle:@"No"];
    [alert addButtonWithTitle:@"Cancel"];
    [alert setMessageText:@"Shutdown NDN daemon as well?"];
    [alert setInformativeText:@"All NDN operations will be become unavailable."];
    [alert setAlertStyle:NSCriticalAlertStyle];
    // [alert setShowsSuppressionButton: YES];

    NSInteger res = [alert runModal];
    if (res == NSAlertFirstButtonReturn) {
      // "YES" stop ndnd
      [m_operationQueue cancelAllOperations];

      [m_operationQueue addOperationWithBlock:^{
          NSTask *task = [[NSTask alloc] init];
          [task setLaunchPath: @NDND_STOP_COMMAND];
          [task launch];
          [task waitUntilExit];
        }];

      [m_operationQueue waitUntilAllOperationsAreFinished];
      [NSApp terminate:self];
    } else if (res == NSAlertSecondButtonReturn) {
      // "NO" terminate app but keep ndnd running
      [m_operationQueue cancelAllOperations];
      [NSApp terminate:self];
    }
  }
}

-(void)onTick:(NSTimer *)timer
{
  [self updateStatus];
}

-(void)updateStatus
{
  NSOperation *operation = [[NdndStatusOperation alloc] initWithDelegate:self];
  [m_operationQueue addOperation:operation];
}

-(void)updateStatusWithDependency:(NSOperation*)dependency
{
  NSOperation *operation = [[NdndStatusOperation alloc] initWithDelegate:self];
  [operation addDependency:dependency];

  [m_operationQueue addOperation:dependency];
  [m_operationQueue addOperation:operation];
}

- (void)statusUpdated:(NSXMLDocument*)document
{
  if (!m_daemonStarted) {
    m_daemonStarted = true;
    [connectionStatusView setStatus:@"Active"];
    
    [statusItem setImage:m_connectedIcon];
  }

  NSXMLDocument *statusXml = [document objectByApplyingXSLT:m_statusXslt
                              arguments:nil
                              error:nil];

  NSXMLDocument *statusFibXml = [document objectByApplyingXSLT:m_statusToFibXslt
                                 arguments:nil
                                 error:nil];
  
  NSXMLNode *element = [[statusXml rootElement] childAtIndex:0]; //data
  [self setDataRecv:[[element childAtIndex:0] stringValue]];
  [self setDataSent:[[element childAtIndex:1] stringValue]];
  
  element = [[statusXml rootElement] childAtIndex:1]; //interests
  [self setInterestRecv:[[element childAtIndex:0] stringValue]];
  [self setInterestSent:[[element childAtIndex:1] stringValue]];

  [preferencesDelegate updateFibStatus:statusFibXml];

  if ([[NSUserDefaults standardUserDefaults] boolForKey:@"enableHubDiscovery"]) { 
    NSArray *autoconf = [[statusFibXml rootElement] nodesForXPath:@"//fib/prefix[text()='ndn:/autoconf-route']" error:nil];
    if ([autoconf count] == 0)
      {
        [self restartDaemon:nil];
      }
  }
}

- (void)statusUnavailable:(id)none
{
  // try start ndnd if it is not started yet
  if (m_daemonStarted) {
    m_daemonStarted = false;
    
    [connectionStatusView setStatus:@"Starting..."];
    
    [statusItem setImage:m_disconnectedIcon];
  }

  [self setInterestSent:@"N/A"];
  [self setInterestRecv:@"N/A"];
  [self setDataSent:@"N/A"];
  [self setDataRecv:@"N/A"];

  [preferencesDelegate updateFibStatus:nil];

  m_autoconfInProgress = true;
  
  NSOperation *startOp = [NSBlockOperation blockOperationWithBlock:^{
      NSTask *task = [[NSTask alloc] init];
      [task setLaunchPath: @NDND_START_COMMAND];
      [task launch];
    }];

  if ([[NSUserDefaults standardUserDefaults] boolForKey:@"enableHubDiscovery"]) {
    NSOperation *autoconfOp = [NSBlockOperation blockOperationWithBlock:^{
        NSTask *task = [[NSTask alloc] init];
        [task setLaunchPath: @NDND_AUTOCONFIG_COMMAND];
        [task launch];
        [task waitUntilExit];

        m_autoconfInProgress = false;
      }];

    [autoconfOp addDependency:startOp];
    [m_operationQueue addOperation:autoconfOp];
  }

  [m_operationQueue addOperation:startOp];
}

-(void)restartDaemon:(id)none
{
  if (![[NSUserDefaults standardUserDefaults] boolForKey:@"enableHubDiscovery"])
    return;
    
  if (m_autoconfInProgress)
    return;

  NSLog (@"No automatically detected route configured, trying to get one");
  
  m_autoconfInProgress = true;
  [m_operationQueue addOperationWithBlock:^{
      NSTask *task = [[NSTask alloc] init];
      [task setLaunchPath: @NDND_AUTOCONFIG_COMMAND];
      [task launch];
      [task waitUntilExit];

      m_autoconfInProgress = false;
    }];
}

@end
