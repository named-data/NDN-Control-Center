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

#include "strategy-status.hpp"
#include "strategy-status.moc"

namespace ndn {

StrategyStatusModel::StrategyStatusModel(QObject* parent/* = 0*/)
  : QAbstractListModel(parent)
{
  connect(this, SIGNAL(onDataReceived(std::vector<ndn::nfd::StrategyChoice>)), this,
          SLOT(updateStatus(std::vector<ndn::nfd::StrategyChoice>)),
          Qt::QueuedConnection);
}

int
StrategyStatusModel::rowCount(const QModelIndex& parent/* = QModelIndex()*/) const
{
  return m_items.count();
}

void
StrategyStatusModel::addItem(const StrategyStatusItem& item)
{
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_items << item;
  endInsertRows();
}

QVariant
StrategyStatusModel::data(const QModelIndex& index, int role) const
{
  if (index.row() < 0 || index.row() >= m_items.count()) {
    return QVariant();
  }

  const StrategyStatusItem& item = m_items.at(index.row());
  if (role == PrefixRole) {
    return item.prefix();
  }
  else if (role == StrategyRole) {
    return item.strategy();
  }

  return QVariant();
}

QHash<int, QByteArray>
StrategyStatusModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[PrefixRole] = "prefix";
  roles[StrategyRole] = "strategy";
  return roles;
}

void
StrategyStatusModel::clear()
{
  beginResetModel();
  m_items.clear();
  endResetModel();
}

void
StrategyStatusModel::updateStatus(std::vector<ndn::nfd::StrategyChoice> status)
{
  beginResetModel();
  m_items.clear();
  for (auto const& strategy : status) {
    addItem(StrategyStatusItem(QString::fromStdString(strategy.getName().toUri()),
                               QString::fromStdString(strategy.getStrategy().toUri())));
  }
  endResetModel();
}

void
StrategyStatusModel::onTimeout()
{
  std::cerr << "Request timed out (should not really happen)" << std::endl;
}

} // namespace ndn
