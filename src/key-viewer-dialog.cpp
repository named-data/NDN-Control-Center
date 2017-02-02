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

#include "key-viewer-dialog.hpp"
#include "key-viewer-dialog.moc"
#include "ui_key-viewer-dialog.h"
#include "key-tree-item.hpp"

#include <iostream>

#include <Qt>
#include <QMenu>

namespace ndn {
namespace ncc {

static int N_ROWS = 30;

KeyViewerDialog::KeyViewerDialog(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::KeyViewerDialog)
  , m_model(new KeyTreeModel)
  , m_keyChain(new KeyChain)
  , m_timer(parent)
{
  ui->setupUi(this);

  // present();

  connect(ui->treeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(displayCert(const QModelIndex&)));

  m_timer.setInterval(600000);
  connect(&m_timer, SIGNAL(timeout()), this, SLOT(display()));
  m_timer.start();

  this->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(ui->treeView, SIGNAL(pressed(const QModelIndex&)),
          this, SLOT(getSelection(const QModelIndex&)));
  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
          this, SLOT(showContextMenu(const QPoint&)));
}

KeyViewerDialog::~KeyViewerDialog()
{
}

void
KeyViewerDialog::updateModel()
{
}

void
KeyViewerDialog::display()
{
  m_keyChain = make_shared<KeyChain>();
  auto newModel = make_shared<KeyTreeModel>();

  std::vector<Name> defaultIdentities;
  m_keyChain->getAllIdentities(defaultIdentities, true);
  for (const auto& identity : defaultIdentities) {
    auto idItem = createIdentityNode(identity);
    idItem->setDefault(true);
    newModel->addChild(idItem);
  }

  std::vector<Name> otherIdentities;
  m_keyChain->getAllIdentities(otherIdentities, false);
  for (const auto& identity : otherIdentities) {
    newModel->addChild(createIdentityNode(identity));
  }

  ui->treeView->setModel(newModel.get());
  ui->treeView->show();
  m_model = newModel;

  auto newTableModel = make_shared<CertTreeModel>(N_ROWS, 2);
  newTableModel->setHeaderData(0, Qt::Horizontal,"Field");
  newTableModel->setHeaderData(1, Qt::Horizontal,"Value");
  ui->certView->setModel(newTableModel.get());
  ui->certView->show();
  m_tableModel = newTableModel;
}

void
KeyViewerDialog::present()
{
  display();

  show();
  raise();
}

void
KeyViewerDialog::getSelection(const QModelIndex& index)
{
  m_selectedIndex = index;
}

void
KeyViewerDialog::showContextMenu(const QPoint& point)
{
  QMenu contextMenu(tr("Key Management"), this);
  QAction action1("Set Default", this);
  connect(&action1, SIGNAL(triggered()), this, SLOT(setDefault()));
  contextMenu.addAction(&action1);
  contextMenu.exec(ui->treeView->mapToGlobal(point));
}

void
KeyViewerDialog::setDefault()
{
  std::cerr << "default" << std::endl;
  KeyTreeItem* item = static_cast<KeyTreeItem*>(m_selectedIndex.internalPointer());
  if (item == nullptr)
    return;

  Name name(item->name().toString().toStdString());
  std::cerr << name << std::endl;

  switch (item->type()) {
  case KeyTreeItem::Type::ID:
    {
      m_keyChain->setDefaultIdentity(name);
      break;
    }
  case KeyTreeItem::Type::KEY:
    {
      m_keyChain->setDefaultKeyNameForIdentity(name);
      break;
    }
  case KeyTreeItem::Type::CERT:
    {
      m_keyChain->setDefaultCertificateNameForKey(name);
      break;
    }
  default:
    return;
  }
  present();
}

void
KeyViewerDialog::displayCert(const QModelIndex& index)
{
  std::cerr << "clicked" << std::endl;

  QModelIndex parent = index;
  KeyTreeItem* item = static_cast<KeyTreeItem*>(parent.internalPointer());
  if (item->type() == KeyTreeItem::Type::ID) {
    std::cerr << item->name().toString().toStdString() << std::endl;
    parent = m_model->index(0, 0, parent);
  }

  item = static_cast<KeyTreeItem*>(parent.internalPointer());
  if (item->type() == KeyTreeItem::Type::KEY) {
    std::cerr << item->name().toString().toStdString() << std::endl;
    parent = m_model->index(0, 0, parent);
  }

  shared_ptr<CertTreeModel> tableModel = make_shared<CertTreeModel>(N_ROWS, 2);
  tableModel->setHeaderData(0, Qt::Horizontal,"Field");
  tableModel->setHeaderData(1, Qt::Horizontal,"Value");

  item = static_cast<KeyTreeItem*>(parent.internalPointer());
  if (item->type() == KeyTreeItem::Type::CERT) {
    std::cerr << item->name().toString().toStdString() << std::endl;

    shared_ptr<IdentityCertificate> cert =
      m_keyChain->getCertificate(Name(item->name().toString().toStdString()));

    tableModel->setItem(0, 0, new QStandardItem(QString("Name")));
    tableModel->setItem(0, 1, new QStandardItem(item->name().toString()));
    // tableModel->setData(tableModel->index(0, 0), Qt::red, Qt::BackgroundRole);

    tableModel->setItem(1, 0, new QStandardItem(QString("NotBefore")));
    tableModel->setItem(1, 1, new QStandardItem(QString::fromStdString(time::toIsoString(cert->getNotBefore()))));

    tableModel->setItem(2, 0, new QStandardItem(QString("NotAfter")));
    tableModel->setItem(2, 1, new QStandardItem(QString::fromStdString(time::toIsoString(cert->getNotAfter()))));

    tableModel->setItem(3, 0, new QStandardItem(QString("FingerPrint")));
    tableModel->setItem(3, 1, new QStandardItem(QString("FFFF")));

    Name signerName = cert->getSignature().getKeyLocator().getName();
    tableModel->setItem(4, 0, new QStandardItem(QString("KeyLocator")));
    tableModel->setItem(4, 1, new QStandardItem(QString::fromStdString(signerName.toUri())));
  }
  else {
    tableModel->setItem(0, 0, new QStandardItem(QString("")));
    tableModel->setItem(0, 1, new QStandardItem(QString("")));
  }
  ui->certView->setModel(tableModel.get());
  ui->certView->show();
  m_tableModel = tableModel;
}

KeyTreeItem*
KeyViewerDialog::createIdentityNode(const Name& identity)
{
  KeyTreeItem* idItem = new KeyTreeItem(QVariant(QString::fromStdString(identity.toUri())),
                                        QVariant(QString::fromStdString(identity.toUri())),
                                        KeyTreeItem::Type::ID);

  std::vector<Name> defaultKeys;
  m_keyChain->getAllKeyNamesOfIdentity(identity, defaultKeys, true);
  for (const auto& keyName : defaultKeys) {
    auto keyItem = createKeyNode(keyName, idItem);
    keyItem->setDefault(true);
    idItem->appendChild(keyItem);
  }

  std::vector<ndn::Name> otherKeys;
  m_keyChain->getAllKeyNamesOfIdentity(identity, otherKeys, false);
  for (const auto& keyName : otherKeys) {
    idItem->appendChild(createKeyNode(keyName, idItem));
  }

  return idItem;
}

KeyTreeItem*
KeyViewerDialog::createKeyNode(const Name& keyName, KeyTreeItem* idItem)
{
  KeyTreeItem* keyItem = new KeyTreeItem(QVariant(QString::fromStdString(keyName.toUri())),
                                         QVariant(QString::fromStdString(keyName[-1].toUri())),
                                         KeyTreeItem::Type::KEY,
                                         idItem);

  std::vector<Name> defaultCertificates;
  m_keyChain->getAllCertificateNamesOfKey(keyName, defaultCertificates, true);
  for (const auto& certName : defaultCertificates) {
    auto certItem = new KeyTreeItem(QVariant(QString::fromStdString(certName.toUri())),
                                    QVariant(QString::fromStdString(certName[-1].toUri())),
                                    KeyTreeItem::Type::CERT,
                                    keyItem);
    certItem->setDefault(true);
    keyItem->appendChild(certItem);
  }

  std::vector<ndn::Name> otherCertificates;
  m_keyChain->getAllCertificateNamesOfKey(keyName, otherCertificates, false);
  for (const auto& certName : otherCertificates) {
    keyItem->appendChild(new KeyTreeItem(QVariant(QString::fromStdString(certName.toUri())),
                                         QVariant(QString::fromStdString(certName[-1].toUri())),
                                         KeyTreeItem::Type::CERT,
                                         keyItem));
  }

  return keyItem;
}

} // namespace ncc
} // namespace ndn
