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

#include "config.hpp"
#include "tray-menu.hpp"

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>


#ifdef OSX_BUILD
#define CONNECT_ICON ":/resources/icon-connected-black.png"
#define DISCONNECT_ICON ":/resources/icon-disconnected-black.png"
#else
#define CONNECT_ICON ":/resources/icon-connected-white.png"
#define DISCONNECT_ICON ":/resources/icon-disconnected-white.png"
#endif

#ifdef WAF
#include "tray-menu.moc"
// #include "tray-menu.cpp.moc"
#endif

namespace ndn {

TrayMenu::TrayMenu(QQmlContext* context)
  : m_context(context)
  , m_isNfdRunning(false)
{
  menu = new QMenu(this);
  pref = new QAction("Preferences...", menu);
  quit = new QAction("Quit", menu);

  connect(pref, SIGNAL(triggered()), this, SIGNAL(showApp()));
  connect(quit, SIGNAL(triggered()), this, SLOT(quitApp()));

  connect(this, SIGNAL(nfdActivityUpdate(bool)), this, SLOT(updateNfdActivityIcon(bool)),
          Qt::QueuedConnection);

  m_context->setContextProperty("startStopButtonText", QVariant::fromValue(QString("Start NFD")));

  // menu->addAction(start);
  // menu->addAction(stop);
  menu->addAction(pref);
  menu->addAction(quit);
  tray = new QSystemTrayIcon(this);
  tray->setContextMenu(menu);
  connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
  tray->setIcon(QIcon(DISCONNECT_ICON));
  tray->show();
}

TrayMenu::~TrayMenu()
{
}

Q_INVOKABLE void
TrayMenu::autoConfig()
{
  std::cout << "auto config" << std::endl;
  QProcess* proc = new QProcess();
  connect(proc, SIGNAL(finished(int)), proc, SLOT(deleteLater()));
  proc->start(NFD_AUTOCONFIG_COMMAND);
}

void
TrayMenu::quitApp()
{
  QCoreApplication::exit(0);
}

void
TrayMenu::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch (reason) {
  // case QSystemTrayIcon::Trigger:
  //   emit showApp();
  //   break;
  case QSystemTrayIcon::Context:
    break;
  default:
    break;
  }
}

Q_INVOKABLE void
TrayMenu::startStopNfd()
{
  if (!m_isNfdRunning) {
    startNfd();
  }
  else {
    stopNfd();
  }
}

void
TrayMenu::startNfd()
{
  QProcess * proc = new QProcess();
  connect(proc,SIGNAL(finished(int)), proc, SLOT(deleteLater()));
#ifdef __linux__
  proc->start("gksudo", QStringList() << NFD_START_COMMAND);
#else
  proc->start("osascript", QStringList()
              << "-e"
              << "do shell script \"" NFD_START_COMMAND "\" with administrator privileges");
#endif
}

void
TrayMenu::stopNfd()
{
  QProcess * proc = new QProcess();
  connect(proc,SIGNAL(finished(int)), proc, SLOT(deleteLater()));
#ifdef __linux__
  proc->start("gksudo", QStringList() << NFD_STOP_COMMAND);
#else
  proc->start("osascript", QStringList()
              << "-e"
              << "do shell script \"" NFD_STOP_COMMAND "\" with administrator privileges");
#endif
}

void
TrayMenu::updateNfdActivityIcon(bool isActive)
{
  m_isNfdRunning = isActive;

  if (isActive) {
    tray->setIcon(QIcon(CONNECT_ICON));
    m_context->setContextProperty("startStopButtonText", QVariant::fromValue(QString("Stop NFD")));
  }
  else {
    tray->setIcon(QIcon(DISCONNECT_ICON));
    m_context->setContextProperty("startStopButtonText", QVariant::fromValue(QString("Start NFD")));
  }
}

} // namespace ndn
