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

#ifndef NDN_NCC_CERT_TREE_MODEL_HPP
#define NDN_NCC_CERT_TREE_MODEL_HPP

#include <QStandardItemModel>

namespace ndn {
namespace ncc {

class CertTreeModel : public QStandardItemModel
{
  Q_OBJECT

public:
  explicit
  CertTreeModel(int row, int column, QObject* parent = 0);

  QVariant
  data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;
};

} // namespace ncc
} // namespace ndn

#endif // NDN_NCC_CERT_TREE_MODEL_HPP
