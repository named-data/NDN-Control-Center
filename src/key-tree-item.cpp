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


#include <QStringList>
#include "key-tree-item.hpp"

namespace ndn {
namespace ncc {

KeyTreeItem::KeyTreeItem(const QVariant& name, const QVariant& data, Type type, KeyTreeItem* parent)
  : m_name(name)
  , m_data(data)
  , m_type(type)
  , m_parentItem(parent)
{
}

KeyTreeItem::~KeyTreeItem()
{
  qDeleteAll(m_childItems);
}

void
KeyTreeItem::setParent(KeyTreeItem* parent)
{
  m_parentItem = parent;
}

void
KeyTreeItem::appendChild(KeyTreeItem* item)
{
  m_childItems.append(item);
}

KeyTreeItem*
KeyTreeItem::child(int row)
{
  return m_childItems.value(row);
}

int
KeyTreeItem::childCount() const
{
  return m_childItems.count();
}

int
KeyTreeItem::columnCount() const
{
  return 1;
}

QVariant
KeyTreeItem::name() const
{
  return m_name;
}

QVariant
KeyTreeItem::data() const
{
  return m_data;
}

KeyTreeItem::Type
KeyTreeItem::type() const
{
  return m_type;
}

KeyTreeItem*
KeyTreeItem::parentItem()
{
  return m_parentItem;
}

int
KeyTreeItem::row() const
{
  if (m_parentItem)
    return m_parentItem->m_childItems.indexOf(const_cast<KeyTreeItem*>(this));

  return 0;
}

} // namespace ndn
} // namespace ncc
