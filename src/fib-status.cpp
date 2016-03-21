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

#include "fib-status.hpp"

#ifdef WAF
#include "fib-status.moc"
// #include "fib-status.cpp.moc"
#endif

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/name.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/encoding/buffer-stream.hpp>
#include <ndn-cxx/management/nfd-fib-entry.hpp>
#include <ndn-cxx/management/nfd-face-status.hpp>
#include <ndn-cxx/management/nfd-forwarder-status.hpp>

namespace ndn {

FibStatusModel::FibStatusModel(Face& face, QObject *parent/* = 0*/)
  : QAbstractListModel(parent)
  // , m_face(face)
{
}

int
FibStatusModel::rowCount(const QModelIndex &parent/* = QModelIndex()*/) const
{
  return m_items.count();
}

void
FibStatusModel::addItem(const FibStatusItem &item)
{
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_items << item;
  endInsertRows();
}

QVariant
FibStatusModel::data(const QModelIndex & index, int role) const
{
  if (index.row() < 0 || index.row() >= m_items.count()) {
    return QVariant();
  }

  const FibStatusItem &item = m_items.at(index.row());
  if (role == PrefixRole) {
    return item.prefix();
  } else if (role == FaceIdRole) {
    return static_cast<uint>(item.faceId());
  } else if (role == CostRole) {
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

Q_INVOKABLE void
FibStatusModel::fetchFibInformation()
{
  // m_buffer = make_shared<OBufferStream>();

  // Interest interest("/localhost/nfd/fib/list");
  // interest.setChildSelector(1);
  // interest.setMustBeFresh(true);
  // m_face.expressInterest(interest,
  //                        bind(&FibStatusModel::fetchSegments, this, _2,
  //                             &FibStatusModel::afterFetchedFibEnumerationInformation),
  //                        bind(&FibStatusModel::onTimeout, this));
  // try {
  //   m_face.processEvents();
  // } catch (Tlv::Error e) {
  //   std::cerr << e.what() << std::endl;
  //   clear();
  // }
}

void
FibStatusModel::afterFetchedFibEnumerationInformation()
{
//   beginResetModel();
//   m_items.clear();
//   ConstBufferPtr buf = m_buffer->buf();

//   Block block;
//   size_t offset = 0;
//   while (offset < buf->size()) {
//     bool ok = Block::fromBuffer(buf, offset, block);
//     if (!ok) {
//       std::cerr << "ERROR: cannot decode FibEntry TLV" << std::endl;
//       break;
//     }
//     offset += block.size();

//     nfd::FibEntry fibEntry(block);

//     for (std::list<nfd::NextHopRecord>::const_iterator
//            nextHop = fibEntry.getNextHopRecords().begin();
//          nextHop != fibEntry.getNextHopRecords().end();
//          ++nextHop) {
//       addItem(FibStatusItem(QString::fromStdString(fibEntry.getPrefix().toUri()),
//                             nextHop->getFaceId(), nextHop->getCost()));
//     }
//   }
//   endResetModel();
}


void
FibStatusModel::fetchSegments(const Data& data, void (FibStatusModel::*onDone)())
{
  // m_buffer->write(reinterpret_cast<const char*>(data.getContent().value()),
  //                 data.getContent().value_size());

  // uint64_t currentSegment = data.getName().get(-1).toSegment();

  // const name::Component& finalBlockId = data.getMetaInfo().getFinalBlockId();
  // if (finalBlockId.empty() ||
  //     finalBlockId.toSegment() > currentSegment) {
  //   m_face.expressInterest(data.getName().getPrefix(-1).appendSegment(currentSegment+1),
  //                          bind(&FibStatusModel::fetchSegments, this, _2, onDone),
  //                          bind(&FibStatusModel::onTimeout, this));
  // } else {
  //   return (this->*onDone)();
  // }
}

void
FibStatusModel::onTimeout()
{
  std::cerr << "Request timed out" << std::endl;
}

} // namespace ndn
