/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Zhenkai Zhu <zhenkai@cs.ucla.edu>
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
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
