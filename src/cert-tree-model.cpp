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

#include "cert-tree-model.hpp"

#ifdef WAF
#include "cert-tree-model.moc"
#endif

namespace ndn {
namespace ncc {

CertTreeModel::CertTreeModel(int row, int column, QObject* parent)
  : QStandardItemModel(row, column, parent)
{
}

QVariant
CertTreeModel::data(const QModelIndex& index, int role) const
{
  switch (role) {
  case Qt::FontRole:
    {
      QFont font;
      if (index.column() == 0) {
        font.setBold(true);
      }
      return font;
    }
  case Qt::BackgroundRole:
    {
      QBrush brush(Qt::white, Qt::SolidPattern);
      if (index.row() % 2 == 0) {
        brush.setColor(Qt::lightGray);
      }
      return brush;
    }
  default:
    return QStandardItemModel::data(index, role);
  }
}

} // namespace ncc
} // namespace ndn
