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

#ifndef NCC_TRAY_MENU_HPP
#define NCC_TRAY_MENU_HPP

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>

#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>

namespace ndn {

class TrayMenu : public QWidget
{
  Q_OBJECT

signals:
  void
  showApp();

  void
  nfdActivityUpdate(bool isActive);

public:
  TrayMenu();

  ~TrayMenu();

  Q_INVOKABLE void
  checkNfdRunning();

  Q_INVOKABLE void
  autoConfig();

private slots:

  void
  quitApp();

  void
  iconActivated(QSystemTrayIcon::ActivationReason reason);

  void
  startNfd();

  void
  stopNfd();

  void
  updateNfdActivityIcon(bool isActive);

private:
  QSystemTrayIcon *tray;
  QMenu* menu;
  QAction* pref;
  QAction* quit;
  // QAction* start;
  // QAction* stop;
};

} // namespace ndn

#endif // NCC_TRAY_MENU_HPP
