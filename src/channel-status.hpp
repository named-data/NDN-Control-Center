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

#ifndef NCC_CHANNEL_STATUS_HPP
#define NCC_CHANNEL_STATUS_HPP

#include <QtCore/QAbstractListModel>
#include <QtCore/QStringList>

#include <ndn-cxx/mgmt/nfd/channel-status.hpp>

namespace ndn {

class ChannelStatusItem
{
public:
  ChannelStatusItem(const QString& channel)
    : m_channel(channel)
  {
  }

  const QString&
  channel() const
  {
    return m_channel;
  }

private:
  QString m_channel;
};


class ChannelStatusModel : public QAbstractListModel
{
  Q_OBJECT

signals:
  void
  onDataReceived(std::vector<ndn::nfd::ChannelStatus> status);

public:

  enum ChannelStatusRoles {
    ChannelRole = Qt::UserRole + 1
  };

  explicit
  ChannelStatusModel(QObject* parent = 0);

  int
  rowCount(const QModelIndex& parent = QModelIndex()) const;

  void
  addItem(const ChannelStatusItem& item);

  QVariant
  data(const QModelIndex& index, int role) const;

  QHash<int, QByteArray>
  roleNames() const;

  void
  clear();

  void
  onTimeout();

private slots:

  void
  updateStatus(std::vector<ndn::nfd::ChannelStatus> status);

private:
  QList<ChannelStatusItem> m_items;
};

} // namespace ndn

#endif // NCC_CHANNEL_STATUS_HPP
