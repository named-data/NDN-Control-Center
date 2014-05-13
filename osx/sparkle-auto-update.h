/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (c) 2013-2014, Regents of the University of California,
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
 *
 * \author Alexander Afanasyev <http://lasr.cs.ucla.edu/afanasyev/index.html>
 * \author Ilya Moiseenko <http://ilyamoiseenko.com/>
 */

#ifndef SPARKLE_AUTO_UPDATE_H
#define SPARKLE_AUTO_UPDATE_H

/**
 * @brief Class implementing interface for automatic updates for OSX using Sparkle framework
 */
class SparkleAutoUpdate
{
public:
  /**
   * @brief Constructor
   * @param updateUrl URL to Sparkle update definition file
   */
  SparkleAutoUpdate (const char *updateUrl);
  ~SparkleAutoUpdate ();

  /**
   * @brief Check for updates once
   */
  void
  checkForUpdates ();

private:
  class Private;
  Private *d;
};

#endif // SPARKLE_AUTO_UPDATE_H
