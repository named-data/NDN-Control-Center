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

#ifndef NDN_NCC_KEY_VIEWER_DIALOG_HPP
#define NDN_NCC_KEY_VIEWER_DIALOG_HPP

#include "key-tree-model.hpp"
#include <ndn-cxx/security/key-chain.hpp>

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class KeyViewerDialog;
}

namespace ndn {
namespace ncc {

class KeyViewerDialog : public QDialog
{
  Q_OBJECT

public:
  KeyViewerDialog(QWidget *parent = 0);

  virtual
  ~KeyViewerDialog();

  void
  updateModel();

private:
  KeyTreeItem*
  createIdentityNode(const Name& identity);

  KeyTreeItem*
  createKeyNode(const Name& keyName, KeyTreeItem* idItem);

signals:
  void
  clicked(const QModelIndex &index);

public slots:
  void
  present();

private slots:
  void
  displayCert(const QModelIndex& index);
  // void insertChild();
  // bool insertColumn();
  // void insertRow();
  // bool removeColumn();
  // void removeRow();

private:
  Ui::KeyViewerDialog* ui;

  shared_ptr<KeyTreeModel> m_model;
  shared_ptr<KeyChain> m_keyChain;

  shared_ptr<QStandardItemModel> m_tableModel;
};

} // namespace ncc
} // namespace ndn

#endif // NDN_NCC_KEY_VIEWER_DIALOG_HPP
