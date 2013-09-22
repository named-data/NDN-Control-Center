/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Zhenkai Zhu <zhenkai@cs.ucla.edu>
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "sparkle-auto-update.h"

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <Sparkle/Sparkle.h>

class SparkleAutoUpdate::Private
{
public:
  SUUpdater *updater;
};

SparkleAutoUpdate::SparkleAutoUpdate(const char *updateUrl)
{
  d = new Private;
  d->updater = [[SUUpdater sharedUpdater] retain];
  NSURL *url = [NSURL URLWithString: [NSString stringWithCString:updateUrl encoding:NSASCIIStringEncoding]];
  [d->updater setFeedURL: url];
  [d->updater setAutomaticallyChecksForUpdates: YES];
  [d->updater setUpdateCheckInterval: 86400];
}

SparkleAutoUpdate::~SparkleAutoUpdate()
{
  [d->updater release];
  delete d;
  // presummably SUUpdater handles garbage collection
}

void SparkleAutoUpdate::checkForUpdates()
{
  //[d->updater checkForUpdatesInBackground];
  [d->updater checkForUpdates : nil];
}
