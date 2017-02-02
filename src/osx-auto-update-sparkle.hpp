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

#ifndef NCC_OSX_AUTO_UPDATE_SPARKLE_HPP
#define NCC_OSX_AUTO_UPDATE_SPARKLE_HPP

#include "config.hpp"

#ifndef OSX_BUILD
#error "Cannot be included for non-OSX build"
#endif // OSX_BUILD

#include <string>
#include <ndn-cxx/util/backports.hpp>

namespace ndn {
namespace ncc {

class OsxAutoUpdateSparkle
{
public:
  OsxAutoUpdateSparkle(const std::string& updateUrl);

  ~OsxAutoUpdateSparkle();

  void
  checkForUpdates();

private:
  class Impl;
  unique_ptr<Impl> m_impl;
};

} // namespace ncc
} // namespace ndn

#endif // NCC_OSX_AUTO_UPDATE_SPARKLE_HPP
