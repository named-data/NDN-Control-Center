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

#ifndef NCC_OSX_ADHOC_HPP
#define NCC_OSX_ADHOC_HPP

#include "config.hpp"

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/mgmt/nfd/controller.hpp>
#include <ndn-cxx/mgmt/nfd/face-monitor.hpp>

#ifdef OSX_BUILD
#define ADHOC_SUPPORTED 1
#endif // OSX_BUILD

#ifdef ADHOC_SUPPORTED

namespace ndn {
namespace ncc {

class Adhoc
{
public:
  Adhoc(Face& face, KeyChain& keychain);

  ~Adhoc();

  bool
  createAdhoc();

  void
  onNotification(const ndn::nfd::FaceEventNotification& notification);

  void
  destroyAdhoc();

private:
  void
  unregisterRoutes(uint16_t origin);

private:
  Face& m_face;
  KeyChain& m_keyChain;
  nfd::Controller* m_controller;
  nfd::FaceMonitor* m_faceMonitor;
};

} // namespace ncc
} // namespace ndn

#endif // ADHOC_SUPPORTED

#endif // NCC_OSX_ADHOC_HPP
