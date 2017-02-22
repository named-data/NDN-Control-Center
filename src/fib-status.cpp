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

#include "fib-status.hpp"
#include "fib-status.moc"

namespace ndn {

FibStatusModel::FibStatusModel(QObject* parent/* = 0*/)
  : QAbstractListModel(parent)
{
  connect(this, SIGNAL(onDataReceived(std::vector<ndn::nfd::FibEntry>)), this,
          SLOT(updateStatus(std::vector<ndn::nfd::FibEntry>)),
          Qt::QueuedConnection);
}

int
FibStatusModel::rowCount(const QModelIndex& parent/* = QModelIndex()*/) const
{
  return m_items.count();
}

void
FibStatusModel::addItem(const FibStatusItem& item)
{
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_items << item;
  endInsertRows();
}

QVariant
FibStatusModel::data(const QModelIndex& index, int role) const
{
  if (index.row() < 0 || index.row() >= m_items.count()) {
    return QVariant();
  }

  const FibStatusItem& item = m_items.at(index.row());
  if (role == PrefixRole) {
    return item.prefix();
  }
  else if (role == FaceIdRole) {
    return static_cast<uint>(item.faceId());
  }
  else if (role == CostRole) {
    return static_cast<uint>(item.cost());
  }

  return QVariant();
}

QHash<int, QByteArray>
FibStatusModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[PrefixRole] = "prefix";
  roles[FaceIdRole] = "faceId";
  roles[CostRole] = "cost";
  return roles;
}

void
FibStatusModel::clear()
{
  beginResetModel();
  m_items.clear();
  endResetModel();
}

void
FibStatusModel::updateStatus(std::vector<ndn::nfd::FibEntry> status)
{
  beginResetModel();
  m_items.clear();
  for (auto const& fibEntry : status) {
    bool isSamePrefix = false;
    for (auto const& nextHop : fibEntry.getNextHopRecords()) {
      if (!isSamePrefix) {
        addItem(FibStatusItem(QString::fromStdString(fibEntry.getPrefix().toUri()),
                              nextHop.getFaceId(), nextHop.getCost()));
        isSamePrefix = true;
      }
      else {
        addItem(FibStatusItem(QString(""), nextHop.getFaceId(), nextHop.getCost()));
      }
    }
  }
  endResetModel();
}

void
FibStatusModel::onTimeout()
{
  std::cerr << "Request timed out (should not really happen)" << std::endl;
}

} // namespace ndn
