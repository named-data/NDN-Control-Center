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

#include <QtCore/QDir>
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtCore/QXmlStreamWriter>

#include <QtWidgets/QWidget>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>

#include <QtQml/QQmlContext>

#include "key-viewer-dialog.hpp"
#include "status-viewer.hpp"

#ifdef OSX_BUILD
#include "osx-auto-update-sparkle.hpp"
#include "osx-adhoc.hpp"
#endif // OSX_BUILD

#include <thread>

namespace ndn {

class Face;

namespace ncc {

class TrayMenu : public QWidget
{
  Q_OBJECT

signals:
  void
  showApp();

  void
  nfdActivityUpdate(bool isActive);

  void
  connectivityUpdate(bool isConnectedToHub);

  void
  adhocUpdate(bool isConnectedToAdhoc);

public:
  explicit
  TrayMenu(QQmlContext* context, Face& face, KeyChain& keyChain);

  ~TrayMenu();

  Q_INVOKABLE bool
  isNccAutoStartEnabled() const;

  Q_INVOKABLE void
  enableDisableNccAutoStart(bool isEnabled);

  Q_INVOKABLE bool
  isNfdAutoStartEnabled() const;

  Q_INVOKABLE void
  enableDisableNfdAutoStart(bool isEnabled);

  Q_INVOKABLE bool
  isNdnAutoConfigEnabled() const;

  Q_INVOKABLE void
  enableDisableNdnAutoConfig(bool isEnabled);

  Q_INVOKABLE bool
  isNfdStopOnExitEnabled() const;

  Q_INVOKABLE void
  enableDisableNfdStopOnExit(bool isEnabled);

  Q_INVOKABLE void
  addDeleteRoute();

  Q_INVOKABLE void
  addRoute();

  Q_INVOKABLE void
  deleteRoute();

private slots:
  void
  startNdnAutoConfig();

  void
  stopNdnAutoConfig(bool appendStatus = true);

  void
  quitApp();

  void
  startNfd();

  void
  stopNfd();

  void
  updateNfdActivityIcon(bool isActive);

  void
  updateConnectivity(bool isConnectedToHub);

  void
  updateAdhoc(bool isConnectedToAdhoc);

  void
  enableCli();

  void
  processOutput();

  void
  appendNdnAutoConfigStatus(const QString& message);

#ifdef OSX_BUILD
  void
  onAdhocChange();

  void
  checkForUpdates();
#endif // OSX_BUILD

  void
  scheduleDelayedTask(std::chrono::milliseconds delays, const std::function<void()>& task);

private:
  QQmlContext* m_context;
  bool m_isNfdRunning;
  bool m_isConnectedToHub;
  bool m_isConnectedToAdhoc;
  QSystemTrayIcon* m_tray;
  QMenu* m_menu;
  QAction* m_entryPref;
  QAction* m_entryStatus;
  QAction* m_entrySec;
  QProcess* m_acProc;
  QSettings* m_settings;
  QString m_ndnAutoConfigMsg;
#ifdef OSX_BUILD
  QAction* m_entryEnableCli;
  QAction* m_entryAdhoc;
  QAction* m_checkForUpdates;
  OsxAutoUpdateSparkle m_sparkle;
#endif // OSX_BUILD

  QAction* m_entryQuit;

  ncc::KeyViewerDialog* m_keyViewerDialog;
  Face& m_face;
  KeyChain& m_keyChain;
  Adhoc m_adhoc;
  StatusViewer* m_statusViewer;
};

} // namespace ncc
} // namespace ndn

#endif // NCC_TRAY_MENU_HPP
