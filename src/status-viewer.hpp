/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2017, Regents of the University of California,
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

#ifndef NCC_STATUS_VIEWER_HPP
#define NCC_STATUS_VIEWER_HPP

#include "forwarder-status.hpp"
#include "channel-status.hpp"
#include "face-status.hpp"
#include "fib-status.hpp"
#include "rib-status.hpp"
#include "strategy-status.hpp"

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/name.hpp>
#include <ndn-cxx/util/scheduler.hpp>
#include <ndn-cxx/mgmt/nfd/channel-status.hpp>
#include <ndn-cxx/mgmt/nfd/face-status.hpp>
#include <ndn-cxx/mgmt/nfd/fib-entry.hpp>
#include <ndn-cxx/mgmt/nfd/rib-entry.hpp>
#include <ndn-cxx/mgmt/nfd/strategy-choice.hpp>
#include <ndn-cxx/mgmt/nfd/controller.hpp>
#include <ndn-cxx/mgmt/nfd/status-dataset.hpp>

#include <QObject>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlApplicationEngine>

namespace ndn {

class StatusViewer : public QObject
{
  Q_OBJECT
public:
  explicit
  StatusViewer(Face& face, KeyChain& keyChain);

  void
  onStatusRetrieved(const nfd::ForwarderStatus& status);

  void
  onChannelStatusRetrieved(const std::vector<nfd::ChannelStatus>& status);

  void
  onFaceStatusRetrieved(const std::vector<nfd::FaceStatus>& status);

  void
  onFibStatusRetrieved(const std::vector<nfd::FibEntry>& status);

  void
  onRibStatusRetrieved(const std::vector<nfd::RibEntry>& status);

  void
  onStrategyChoiceStatusRetrieved(const std::vector<nfd::StrategyChoice>& status);

  void
  onStatusTimeout();

private:
  void
  requestNfdStatus();

signals:
  void
  showStatus();

public slots:
  void
  present();

private:
  Face& s_face;
  KeyChain& s_keyChain;
  nfd::Controller* s_controller;
  Scheduler s_scheduler;

  QQmlApplicationEngine s_engine;

  ForwarderStatusModel s_forwarderStatusModel;
  ChannelStatusModel s_channelModel;
  FaceStatusModel s_faceModel;
  FibStatusModel s_fibModel;
  RibStatusModel s_ribModel;
  StrategyStatusModel s_strategyModel;
};

} // namespace ndn

#endif // NCC_STATUS_VIEWER_HPP
