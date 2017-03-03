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

#ifndef NCC_FACE_STATUS_HPP
#define NCC_FACE_STATUS_HPP

#include <QtCore/QAbstractListModel>
#include <QtCore/QStringList>

#include <ndn-cxx/mgmt/nfd/face-status.hpp>

namespace ndn {

class FaceStatusItem
{
public:
  FaceStatusItem(uint64_t faceId, const QString& remote, const QString& local, const QString& scope, const QString& persistency,
                 const QString& linkType, uint64_t inInterest, uint64_t outInterest, uint64_t inData, uint64_t outData,
                 uint64_t inByte, uint64_t outByte, uint64_t inNack, uint64_t outNack)
    : m_faceId(faceId)
    , m_remote(remote)
    , m_local(local)
    , m_scope(scope)
    , m_persistency(persistency)
    , m_linkType(linkType)
    , m_inInterest(inInterest)
    , m_outInterest(outInterest)
    , m_inData(inData)
    , m_outData(outData)
    , m_inByte(inByte)
    , m_outByte(outByte)
    , m_inNack(inNack)
    , m_outNack(outNack)
  {
  }

  uint64_t
  faceId() const
  {
    return m_faceId;
  }

  const QString&
  remote() const
  {
    return m_remote;
  }

  const QString&
  local() const
  {
    return m_local;
  }

  const QString&
  scope() const
  {
    return m_scope;
  }

  const QString&
  persistency() const
  {
    return m_persistency;
  }

  const QString&
  linkType() const
  {
    return m_linkType;
  }

  uint64_t
  inInterest() const
  {
    return m_inInterest;
  }

  uint64_t
  outInterest() const
  {
    return m_outInterest;
  }

  uint64_t
  inData() const
  {
    return m_inData;
  }

  uint64_t
  outData() const
  {
    return m_outData;
  }

  uint64_t
  inByte() const
  {
    return m_inByte;
  }

  uint64_t
  outByte() const
  {
    return m_outByte;
  }

  uint64_t
  inNack() const
  {
    return m_inNack;
  }

  uint64_t
  outNack() const
  {
    return m_outNack;
  }

private:
  uint64_t m_faceId;
  QString m_remote;
  QString m_local;
  QString m_scope;
  QString m_persistency;
  QString m_linkType;
  uint64_t m_inInterest;
  uint64_t m_outInterest;
  uint64_t m_inData;
  uint64_t m_outData;
  uint64_t m_inByte;
  uint64_t m_outByte;
  uint64_t m_inNack;
  uint64_t m_outNack;
};

class FaceStatusModel : public QAbstractListModel
{
  Q_OBJECT

signals:
  void
  onDataReceived(std::vector<ndn::nfd::FaceStatus> status);

public:
  enum FaceStatusRoles {
    FaceIdRole = Qt::UserRole + 1,
    RemoteRole,
    LocalRole,
    ScopeRole,
    PersistencyRole,
    LinkTypeRole,
    InInterestRole,
    OutInterestRole,
    InDataRole,
    OutDataRole,
    InByteRole,
    OutByteRole,
    InNackRole,
    OutNackRole
  };

  explicit
  FaceStatusModel(QObject* parent = 0);

  int
  rowCount(const QModelIndex& parent = QModelIndex()) const;

  void
  addItem(const FaceStatusItem& item);

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
  updateStatus(std::vector<ndn::nfd::FaceStatus> status);

private:
  QList<FaceStatusItem> m_items;
};

} // namespace ndn

#endif // NCC_FACE_STATUS_HPP
