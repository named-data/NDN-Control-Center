/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2013-2017, Regents of the University of California,
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

#include "osx-adhoc.hpp"

#include <ndn-cxx/name.hpp>
#include <ndn-cxx/mgmt/nfd/control-parameters.hpp>

#ifdef OSX_BUILD

#import <CoreWLAN/CoreWLAN.h>
#import <CoreWLAN/CoreWLANConstants.h>
#import <CoreWLAN/CWInterface.h>
#import <CoreWLAN/CWWiFiClient.h>
#import <CoreWLAN/CoreWLANTypes.h>

namespace ndn {
namespace ncc {

const std::string adhocUri = "udp4://224.0.23.170:56363";
const std::set<Name> adhocPrefixes = {"/", "/adhoc"};
const NSUInteger g_channel = 11;
const uint16_t ROUTE_ORIGIN_NCC = 67;

Adhoc::Adhoc(Face& face, KeyChain& keychain)
  : m_face(face)
  , m_keyChain(keychain)
  , m_controller(new nfd::Controller(m_face, m_keyChain))
  , m_faceMonitor(new nfd::FaceMonitor(m_face))
{
  m_faceMonitor->onNotification.connect(bind(&Adhoc::onNotification, this, _1));
}

Adhoc::~Adhoc()
{
  delete m_faceMonitor;
  delete m_controller;
}

bool
Adhoc::createAdhoc()
{
  m_faceMonitor->start();

  NSString* networkName =
    [[NSString alloc] initWithCString:"NDNdirect" encoding:NSASCIIStringEncoding];
  NSString* passphrase =
    [[NSString alloc] initWithCString:"NDNhello" encoding:NSASCIIStringEncoding];
  NSArray* airportInterfaces = [CWWiFiClient interfaceNames];
  NSString* interfaceName = [airportInterfaces objectAtIndex:0];
  CWWiFiClient* wifiInterfaces = [[CWWiFiClient alloc] init];
  CWInterface* airport = [wifiInterfaces interfaceWithName:interfaceName];

  NSError* error = nil;
  NSData* data = [networkName dataUsingEncoding:NSUTF8StringEncoding];
  BOOL created = [airport startIBSSModeWithSSID:data
                                       security:kCWIBSSModeSecurityNone
                                        channel:g_channel
                                       password:passphrase
                                          error:&error];

  if (!created) {
    return false;
  }
  else {
    unregisterRoutes(ndn::nfd::ROUTE_ORIGIN_AUTOCONF);
    return true;
  }
}

void
Adhoc::destroyAdhoc()
{
  unregisterRoutes(ROUTE_ORIGIN_NCC);
  m_faceMonitor->stop();

  NSArray* airportInterfaces = [CWWiFiClient interfaceNames];
  NSString* interfaceName = [airportInterfaces objectAtIndex:0];
  CWWiFiClient* wifiInterfaces = [[CWWiFiClient alloc] init];
  CWInterface* airport = [wifiInterfaces interfaceWithName:interfaceName];
  [airport disassociate];

  NSError* err = nil;

  [airport setPower:NO error:&err];
  [NSThread sleepForTimeInterval: 3.0];
  [airport setPower:YES error:&err];
}

void
Adhoc::onNotification(const ndn::nfd::FaceEventNotification& notification)
{
  if (notification.getKind() == ndn::nfd::FACE_EVENT_CREATED &&
      notification.getRemoteUri() == adhocUri &&
      notification.getLinkType() == ndn::nfd::LINK_TYPE_MULTI_ACCESS) {
    for (const auto& prefix : adhocPrefixes) {
      ndn::nfd::ControlParameters params;
      params
        .setName(prefix)
        .setFaceId(notification.getFaceId())
        .setOrigin(ROUTE_ORIGIN_NCC);
      m_controller->start<ndn::nfd::RibRegisterCommand>(params, nullptr, nullptr);
    }
  }
}

void
Adhoc::unregisterRoutes(uint16_t origin)
{
  m_controller->fetch<ndn::nfd::RibDataset>([this, origin] (const std::vector<nfd::RibEntry>& ribs) {
      for (const auto& rib : ribs) {
        for (const auto& route : rib.getRoutes()) {
          if (route.getOrigin() == origin) {
            ndn::nfd::ControlParameters params;
            params.setFaceId(route.getFaceId());
            m_controller->start<ndn::nfd::FaceDestroyCommand>(params, nullptr, nullptr);
          }
        }
      }
    }, nullptr);
}

} // namespace ncc
} // namespace ndn

#endif // OSX_BUILD
