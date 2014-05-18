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

#ifndef NCC_QT_TRAY_MENU_H
#define NCC_QT_TRAY_MENU_H

#include <QMainWindow>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer>
#include <QProcess>
#include <QThread>
#include <QStandardItemModel>
#include <QtXml>
#include <QThread>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

// #include "fib-input-dialog.hpp"
#include "quit-dialog.hpp"
#include "network-manager.hpp"

#define ALLOW_SOFTWARE_UPDATES "AllowAutomaticUpdates"
#define ENABLE_HUB_DISCOVERY "EnableHubDiscovery"
#define ENABLE_START_ON_LOGIN "enableStartOnLogin"
#define SHUTDOWN_ON_EXIT "ShutdownOnExit"

#define AUTOSTART_DIRECTORY "/.config/autostart/"
#define SHORTCUT_FILE "ndnxcontrolcenter.desktop"
#define STATUS_XSLT_FILE "status.xslt"
#define FIB_XSLT_FILE "status-to-fib.xslt"

namespace Ui {
class TrayMenu;
}

class TrayMenu : public QMainWindow
{
  Q_OBJECT

public:
  explicit TrayMenu(QWidget *parent = 0);
  ~TrayMenu();

private:
  Ui::TrayMenu *ui;

  void createTrayIcon();
  void setIcon(bool isConnected);
  void createToolbar();
  void createTableView();
  void closeEvent(QCloseEvent *); // Overriding the window's close event
  void showEvent(QShowEvent * event); //Overriding the window's show event

  void loadSettings();
  void makeAutostartDirectory();

  void resizeEvent (QResizeEvent * event);

  bool daemonStarted;

  QSettings *persistentSettings;

  QSystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;

  QTimer *daemonStatusTimer;
  QThread *statusUpdateThread;

  QProcess *processLookup;
  QProcess *applyStatusXslt;
  QProcess *applyFibXslt;

  QString statusXml;
  QString fibContentsXml;

  QStandardItemModel *model;
  int selectedRow;
  int scrollPosition;

  QAction *statusIndicator;
  QAction *displayStatus;
  QAction *interestSentRecv;
  QAction *dataSentRecv;
  QAction *open;
  QAction *close;
  QAction *openGeneralSettings;
  QAction *openForwardingSettings;
  QAction *openSecuritySettings;

  // FibInputDialog *dialog;

  bool enableHubDiscovery;
  bool enableStartOnLogin;

  NetworkManager *networkManager;
  QNetworkAccessManager *urlManager;

private slots:
  void trayIconClicked(QSystemTrayIcon::ActivationReason);
  void generalSettingsClicked();
  void forwardingSettingsClicked();
  void securitySettingsClicked();
  void openTrafficMap();
  void openRoutingStatus();
  void openCertificationPage();
  void displayPopup();
  // void confirmQuit();
  void showFibInputDialog();
  // void terminateDaemonAndClose();
  void addFibEntry();
  // void daemonStatusUpdate();
  void selectTableRow();
  // void deleteFibEntry();
  void changeHubDiscovery();
  void changeLoginStart();
  void copyFile();
  // void runXmlProc(QNetworkReply *reply);
  // void parseStatusXml();
  // void parseFibXml();
  void parseProcessLookup();
};

#endif // NCC_QT_TRAY_MENU_H
