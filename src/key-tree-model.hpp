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

#ifndef NDN_NCC_KEY_TREE_MODEL_HPP
#define NDN_NCC_KEY_TREE_MODEL_HPP

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

namespace ndn {
namespace ncc {

class KeyTreeItem;

class KeyTreeModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  explicit
  KeyTreeModel(QObject* parent = 0);

  ~KeyTreeModel();

  void
  clear();

  void
  addChild(KeyTreeItem* child);

  QVariant
  name(const QModelIndex& index, int role) const;

  QVariant
  data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;

  Qt::ItemFlags
  flags(const QModelIndex& index) const Q_DECL_OVERRIDE;

  QVariant
  headerData(int section, Qt::Orientation orientation,
             int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

  QModelIndex
  index(int row, int column, const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

  QModelIndex
  parent(const QModelIndex& index) const Q_DECL_OVERRIDE;

  int
  rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

  int
  columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

private:
  KeyTreeItem* m_rootItem;
};

} // namespace ncc
} // namespace ndn

#endif // NDN_NCC_KEY_TREE_MODEL_HPP
