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

#include "key-tree-model.hpp"
#include "key-tree-item.hpp"

#include <QStringList>
#include <QFont>
#include <QBrush>
#include <iostream>


#ifdef WAF
#include "key-tree-model.moc"
#endif

namespace ndn {
namespace ncc {

KeyTreeModel::KeyTreeModel(QObject *parent)
  : QAbstractItemModel(parent)
  , m_rootItem(new KeyTreeItem(QVariant("")))
{
}

KeyTreeModel::~KeyTreeModel()
{
  delete m_rootItem;
}

void
KeyTreeModel::clear()
{
  delete m_rootItem;
  m_rootItem = new KeyTreeItem(QVariant(""));
}

void
KeyTreeModel::addChild(KeyTreeItem* child)
{
  child->setParent(m_rootItem);
  m_rootItem->appendChild(child);
}

int
KeyTreeModel::columnCount(const QModelIndex& parent) const
{
  if (parent.isValid())
    return static_cast<KeyTreeItem*>(parent.internalPointer())->columnCount();
  else
    return m_rootItem->columnCount();
}

QVariant
KeyTreeModel::name(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  KeyTreeItem* item = static_cast<KeyTreeItem*>(index.internalPointer());

  return item->name();
}

QVariant
KeyTreeModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();


  KeyTreeItem* item = static_cast<KeyTreeItem*>(index.internalPointer());
  switch (role) {
  case Qt::DisplayRole:
    {
      return item->data();
    }
  case Qt::FontRole:
    {
      QFont font;
      if (item->isDefault()) {
        font.setBold(true);
      }
      return font;
    }
  // case Qt::BackgroundRole:
  //   {
  //     std::cerr << "brush" << std::endl;
  //     QBrush brush(Qt::white, Qt::SolidPattern);
  //     if (index.row() % 2 == 0) {
  //       brush.setColor(Qt::cyan);
  //     }
  //     return brush;
  //   }
  default:
    return QVariant();
  }
}

Qt::ItemFlags
KeyTreeModel::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return 0;

  return QAbstractItemModel::flags(index);
}

QVariant
KeyTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  // if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
  //   return m_rootItem->data();

  return QVariant();
}

QModelIndex
KeyTreeModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  KeyTreeItem *parentItem;

  if (!parent.isValid())
    parentItem = m_rootItem;
  else
    parentItem = static_cast<KeyTreeItem*>(parent.internalPointer());

  KeyTreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex
KeyTreeModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  KeyTreeItem *childItem = static_cast<KeyTreeItem*>(index.internalPointer());
  KeyTreeItem *parentItem = childItem->parentItem();

  if (parentItem == m_rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int
KeyTreeModel::rowCount(const QModelIndex &parent) const
{
  KeyTreeItem *parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = m_rootItem;
  else
    parentItem = static_cast<KeyTreeItem*>(parent.internalPointer());

  return parentItem->childCount();
}

// void
// KeyTreeModel::setupModelData(const QStringList &lines, KeyTreeItem *parent)
// {
  // QList<KeyTreeItem*> parents;
  // QList<int> indentations;
  // parents << parent;
  // indentations << 0;

  // int number = 0;

  // while (number < lines.count()) {
  //   int position = 0;
  //   while (position < lines[number].length()) {
  //     if (lines[number].at(position) != ' ')
  //       break;
  //     position++;
  //   }

  //   QString lineData = lines[number].mid(position).trimmed();

  //   if (!lineData.isEmpty()) {
  //     // Read the column data from the rest of the line.
  //     QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
  //     QList<QVariant> columnData;
  //     for (int column = 0; column < columnStrings.count(); ++column)
  //       columnData << columnStrings[column];

  //     if (position > indentations.last()) {
  //       // The last child of the current parent is now the new parent
  //       // unless the current parent has no children.

  //       if (parents.last()->childCount() > 0) {
  //         parents << parents.last()->child(parents.last()->childCount()-1);
  //         indentations << position;
  //       }
  //     } else {
  //       while (position < indentations.last() && parents.count() > 0) {
  //         parents.pop_back();
  //         indentations.pop_back();
  //       }
  //     }

  //     // Append a new item to the current parent's list of children.
  //     parents.last()->appendChild(new TreeItem(columnData, parents.last()));
  //   }

  //   ++number;
  // }
// }

} // namespace ndn
} // namespace ncc
