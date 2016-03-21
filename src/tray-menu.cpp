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
#define CONNECT_ICON ":/res/icon-connected-black.png"
#define DISCONNECT_ICON ":/res/icon-disconnected-black.png"

#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>
#else
#define CONNECT_ICON ":/res/icon-connected-white.png"
#define DISCONNECT_ICON ":/res/icon-disconnected-white.png"
#endif // OSX_BUILD

#ifdef WAF
#include "tray-menu.moc"
// #include "tray-menu.cpp.moc"
#endif

namespace ndn {

TrayMenu::TrayMenu(QQmlContext* context)
  : m_context(context)
  , m_isNfdRunning(false)
  , m_menu(new QMenu(this))
  , m_entryPref(new QAction("Preferences...", m_menu))
  , m_entrySec(new QAction("Security...", m_menu))
#ifdef OSX_BUILD
  , m_entryEnableCli(new QAction("Enable Command Terminal Usage...", m_menu))
#endif
  , m_entryQuit(new QAction("Quit", m_menu))
  , m_keyViewerDialog(new ncc::KeyViewerDialog)

{
  connect(m_entryPref, SIGNAL(triggered()), this, SIGNAL(showApp()));
  connect(m_entrySec, SIGNAL(triggered()), m_keyViewerDialog, SLOT(present()));
  connect(m_entryQuit, SIGNAL(triggered()), this, SLOT(quitApp()));

#ifdef OSX_BUILD
  connect(m_entryEnableCli, SIGNAL(triggered()), this, SLOT(enableCli()));
#endif

  connect(this, SIGNAL(nfdActivityUpdate(bool)), this, SLOT(updateNfdActivityIcon(bool)),
          Qt::QueuedConnection);

  m_context->setContextProperty("startStopButtonText", QVariant::fromValue(QString("Start NFD")));

  // m_menu->addAction(start);
  // m_menu->addAction(stop);
  m_menu->addAction(m_entryPref);
  m_menu->addAction(m_entrySec);
#ifdef OSX_BUILD
  m_menu->addAction(m_entryEnableCli);
#endif
  m_menu->addAction(m_entryQuit);
  m_tray = new QSystemTrayIcon(this);
  m_tray->setContextMenu(m_menu);
  connect(m_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
  m_tray->setIcon(QIcon(DISCONNECT_ICON));
  m_tray->show();
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
  // proc->start(NFD_AUTOCONFIG_COMMAND);
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
//   QProcess * proc = new QProcess();
//   connect(proc,SIGNAL(finished(int)), proc, SLOT(deleteLater()));
// #ifdef __linux__
//   proc->start("gksudo", QStringList() << NFD_START_COMMAND);
// #else
//   proc->start("osascript", QStringList()
//               << "-e"
//               << "do shell script \"" NFD_START_COMMAND "\" with administrator privileges");
// #endif
}

void
TrayMenu::stopNfd()
{
//   QProcess * proc = new QProcess();
//   connect(proc,SIGNAL(finished(int)), proc, SLOT(deleteLater()));
// #ifdef __linux__
//   proc->start("gksudo", QStringList() << NFD_STOP_COMMAND);
// #else
//   proc->start("osascript", QStringList()
//               << "-e"
//               << "do shell script \"" NFD_STOP_COMMAND "\" with administrator privileges");
// #endif
}

Q_INVOKABLE void
TrayMenu::addDeleteRoute()
{
  addRoute();
}

Q_INVOKABLE void
TrayMenu::addRoute()
{
  std::cout << "Adding route" << std::endl;
  QString cmd = "nfdc register /test tcp4://localhost";
  QProcess *addNewRoute = new QProcess();
  connect(addNewRoute,SIGNAL(finished(int)), addNewRoute, SLOT(deleteLater()));
  addNewRoute->start("bash", QStringList() << "-c" << cmd);
  std::cout << "Done" << std::endl;


//   QProcess * proc = new QProcess();
}

void
TrayMenu::deleteRoute()
{
  std::cout << "Deleting route" << std::endl;
  QString cmd = "nfdc unregister /test tcp4://localhost";
  QProcess *addNewRoute = new QProcess();
  connect(addNewRoute,SIGNAL(finished(int)), addNewRoute, SLOT(deleteLater()));
  addNewRoute->start("bash", QStringList() << "-c" << cmd);
  std::cout << "Done" << std::endl;

}


void
TrayMenu::updateNfdActivityIcon(bool isActive)
{
  m_isNfdRunning = isActive;

  if (isActive) {
    m_tray->setIcon(QIcon(CONNECT_ICON));
    m_context->setContextProperty("startStopButtonText", QVariant::fromValue(QString("Stop NFD")));
  }
  else {
    m_tray->setIcon(QIcon(DISCONNECT_ICON));
    m_context->setContextProperty("startStopButtonText", QVariant::fromValue(QString("Start NFD")));
  }
}

void
TrayMenu::enableCli()
{
#ifdef OSX_BUILD
  AuthorizationRef authorizationRef;
  OSStatus status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment,
                                        kAuthorizationFlagDefaults, &authorizationRef);
  if (status != errAuthorizationSuccess)
    return;

  AuthorizationItem item = { kAuthorizationRightExecute, 0, 0, 0 };
  AuthorizationRights rights = { 1, &item };
  const AuthorizationFlags flags = kAuthorizationFlagDefaults | kAuthorizationFlagInteractionAllowed
    | kAuthorizationFlagPreAuthorize | kAuthorizationFlagExtendRights;

  status = AuthorizationCopyRights(authorizationRef, &rights, kAuthorizationEmptyEnvironment,
                                   flags, 0);
  if (status != errAuthorizationSuccess)
    return;

  char const* mkdir_arg[] = { "-p", "/usr/local/bin", nullptr };
  char const* mkdir = "/bin/mkdir";
  AuthorizationExecuteWithPrivileges(authorizationRef,
                                     mkdir,
                                     kAuthorizationFlagDefaults, (char**)mkdir_arg, nullptr);

  std::vector<std::string> arguments = { "-f",
                                       QCoreApplication::applicationDirPath().toStdString() + "/../Resources/ndn",
                                       "/usr/local/bin/ndn" };
  std::vector<const char*> args;
  for (const auto& i : arguments) {
    args.push_back(i.c_str());
  }
  args.push_back(nullptr);

  char const* helperTool  = "/bin/cp";
  AuthorizationExecuteWithPrivileges(authorizationRef,
                                     helperTool,
                                     kAuthorizationFlagDefaults,
                                     (char**)args.data(), NULL);

  // QVector<char *> args;
  // QVector<QByteArray> utf8Args;
  // for (const QString &argument : arguments) {
  //   utf8Args.push_back(argument.toUtf8());
  //   args.push_back(utf8Args.last().data());
  // }
  // args.push_back(0);

  // const QByteArray utf8Program = program.toUtf8();
  // status = AuthorizationExecuteWithPrivileges(authorizationRef, utf8Program.data(),
  //                                             kAuthorizationFlagDefaults, args.data(), 0);

  AuthorizationFree(authorizationRef, kAuthorizationFlagDestroyRights);
#endif
}

} // namespace ndn
