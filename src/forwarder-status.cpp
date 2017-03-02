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

#include "forwarder-status.hpp"
#include "forwarder-status.moc"

namespace ndn {

ForwarderStatusModel::ForwarderStatusModel(QObject* parent/* = 0*/)
  : QAbstractListModel(parent)
{
  connect(this, SIGNAL(onDataReceived(ndn::nfd::ForwarderStatus)), this,
          SLOT(updateStatus(ndn::nfd::ForwarderStatus)),
          Qt::QueuedConnection);
}

int
ForwarderStatusModel::rowCount(const QModelIndex& parent/* = QModelIndex()*/) const
{
  return m_items.count();
}

void
ForwarderStatusModel::addItem(const ForwarderStatusItem& item)
{
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_items << item;
  endInsertRows();
}

QVariant
ForwarderStatusModel::data(const QModelIndex& index, int role) const
{
  if (index.row() < 0 || index.row() >= m_items.count()) {
    return QVariant();
  }

  const ForwarderStatusItem& item = m_items.at(index.row());
  if (role == TypeRole) {
    return item.type();
  }
  else if (role == ValueRole) {
    return item.value();
  }
  return QVariant();
}

QHash<int, QByteArray>
ForwarderStatusModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[TypeRole] = "type";
  roles[ValueRole] = "value";
  return roles;
}

void
ForwarderStatusModel::clear()
{
  beginResetModel();
  m_items.clear();
  endResetModel ();
}

void
ForwarderStatusModel::updateStatus(ndn::nfd::ForwarderStatus status)
{
  beginResetModel();
  m_items.clear();
  addItem(ForwarderStatusItem("Version",       QString::fromStdString(status.getNfdVersion())));
  addItem(ForwarderStatusItem("Start Time",     QString::fromStdString(time::toIsoString(status.getStartTimestamp()))));
  addItem(ForwarderStatusItem("Current Time",   QString::fromStdString(time::toIsoString(status.getCurrentTimestamp()))));
  addItem(ForwarderStatusItem("Name Tree Entries", QString::number(status.getNNameTreeEntries())));
  addItem(ForwarderStatusItem("Fib Entries",   QString::number(status.getNFibEntries())));
  addItem(ForwarderStatusItem("PitEntries",   QString::number(status.getNPitEntries())));
  addItem(ForwarderStatusItem("Measurements Entries", QString::number(status.getNMeasurementsEntries())));
  addItem(ForwarderStatusItem("Content Store Entries",    QString::number(status.getNCsEntries())));
  addItem(ForwarderStatusItem("Incoming Interests",  QString::number(status.getNInInterests())));
  addItem(ForwarderStatusItem("Outgoing Interests", QString::number(status.getNOutInterests())));
  addItem(ForwarderStatusItem("Incoming Data",      QString::number(status.getNInDatas())));
  addItem(ForwarderStatusItem("Outgoing Data",      QString::number(status.getNOutDatas())));
  addItem(ForwarderStatusItem("Incoming Nacks",      QString::number(status.getNInDatas())));
  addItem(ForwarderStatusItem("Outgoing Nacks",     QString::number(status.getNOutDatas())));
  endResetModel();
}

void
ForwarderStatusModel::onTimeout(const Interest& interest)
{
  std::cerr << "Request timed out (should not really happen)" << std::endl;
}

} // namespace ndn
