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
  : s_face(face)
  , s_keyChain(keyChain)
  , s_controller(new nfd::Controller(s_face, s_keyChain))
  , s_scheduler(s_face.getIoService())
{
  QQmlContext* s_context = s_engine.rootContext();

  s_context->setContextProperty("forwarderModel", &s_forwarderStatusModel);
  s_context->setContextProperty("faceModel", &s_faceModel);
  s_context->setContextProperty("fibModel", &s_fibModel);
  s_context->setContextProperty("ribModel", &s_ribModel);
  s_context->setContextProperty("statusViewer", this);

  s_engine.load((QUrl("qrc:/status.qml")));
}

void
StatusViewer::onStatusRetrieved(const nfd::ForwarderStatus& status)
{
  emit s_forwarderStatusModel.onDataReceived(status);
}

void
StatusViewer::onFaceStatusRetrieved(const std::vector<nfd::FaceStatus>& status)
{
  emit s_faceModel.onDataReceived(status);
}

void
StatusViewer::onFibStatusRetrieved(const std::vector<nfd::FibEntry>& status)
{
  emit s_fibModel.onDataReceived(status);
}

void
StatusViewer::onRibStatusRetrieved(const std::vector<nfd::RibEntry>& status)
{
  emit s_ribModel.onDataReceived(status);
}

void
StatusViewer::onStatusTimeout()
{
  std::cerr << "Should not really happen, most likely a serious problem" << std::endl;
  s_scheduler.scheduleEvent(time::seconds(15), bind(&StatusViewer::requestNfdStatus, this));
}

void
StatusViewer::requestNfdStatus()
{
  s_controller->fetch<ndn::nfd::ForwarderGeneralStatusDataset>(bind(&StatusViewer::onStatusRetrieved, this, _1),
                                                              bind(&StatusViewer::onStatusTimeout, this));
  s_controller->fetch<ndn::nfd::FaceDataset>(bind(&StatusViewer::onFaceStatusRetrieved, this, _1),
                                             bind(&StatusViewer::onStatusTimeout, this));
  s_controller->fetch<ndn::nfd::FibDataset>(bind(&StatusViewer::onFibStatusRetrieved, this, _1),
                                           bind(&StatusViewer::onStatusTimeout, this));
  s_controller->fetch<ndn::nfd::RibDataset>(bind(&StatusViewer::onRibStatusRetrieved, this, _1),
                                           bind(&StatusViewer::onStatusTimeout, this));
  s_scheduler.scheduleEvent(time::seconds(15), bind(&StatusViewer::requestNfdStatus, this));
}

void
StatusViewer::present()
{
  requestNfdStatus();
  emit showStatus();
}

} // namespace ndn
