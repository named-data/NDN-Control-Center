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

#ifndef NDN_NCC_KEY_TREE_ITEM_HPP
#define NDN_NCC_KEY_TREE_ITEM_HPP

#include <string>
#include <QList>
#include <QVariant>

namespace ndn {
namespace ncc {

class KeyTreeItem
{
public:
  enum class Type {
    ROOT = 0,
    ID = 1,
    KEY = 2,
    CERT = 3
  };

public:
  explicit
  KeyTreeItem(const QVariant& name,
              const QVariant& data = QVariant(),
              Type type = Type::ROOT,
              KeyTreeItem* parent = nullptr);

  ~KeyTreeItem();

  void
  setDefault(bool isDefault);

  bool
  isDefault()
  {
    return m_isDefault;
  }

  void
  setParent(KeyTreeItem* parent);

  void
  appendChild(KeyTreeItem* child);

  KeyTreeItem*
  child(int row);

  int
  childCount() const;

  int
  columnCount() const;

  QVariant
  name() const;

  QVariant
  data() const;

  Type
  type() const;

  int
  row() const;

  KeyTreeItem*
  parentItem();

private:
  QList<KeyTreeItem*> m_childItems;
  QVariant m_name;
  QVariant m_data;
  Type m_type;
  bool m_isDefault;
  KeyTreeItem* m_parentItem;
};

} // namespace ndn
} // namespace ncc

#endif // NDN_NCC_KEY_TREE_ITEM_HPP
