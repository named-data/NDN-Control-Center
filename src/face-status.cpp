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

#include "face-status.hpp"
#include "face-status.moc"

namespace ndn {

FaceStatusModel::FaceStatusModel(QObject* parent/* = 0*/)
  : QAbstractListModel(parent)
{
  connect(this, SIGNAL(onDataReceived(std::vector<ndn::nfd::FaceStatus>)), this,
          SLOT(updateStatus(std::vector<ndn::nfd::FaceStatus>)),
          Qt::QueuedConnection);
}

int
FaceStatusModel::rowCount(const QModelIndex& parent/* = QModelIndex()*/) const
{
  return m_items.count();
}

void
FaceStatusModel::addItem(const FaceStatusItem& item)
{
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_items << item;
  endInsertRows();
}

QVariant
FaceStatusModel::data(const QModelIndex& index, int role) const
{
  if (index.row() < 0 || index.row() >= m_items.count()) {
    return QVariant();
  }

  const FaceStatusItem& item = m_items.at(index.row());
  if (role == FaceIdRole) {
    return static_cast<uint>(item.faceId());
  }
  else if (role == RemoteRole) {
    return item.remote();
  }
  else if (role == LocalRole) {
    return item.local();
  }
  else if (role == ScopeRole) {
    return item.scope();
  }
  else if (role == PersistencyRole) {
    return item.persistency();
  }
  else if (role == LinkTypeRole) {
    return item.linkType();
  }
  else if (role == InInterestRole) {
    return static_cast<uint>(item.inInterest());
  }
  else if (role == OutInterestRole) {
    return static_cast<uint>(item.outInterest());
  }
  else if (role == InDataRole) {
    return static_cast<uint>(item.inData());
  }
  else if (role == OutDataRole) {
    return static_cast<uint>(item.outData());
  }
  else if (role == InByteRole) {
    return static_cast<uint>(item.inByte());
  }
  else if (role == OutByteRole) {
    return static_cast<uint>(item.outByte());
  }
  else if (role == InNackRole) {
    return static_cast<uint>(item.inNack());
  }
  else if (role == OutNackRole) {
    return static_cast<uint>(item.outNack());
  }

  return QVariant();
}

QHash<int, QByteArray>
FaceStatusModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[FaceIdRole] = "faceId";
  roles[RemoteRole] = "remote";
  roles[LocalRole] = "local";
  roles[ScopeRole] = "scope";
  roles[PersistencyRole] = "persistency";
  roles[LinkTypeRole] = "linkType";
  roles[InInterestRole] = "inInterest";
  roles[OutInterestRole] = "outInterest";
  roles[InDataRole] = "inData";
  roles[OutDataRole] = "outData";
  roles[InByteRole] = "inByte";
  roles[OutByteRole] = "outByte";
  roles[InNackRole] = "inNack";
  roles[OutNackRole] = "outNack";
  return roles;
}

void
FaceStatusModel::clear()
{
  beginResetModel();
  m_items.clear();
  endResetModel();
}

void
FaceStatusModel::updateStatus(std::vector<ndn::nfd::FaceStatus> status)
{
  beginResetModel();
  m_items.clear();
  for (auto const& faceEntry : status) {
    QString faceScope = (faceEntry.getFaceScope() == ndn::nfd::FACE_SCOPE_LOCAL) ? QString("Local") : QString("Non-Local");
    QString facePersistency = (faceEntry.getFacePersistency() == ndn::nfd::FACE_PERSISTENCY_PERSISTENT) ? QString("Persistent") :
                              ((faceEntry.getFacePersistency() == ndn::nfd::FACE_PERSISTENCY_PERMANENT) ? QString("Permanent") :
                              QString("On-Demand"));
    QString faceLinkType = (faceEntry.getLinkType() == ndn::nfd::LINK_TYPE_MULTI_ACCESS) ? QString("Multi-Access") :
                           QString("Point-to-Point");
    addItem(FaceStatusItem(faceEntry.getFaceId(), QString::fromStdString(faceEntry.getRemoteUri()),
                           QString::fromStdString(faceEntry.getLocalUri()), faceScope, facePersistency, faceLinkType,
                           faceEntry.getNInInterests(), faceEntry.getNOutInterests(), faceEntry.getNInDatas(),
                           faceEntry.getNOutDatas(), faceEntry.getNInBytes(), faceEntry.getNOutBytes(),
                           faceEntry.getNInNacks(), faceEntry.getNOutNacks()));
  }
  endResetModel();
}

void
FaceStatusModel::onTimeout()
{
  std::cerr << "Request timed out (should not really happen)" << std::endl;
}

} // namespace ndn
