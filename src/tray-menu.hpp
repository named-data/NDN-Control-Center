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

#ifndef NCC_TRAY_MENU_HPP
#define NCC_TRAY_MENU_HPP

#include "config.hpp"

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>

#include <QtWidgets/QWidget>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

#include <QtQml/QQmlContext>

#include "key-viewer-dialog.hpp"

namespace ndn {

class Face;

class TrayMenu : public QWidget
{
  Q_OBJECT

signals:
  void
  showApp();

  void
  nfdActivityUpdate(bool isActive);

public:
  explicit
  TrayMenu(QQmlContext* context, Face& face);

  ~TrayMenu();

  Q_INVOKABLE bool
  isAutoConfigEnabled();

  Q_INVOKABLE void
  startStopAutoConfig(bool autoConfig);

  Q_INVOKABLE void
  startStopNfd(bool autoConfig);

  Q_INVOKABLE void
  addDeleteRoute();

  Q_INVOKABLE void
  addRoute();

  Q_INVOKABLE void
  deleteRoute();

private slots:

  void
  quitApp();

  void
  iconActivated(QSystemTrayIcon::ActivationReason reason);

  void
  startAutoConfig();

  void
  stopAutoConfig();

  void
  startNfd();

  void
  stopNfd();

  void
  updateNfdActivityIcon(bool isActive);

  void
  enableCli();

  void
  processOutput();

  void
  showPref();

  static void
  appendMsg(QString &target, QString source);

private:
  QQmlContext* m_context;
  bool m_isNfdRunning;
  QSystemTrayIcon* m_tray;
  QMenu* m_menu;
  QAction* m_entryPref;
  QAction* m_entrySec;
  QProcess* m_acProc;
  QString m_autoConfigMsg;
  QSettings m_settings;
#ifdef OSX_BUILD
  QAction* m_entryEnableCli;
#endif

  QAction* m_entryQuit;

  ncc::KeyViewerDialog* m_keyViewerDialog;
  Face& m_face;
};

} // namespace ndn

#endif // NCC_TRAY_MENU_HPP
