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

#ifndef NCC_FIB_STATUS_HPP
#define NCC_FIB_STATUS_HPP

#include <QtCore/QAbstractListModel>
#include <QtCore/QStringList>

#include <ndn-cxx/mgmt/nfd/fib-entry.hpp>

namespace ndn {

class FibStatusItem
{
public:
  FibStatusItem(const QString& prefix, uint64_t faceId, uint64_t cost)
    : m_prefix(prefix)
    , m_faceId(faceId)
    , m_cost(cost)
  {
  }

  const QString&
  prefix() const
  {
    return m_prefix;
  }

  uint64_t
  faceId() const
  {
    return m_faceId;
  }

  uint64_t
  cost() const
  {
    return m_cost;
  }

private:
  QString m_prefix;
  uint64_t m_faceId;
  uint64_t m_cost;
};

class FibStatusModel : public QAbstractListModel
{
  Q_OBJECT

signals:
  void
  onDataReceived(std::vector<ndn::nfd::FibEntry> status);

public:
  enum FibStatusRoles {
    PrefixRole = Qt::UserRole + 1,
    FaceIdRole,
    CostRole
  };

  explicit
  FibStatusModel(QObject* parent = 0);

  int
  rowCount(const QModelIndex& parent = QModelIndex()) const;

  void
  addItem(const FibStatusItem& item);

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
  updateStatus(std::vector<ndn::nfd::FibEntry> status);

private:
  QList<FibStatusItem> m_items;
};

} // namespace ndn

#endif // NCC_FIB_STATUS_HPP
