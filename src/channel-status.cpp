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

#include "channel-status.hpp"
#include "channel-status.moc"

namespace ndn {

ChannelStatusModel::ChannelStatusModel(QObject* parent/* = 0*/)
  : QAbstractListModel(parent)
{
  connect(this, SIGNAL(onDataReceived(std::vector<ndn::nfd::ChannelStatus>)), this,
          SLOT(updateStatus(std::vector<ndn::nfd::ChannelStatus>)),
          Qt::QueuedConnection);
}

int
ChannelStatusModel::rowCount(const QModelIndex& parent/* = QModelIndex()*/) const
{
  return m_items.count();
}

void
ChannelStatusModel::addItem(const ChannelStatusItem& item)
{
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_items << item;
  endInsertRows();
}

QVariant
ChannelStatusModel::data(const QModelIndex& index, int role) const
{
  if (index.row() < 0 || index.row() >= m_items.count()) {
    return QVariant();
  }

  const ChannelStatusItem& item = m_items.at(index.row());
  if (role == ChannelRole) {
    return item.channel();
  }

  return QVariant();
}

QHash<int, QByteArray>
ChannelStatusModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[ChannelRole] = "channel";
  return roles;
}

void
ChannelStatusModel::clear()
{
  beginResetModel();
  m_items.clear();
  endResetModel();
}

void
ChannelStatusModel::updateStatus(std::vector<ndn::nfd::ChannelStatus> status)
{
  beginResetModel();
  m_items.clear();
  for (auto const& channel : status) {
    addItem(ChannelStatusItem(QString::fromStdString(channel.getLocalUri())));
  }
  endResetModel();
}

void
ChannelStatusModel::onTimeout()
{
  std::cerr << "Request timed out (should not really happen)" << std::endl;
}

} // namespace ndn
