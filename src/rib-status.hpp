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

#ifndef NCC_RIB_STATUS_HPP
#define NCC_RIB_STATUS_HPP

#include <QtCore/QAbstractListModel>
#include <QtCore/QStringList>

#include <ndn-cxx/mgmt/nfd/rib-entry.hpp>

namespace ndn {

class RibStatusItem
{
public:
  RibStatusItem(const QString& prefix, uint64_t faceId, uint64_t origin, uint64_t cost, bool child, bool ribcap,
                const QString& exp)
    : m_prefix(prefix)
    , m_faceId(faceId)
    , m_origin(origin)
    , m_cost(cost)
    , m_child(child)
    , m_ribcap(ribcap)
    , m_exp(exp)
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
  origin() const
  {
    return m_origin;
  }

  uint64_t
  cost() const
  {
    return m_cost;
  }

  bool
  child() const
  {
    return m_child;
  }

  bool
  ribcap() const
  {
    return m_ribcap;
  }

  const QString&
  exp() const
  {
    return m_exp;
  }

private:
  QString m_prefix;
  uint64_t m_faceId;
  uint64_t m_origin;
  uint64_t m_cost;
  bool m_child;
  bool m_ribcap;
  QString m_exp;
};

class RibStatusModel : public QAbstractListModel
{
  Q_OBJECT

signals:
  void
  onDataReceived(std::vector<ndn::nfd::RibEntry> status);

public:
  enum RibStatusRoles {
    PrefixRole = Qt::UserRole + 1,
    FaceIdRole,
    OriginRole,
    CostRole,
    ChildRole,
    RibCapRole,
    ExpRole
  };

  explicit
  RibStatusModel(QObject* parent = 0);

  int
  rowCount(const QModelIndex& parent = QModelIndex()) const;

  void
  addItem(const RibStatusItem& item);

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
  updateStatus(std::vector<ndn::nfd::RibEntry> status);

private:
  QList<RibStatusItem> m_items;
};

} // namespace ndn

#endif // NCC_RIB_STATUS_HPP
