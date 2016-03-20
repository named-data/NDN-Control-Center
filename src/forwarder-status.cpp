/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2013-2014, Regents of the University of California,
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

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/name.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/management/nfd-fib-entry.hpp>
#include <ndn-cxx/management/nfd-face-status.hpp>
#include <ndn-cxx/management/nfd-forwarder-status.hpp>

#ifdef WAF
#include "forwarder-status.moc"
// #include "forwarder-status.cpp.moc"
#endif

namespace ndn {

ForwarderStatusModel::ForwarderStatusModel(QObject* parent/* = 0*/)
  : QAbstractListModel(parent)
{
  connect(this, SIGNAL(onDataReceived(ndn::shared_ptr<const ndn::Data>)), this,
          SLOT(updateStatus(ndn::shared_ptr<const ndn::Data>)),
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
ForwarderStatusModel::afterFetchedVersionInformation(const Data& data)
{
  emit onDataReceived(data.shared_from_this());
}

void
ForwarderStatusModel::updateStatus(shared_ptr<const Data> data)
{
  beginResetModel();
  m_items.clear();
  nfd::ForwarderStatus status(data->getContent());
  addItem(ForwarderStatusItem("version",       QString::number(status.getNfdVersion())));
  addItem(ForwarderStatusItem("startTime",     QString::fromStdString(time::toIsoString(status.getStartTimestamp()))));
  addItem(ForwarderStatusItem("currentTime",   QString::fromStdString(time::toIsoString(status.getCurrentTimestamp()))));
  addItem(ForwarderStatusItem("nNameTreeEntries", QString::number(status.getNNameTreeEntries())));
  addItem(ForwarderStatusItem("nFibEntries",   QString::number(status.getNFibEntries())));
  addItem(ForwarderStatusItem("nPitEntries",   QString::number(status.getNPitEntries())));
  addItem(ForwarderStatusItem("nMeasurementsEntries", QString::number(status.getNMeasurementsEntries())));
  addItem(ForwarderStatusItem("nCsEntries",    QString::number(status.getNCsEntries())));
  addItem(ForwarderStatusItem("nInInterests",  QString::number(status.getNInInterests())));
  addItem(ForwarderStatusItem("nOutInterests", QString::number(status.getNOutInterests())));
  addItem(ForwarderStatusItem("nInDatas",      QString::number(status.getNInDatas())));
  addItem(ForwarderStatusItem("nOutDatas",     QString::number(status.getNOutDatas())));
  endResetModel();
}

void
ForwarderStatusModel::onTimeout(const Interest& interest)
{
  std::cerr << "Request timed out (should not really happen)" << std::endl;
}

} // namespace ndn
