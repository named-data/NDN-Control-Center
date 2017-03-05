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

#ifndef NCC_STRATEGY_STATUS_HPP
#define NCC_STRATEGY_STATUS_HPP

#include <QtCore/QAbstractListModel>
#include <QtCore/QStringList>

#include <ndn-cxx/mgmt/nfd/strategy-choice.hpp>

namespace ndn {

class StrategyStatusItem
{
public:
  StrategyStatusItem(const QString& prefix, const QString& strategy)
    : m_prefix(prefix)
    , m_strategy(strategy)
  {
  }

  const QString&
  prefix() const
  {
    return m_prefix;
  }

  const QString&
  strategy() const
  {
    return m_strategy;
  }

private:
  QString m_prefix;
  QString m_strategy;
};

class StrategyStatusModel : public QAbstractListModel
{
  Q_OBJECT

signals:
  void
  onDataReceived(std::vector<ndn::nfd::StrategyChoice> status);

public:
  enum StrategyStatusRoles {
    PrefixRole = Qt::UserRole + 1,
    StrategyRole
  };

  explicit
  StrategyStatusModel(QObject* parent = 0);

  int
  rowCount(const QModelIndex& parent = QModelIndex()) const;

  void
  addItem(const StrategyStatusItem& item);

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
  updateStatus(std::vector<ndn::nfd::StrategyChoice> status);

private:
  QList<StrategyStatusItem> m_items;
};

} // namespace ndn

#endif // NCC_STRATEGY_STATUS_HPP
