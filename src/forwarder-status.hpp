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

#ifndef NCC_FORWARDER_STATUS_HPP
#define NCC_FORWARDER_STATUS_HPP

#include <QAbstractListModel>
#include <QStringList>
#include <QDateTime>

#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/mgmt/nfd/forwarder-status.hpp>

namespace ndn {

class ForwarderStatusItem
{
public:
  ForwarderStatusItem(const QString& type, const QString& value)
    : m_type(type)
    , m_value(value)
  {
  }

  ForwarderStatusItem(const QString& type, const std::string& value)
    : m_type(type)
    , m_value(QString::fromStdString(value))
  {
  }

  ForwarderStatusItem(const QString& type, uint64_t value)
    : m_type(type)
    , m_value(QString::number(value))
  {
  }

  ForwarderStatusItem(const QString& type, const time::system_clock::TimePoint& value)
    : ForwarderStatusItem(type, time::toString(value))
  {
  }

  const QString&
  type() const
  {
    return m_type;
  }

  const QString&
  value() const
  {
    return m_value;
  }

private:
  QString m_type;
  QString m_value;
};


class ForwarderStatusModel : public QAbstractListModel
{
  Q_OBJECT

signals:
  void
  onDataReceived(ndn::nfd::ForwarderStatus status);

public:

  enum ForwarderStatusRoles {
    TypeRole = Qt::UserRole + 1,
    ValueRole
  };

  explicit
  ForwarderStatusModel(QObject* parent = 0);

  int
  rowCount(const QModelIndex& parent = QModelIndex()) const;

  void
  addItem(const ForwarderStatusItem& item);

  QVariant
  data(const QModelIndex& index, int role) const;

  QHash<int, QByteArray>
  roleNames() const;

  void
  clear();

  void
  onTimeout(const Interest& interest);

private slots:

  void
  updateStatus(ndn::nfd::ForwarderStatus status);

private:
  QList<ForwarderStatusItem> m_items;
};

} // namespace ndn

#endif // NCC_FORWARDER_STATUS_HPP
