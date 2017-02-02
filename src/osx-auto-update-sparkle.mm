/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2013-2017, Regents of the University of California.
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
 */

#include "osx-auto-update-sparkle.hpp"

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#import <Sparkle/Sparkle.h>

@interface UpdaterDelegate : NSObject<SUUpdaterDelegate> {
  ndn::ncc::OsxAutoUpdateSparkle* this_;
}

- (void)setSelf:(ndn::ncc::OsxAutoUpdateSparkle*)ptr;

- (BOOL)updater:(SUUpdater *)updater
        shouldPostponeRelaunchForUpdate:(SUAppcastItem *)update
        untilInvoking:(NSInvocation *)invocation;

@end

@implementation UpdaterDelegate

- (void)setSelf:(ndn::ncc::OsxAutoUpdateSparkle*)ptr
{
  this_ = ptr;
}

- (BOOL)updater:(SUUpdater *)updater
        shouldPostponeRelaunchForUpdate:(SUAppcastItem *)update
        untilInvoking:(NSInvocation *)invocation
{
  NSTask *task = [[NSTask alloc] init];
  task.launchPath = @"/usr/bin/killall";
  task.arguments = @[@"ndn-autoconfig", @"nfd"];
  [task launch];
  return NO;
}

@end

namespace ndn {
namespace ncc {

class OsxAutoUpdateSparkle::Impl
{
public:
  SUUpdater* m_updater;
  UpdaterDelegate* delegate;
};

OsxAutoUpdateSparkle::OsxAutoUpdateSparkle(const std::string& updateUrl)
  : m_impl(make_unique<Impl>())
{
  m_impl->m_updater = [[SUUpdater sharedUpdater] retain];
  NSURL* url = [NSURL URLWithString:[NSString stringWithUTF8String:updateUrl.data()]];
  [m_impl->m_updater setFeedURL:url];
  [m_impl->m_updater setAutomaticallyChecksForUpdates:YES];
  [m_impl->m_updater setUpdateCheckInterval:86400];

  m_impl->delegate = [[UpdaterDelegate alloc] init];
  [m_impl->delegate setSelf:this];
  [m_impl->m_updater setDelegate:m_impl->delegate];
}

OsxAutoUpdateSparkle::~OsxAutoUpdateSparkle()
{
  [m_impl->m_updater release];
  // presummably SUUpdater handles garbage collection
}

void
OsxAutoUpdateSparkle::checkForUpdates()
{
  //[m_impl->m_updater checkForUpdatesInBackground];
  [m_impl->m_updater checkForUpdates:nil];
}

} // namespace ncc
} // namespace ndn
