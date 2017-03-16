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

#include "rib-status.hpp"
#include "rib-status.moc"

namespace ndn {

RibStatusModel::RibStatusModel(QObject* parent/* = 0*/)
  : QAbstractListModel(parent)
{
  connect(this, SIGNAL(onDataReceived(std::vector<ndn::nfd::RibEntry>)), this,
          SLOT(updateStatus(std::vector<ndn::nfd::RibEntry>)),
          Qt::QueuedConnection);
}

int
RibStatusModel::rowCount(const QModelIndex& parent/* = QModelIndex()*/) const
{
  return m_items.count();
}

void
RibStatusModel::addItem(const RibStatusItem& item)
{
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_items << item;
  endInsertRows();
}

QVariant
RibStatusModel::data(const QModelIndex& index, int role) const
{
  if (index.row() < 0 || index.row() >= m_items.count()) {
    return QVariant();
  }

  const RibStatusItem& item = m_items.at(index.row());
  if (role == PrefixRole) {
    return item.prefix();
  }
  else if (role == FaceIdRole) {
    return static_cast<uint>(item.faceId());
  }
  else if (role == OriginRole) {
    return static_cast<uint>(item.origin());
  }
  else if (role == CostRole) {
    return static_cast<uint>(item.cost());
  }
  else if (role == ChildRole) {
    return item.child();
  }
  else if (role == RibCapRole) {
    return item.ribcap();
  }
  else if (role == ExpRole) {
    return item.exp();
  }

  return QVariant();
}

QHash<int, QByteArray>
RibStatusModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[PrefixRole] = "prefix";
  roles[FaceIdRole] = "faceId";
  roles[OriginRole] = "origin";
  roles[CostRole] = "cost";
  roles[ChildRole] = "childinherit";
  roles[RibCapRole] = "ribcapture";
  roles[ExpRole] = "expiresin";
  return roles;
}

void
RibStatusModel::clear()
{
  beginResetModel();
  m_items.clear();
  endResetModel();
}

void
RibStatusModel::updateStatus(std::vector<ndn::nfd::RibEntry> status)
{
  beginResetModel();
  m_items.clear();
  for (auto const& ribEntry : status) {
    bool isSamePrefix = false;
    for (auto const& route : ribEntry.getRoutes()) {
      QString expirationPeriod = route.hasExpirationPeriod() ?
        QString::fromStdString(std::to_string(route.getExpirationPeriod().count())) : QString("Never");
      if (!isSamePrefix) {
        addItem(RibStatusItem(QString::fromStdString(ribEntry.getName().toUri()), route.getFaceId(), route.getOrigin(),
                              route.getCost(), route.isChildInherit(), route.isRibCapture(), expirationPeriod));
        isSamePrefix = true;
      }
      else {
        addItem(RibStatusItem(QString(""), route.getFaceId(), route.getOrigin(), route.getCost(), route.isChildInherit(),
                              route.isRibCapture(), expirationPeriod));
      }
    }
  }
  endResetModel();
}

void
RibStatusModel::onTimeout()
{
  std::cerr << "Request timed out (should not really happen)" << std::endl;
}

} // namespace ndn
