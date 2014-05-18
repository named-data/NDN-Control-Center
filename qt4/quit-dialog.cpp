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
 *
 * \author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "quit-dialog.hpp"

#include <QVBoxLayout>
#include <QApplication>

QuitDialog::QuitDialog(QWidget *parent)
  : QDialog(parent)
{
  question = new QLabel(tr("Shutdown NDN daemon as well?"));
  information = new QLabel(tr("All NDN operations will become unavailable."));

  confirmButton = new QPushButton(tr("Yes"));
  noButton = new QPushButton(tr("No"));
  cancelButton = new QPushButton(tr("Cancel"));
  cancelButton->setDefault(true);

  buttonBox = new QDialogButtonBox(Qt::Horizontal);
  buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);
  buttonBox->addButton(noButton, QDialogButtonBox::ActionRole);
  buttonBox->addButton(confirmButton, QDialogButtonBox::ActionRole);

  connect(confirmButton,SIGNAL(pressed()), parent, SLOT(terminateDaemonAndClose()));
  connect(noButton,SIGNAL(pressed()), qApp, SLOT(quit()));
  connect(cancelButton,SIGNAL(pressed()), this, SLOT(hide()));

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(question);
  layout->addWidget(information);
  layout->addWidget(buttonBox);

  setLayout(layout);

  setWindowTitle(tr("NDNx Control Center"));
}

#if WAF
#include "quit-dialog.moc"
#include "quit-dialog.cpp.moc"
#endif
