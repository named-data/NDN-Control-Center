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

#include "status-viewer.hpp"
#include "status-viewer.moc"

namespace ndn {

StatusViewer::StatusViewer(Face& face, KeyChain& keyChain)
  : m_face(face)
  , m_keyChain(keyChain)
  , m_controller(new nfd::Controller(m_face, m_keyChain))
  , m_scheduler(m_face.getIoService())
  , m_nextStatusRetrieval(m_scheduler)
{
  QQmlContext* m_context = m_engine.rootContext();

  m_context->setContextProperty("forwarderModel", &m_forwarderStatusModel);
  m_context->setContextProperty("channelModel", &m_channelModel);
  m_context->setContextProperty("faceModel", &m_faceModel);
  m_context->setContextProperty("fibModel", &m_fibModel);
  m_context->setContextProperty("ribModel", &m_ribModel);
  m_context->setContextProperty("strategyModel", &m_strategyModel);
  m_context->setContextProperty("statusViewer", this);

  m_engine.load((QUrl("qrc:/status.qml")));
}

void
StatusViewer::onStatusRetrieved(const nfd::ForwarderStatus& status)
{
  emit m_forwarderStatusModel.onDataReceived(status);
}

void
StatusViewer::onChannelStatusRetrieved(const std::vector<nfd::ChannelStatus>& status)
{
  emit m_channelModel.onDataReceived(status);
}

void
StatusViewer::onFaceStatusRetrieved(const std::vector<nfd::FaceStatus>& status)
{
  emit m_faceModel.onDataReceived(status);
}

void
StatusViewer::onFibStatusRetrieved(const std::vector<nfd::FibEntry>& status)
{
  emit m_fibModel.onDataReceived(status);
}

void
StatusViewer::onRibStatusRetrieved(const std::vector<nfd::RibEntry>& status)
{
  emit m_ribModel.onDataReceived(status);
}

void
StatusViewer::onStrategyChoiceStatusRetrieved(const std::vector<nfd::StrategyChoice>& status)
{
  emit m_strategyModel.onDataReceived(status);
}

void
StatusViewer::onStatusTimeout()
{
  std::cerr << "Should not really happen, most likely a serious problem" << std::endl;
  m_scheduler.scheduleEvent(time::seconds(15), bind(&StatusViewer::requestNfdStatus, this));
}

void
StatusViewer::cancelEvents()
{
  m_nextStatusRetrieval.cancel();
  std::cerr << "Future events canceled" << std::endl;
}

void
StatusViewer::requestNfdStatus()
{
  std::cerr << "requestNfdStatus" << std::endl;

  m_controller->fetch<ndn::nfd::ForwarderGeneralStatusDataset>(bind(&StatusViewer::onStatusRetrieved, this, _1),
                                                               bind(&StatusViewer::onStatusTimeout, this));
  m_controller->fetch<ndn::nfd::ChannelDataset>(bind(&StatusViewer::onChannelStatusRetrieved, this, _1),
                                                bind(&StatusViewer::onStatusTimeout, this));
  m_controller->fetch<ndn::nfd::FaceDataset>(bind(&StatusViewer::onFaceStatusRetrieved, this, _1),
                                             bind(&StatusViewer::onStatusTimeout, this));
  m_controller->fetch<ndn::nfd::FibDataset>(bind(&StatusViewer::onFibStatusRetrieved, this, _1),
                                            bind(&StatusViewer::onStatusTimeout, this));
  m_controller->fetch<ndn::nfd::RibDataset>(bind(&StatusViewer::onRibStatusRetrieved, this, _1),
                                            bind(&StatusViewer::onStatusTimeout, this));
  m_controller->fetch<ndn::nfd::StrategyChoiceDataset>(bind(&StatusViewer::onStrategyChoiceStatusRetrieved, this, _1),
                                                       bind(&StatusViewer::onStatusTimeout, this));
  m_nextStatusRetrieval = m_scheduler.scheduleEvent(time::seconds(15), bind(&StatusViewer::requestNfdStatus, this));
}

void
StatusViewer::present()
{
  requestNfdStatus();
  emit showStatus();
}

} // namespace ndn
