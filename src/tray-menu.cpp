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

#include "tray-menu.hpp"
#include "tray-menu.moc"

#include <chrono>
#include <functional>

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>
#include <boost/algorithm/string/replace.hpp>

#ifdef OSX_BUILD
#define CONNECT_ICON ":/res/icon-connected-black.png"
#define DISCONNECT_ICON ":/res/icon-disconnected-black.png"
#define CONNECT_STATUS_ICON ":/res/icon-connected-status-black.png"
#define CONNECT_ADHOC_ICON ":/res/icon-connected-adhoc-black.png"

#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>

#include "build/NFD/build/core/version.hpp"
#include "build/ndn-tools/build/core/version.cpp"

#else
#define CONNECT_ICON ":/res/icon-connected-white.png"
#define DISCONNECT_ICON ":/res/icon-disconnected-white.png"

#endif // OSX_BUILD

#include <ndn-cxx/version.hpp>

namespace ndn {
namespace ncc {

/**
 * @brief Maximum number of lines that can show up in auto-config status tab view
 */
const int MAX_LINES_IN_AUTOCONF_STATUS = 100;

#ifdef OSX_BUILD
const QString AUTO_START_SUFFIX = "Library/LaunchAgents/net.named-data.control-center.plist";
#endif // OSX_BUILD

TrayMenu::TrayMenu(QQmlContext* context, Face& face, KeyChain& keyChain)
  : m_context(context)
  , m_isNfdRunning(false)
  , m_isConnectedToHub(false)
  , m_isConnectedToAdhoc(false)
  , m_menu(new QMenu(this))
  , m_entryPref(new QAction("Preferences...", m_menu))
  , m_entryStatus(new QAction("Status...", m_menu))
  , m_entrySec(new QAction("Security...", m_menu))
  , m_acProc(nullptr)
  , m_settings(new QSettings())
#ifdef OSX_BUILD
  , m_entryEnableCli(new QAction("Install command-line tools...", m_menu))
  , m_entryAdhoc(new QAction("Enable ad hoc Wi-Fi...", m_menu))
  , m_checkForUpdates(new QAction("Check for updates", m_menu))
  , m_sparkle(NCC_APPCAST)
#endif
  , m_entryQuit(new QAction("Quit", m_menu))
  , m_keyViewerDialog(new ncc::KeyViewerDialog)
  , m_face(face)
  , m_keyChain(keyChain)
  , m_adhoc(m_face, m_keyChain)
  , m_statusViewer(new StatusViewer(m_face, m_keyChain))
{
  connect(m_entryPref, SIGNAL(triggered()), this, SIGNAL(showApp()));
  connect(m_entryStatus, SIGNAL(triggered()), m_statusViewer, SLOT(present()));
  connect(m_entrySec, SIGNAL(triggered()), m_keyViewerDialog, SLOT(present()));
  connect(m_entryQuit, SIGNAL(triggered()), this, SLOT(quitApp()));

  connect(this, SIGNAL(nfdActivityUpdate(bool)), this, SLOT(updateNfdActivityIcon(bool)), Qt::QueuedConnection);
  connect(this, SIGNAL(connectivityUpdate(bool)), this, SLOT(updateConnectivity(bool)), Qt::QueuedConnection);
  connect(this, SIGNAL(adhocUpdate(bool)), this, SLOT(updateAdhoc(bool)), Qt::QueuedConnection);

  QString nccVersion = QString(NCC_VERSION) + " (ndn-cxx: " + NDN_CXX_VERSION_BUILD_STRING +
    ", NFD: " + NFD_VERSION_BUILD_STRING +
    ", ndn-tools: " + ::ndn::tools::VERSION +
    ")";

  m_context->setContextProperty("nccVersion", nccVersion);

  m_menu->addAction(m_entryStatus);
  m_menu->addAction(m_entrySec);

  m_menu->addSeparator();
  m_menu->addAction(m_entryPref);

#ifdef OSX_BUILD
  m_entryAdhoc->setChecked(false);
  m_entryAdhoc->setCheckable(true);
  connect(m_entryAdhoc, SIGNAL(triggered()), this, SLOT(onAdhocChange()));
  m_menu->addAction(m_entryAdhoc);

  connect(m_entryEnableCli, SIGNAL(triggered()), this, SLOT(enableCli()));
  m_menu->addAction(m_entryEnableCli);

  connect(m_checkForUpdates, SIGNAL(triggered()), this, SLOT(checkForUpdates()));
  m_menu->addAction(m_checkForUpdates);
#endif

  m_menu->addSeparator();
  m_menu->addAction(m_entryQuit);
  m_tray = new QSystemTrayIcon(this);
  m_tray->setContextMenu(m_menu);
  m_tray->setIcon(QIcon(DISCONNECT_ICON));
  m_tray->show();

  // delaying for the case when NFD is already started (to avoid starting / to avoid starting autoconfig
  if (isNfdAutoStartEnabled()) {
    scheduleDelayedTask(std::chrono::seconds(5), [this] {
        if (!m_isNfdRunning) {
          startNfd();
        }
      });
  }
}

TrayMenu::~TrayMenu()
{
  // delete for all new's
}

////////////////////////////////////////////////////////////////////////////////
// NFD Control Center start on login

Q_INVOKABLE bool
TrayMenu::isNccAutoStartEnabled() const
{
  QFileInfo file(QDir::home().path() + "/" + AUTO_START_SUFFIX);
  return file.exists() && file.isFile();
}

Q_INVOKABLE void
TrayMenu::enableDisableNccAutoStart(bool isEnabled)
{
  if (isEnabled) {
    QFile file(QDir::home().path() + "/" + AUTO_START_SUFFIX);
    file.open(QIODevice::WriteOnly);

    std::string plist = R"PLIST(<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<?xml version="1.0" encoding="UTF-8"?>
<plist version="1.0">
    <dict>
        <key>Label</key>
        <string>net.named-data.control-center</string>
        <key>ProgramArguments</key>
        <array>
            <string>%%PATH%%</string>
        </array>
        <key>RunAtLoad</key>
        <true/>
        <key>KeepAlive</key>
        <false/>
    </dict>
</plist>
)PLIST"; //"
    boost::replace_all(plist, "%%PATH%%", QCoreApplication::applicationFilePath().toStdString());
    file.write(plist.data(), plist.size());
    file.close();
  }
  else {
    QFile::remove(QDir::home().path() + "/" + AUTO_START_SUFFIX);
  }
}

////////////////////////////////////////////////////////////////////////////////
// NFD start on launch

Q_INVOKABLE bool
TrayMenu::isNfdAutoStartEnabled() const
{
  return m_settings->value("ENABLE_AUTO_START", false).toBool();
}

Q_INVOKABLE void
TrayMenu::enableDisableNfdAutoStart(bool isEnabled)
{
  m_settings->setValue("ENABLE_AUTO_START", isEnabled);
  if (!m_isNfdRunning) {
    startNfd();
  }
}

void
TrayMenu::startNfd()
{
#ifdef OSX_BUILD
  QProcess* proc = new QProcess();
  connect(proc, SIGNAL(finished(int)), proc, SLOT(deleteLater()));
  proc->startDetached((QCoreApplication::applicationDirPath() + "/../Helpers/nfd"),
                      QStringList()
                        << "--config"
                        << (QCoreApplication::applicationDirPath() + "/../etc/ndn/nfd.conf"));
#endif
}

void
TrayMenu::stopNfd()
{
#ifdef OSX_BUILD
  QProcess* proc = new QProcess();
  connect(proc, SIGNAL(finished(int)), proc, SLOT(deleteLater()));
  proc->startDetached("killall", QStringList() << "nfd");
#endif
}

////////////////////////////////////////////////////////////////////////////////
// NDN Autoconfig

Q_INVOKABLE bool
TrayMenu::isNdnAutoConfigEnabled() const
{
  return m_settings->value("ENABLE_NDN_AUTO_CONFIG", false).toBool();
}

Q_INVOKABLE void
TrayMenu::enableDisableNdnAutoConfig(bool isEnabled)
{
  m_settings->setValue("ENABLE_NDN_AUTO_CONFIG", isEnabled);

  if (isEnabled) {
    startNdnAutoConfig();
  }
  else {
    stopNdnAutoConfig();
  }
}

void
TrayMenu::startNdnAutoConfig()
{
  if (m_acProc != nullptr) {
    return;
  }

  stopNdnAutoConfig(false); // Make sure no more than one auto-config process exist

  if (!m_isNfdRunning) {
    return;
  }

  m_acProc = new QProcess();

  scheduleDelayedTask(std::chrono::seconds(2), [this] {
      appendNdnAutoConfigStatus("NDN auto configuration starting...\n");
      m_acProc->start(QCoreApplication::applicationDirPath() + "/../Helpers/ndn-autoconfig",
                      QStringList() << "--daemon");
      connect(m_acProc, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));
      connect(m_acProc, SIGNAL(readyReadStandardError()), this, SLOT(processOutput()));
    });
}

void
TrayMenu::stopNdnAutoConfig(bool appendStatus)
{
  m_acProc = nullptr;
  QProcess* proc = new QProcess();
  connect(proc, SIGNAL(finished(int)), proc, SLOT(deleteLater()));
  proc->startDetached("killall", QStringList() << "ndn-autoconfig");
  if (appendStatus) {
    appendNdnAutoConfigStatus("NDN auto configuration stopped\n");
  }
}

void
TrayMenu::appendNdnAutoConfigStatus(const QString& message)
{
  if (message == "") {
    return;
  }
  if (m_ndnAutoConfigMsg.count("\n") > MAX_LINES_IN_AUTOCONF_STATUS) {
    int end = m_ndnAutoConfigMsg.indexOf("\n");
    m_ndnAutoConfigMsg.remove(0, end + 1);
  }
  m_ndnAutoConfigMsg.append(message);

  m_context->setContextProperty("ndnAutoConfigText", m_ndnAutoConfigMsg);
}

void
TrayMenu::processOutput()
{
  if (m_acProc == nullptr) {
    return;
  }
  appendNdnAutoConfigStatus(m_acProc->readAllStandardOutput());
  appendNdnAutoConfigStatus(m_acProc->readAllStandardError());
}

////////////////////////////////////////////////////////////////////////////////
// NFD stop on shutdown

Q_INVOKABLE bool
TrayMenu::isNfdStopOnExitEnabled() const
{
  return m_settings->value("ENABLE_SHUTDOWN", false).toBool();
}

Q_INVOKABLE void
TrayMenu::enableDisableNfdStopOnExit(bool isEnabled)
{
  m_settings->setValue("ENABLE_SHUTDOWN", isEnabled);
}

///////////////////////////////////////////////////////////////////////////////
// Ad hoc WiFi communication

void
TrayMenu::onAdhocChange()
{
#ifdef ADHOC_SUPPORTED
  if (m_entryAdhoc->isChecked()) {
    QMessageBox* msgBox = new QMessageBox();
    msgBox->setText("WARNING: Wi-Fi will be disconnected!");
    msgBox->setIcon(QMessageBox::Warning);
    msgBox->setInformativeText(
      "Are you sure that you are OK with that?");
    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox->setDefaultButton(QMessageBox::No);
    int ret = msgBox->exec();

    if (ret == QMessageBox::No) {
      m_entryAdhoc->setChecked(false);
    }
    else {
      stopNdnAutoConfig();
      m_entryAdhoc->setText("Adhoc Wi-Fi is enabled");
      m_adhoc.createAdhoc();
      std::cerr << "ad hoc is on!" << std::endl;
    }
  }
  else {
    m_entryAdhoc->setText("Enable Adhoc Wi-Fi...");
    m_adhoc.destroyAdhoc();
    std::cerr << "ad hos is off!" << std::endl;
    // a trick in DestroyAdhoc ensures that WiFi will be reconnected to a default WiFi
    if (isNdnAutoConfigEnabled()) {
      startNdnAutoConfig();
    }
  }
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Misc

void
TrayMenu::quitApp()
{
  if (isNfdStopOnExitEnabled()) {
    stopNdnAutoConfig();
    stopNfd();
  }
  QCoreApplication::exit(0);
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
    if (m_isConnectedToHub) {
      m_tray->setIcon(QIcon(CONNECT_STATUS_ICON));
    }
    else if (m_isConnectedToAdhoc) {
      m_tray->setIcon(QIcon(CONNECT_ADHOC_ICON));
    }
    else {
      m_tray->setIcon(QIcon(CONNECT_ICON));
    }
    if (!m_entryAdhoc->isChecked() && isNdnAutoConfigEnabled()) {
      startNdnAutoConfig();
    }
  }
  else {
    m_tray->setIcon(QIcon(DISCONNECT_ICON));
  }
}

void
TrayMenu::updateConnectivity(bool isConnectedToHub)
{
  m_isConnectedToHub = isConnectedToHub;
}

void
TrayMenu::updateAdhoc(bool isConnectedToAdhoc)
{
  m_isConnectedToAdhoc = isConnectedToAdhoc;
}

void
TrayMenu::enableCli()
{
#ifdef OSX_BUILD
  QProcess* proc = new QProcess();
  connect(proc, SIGNAL(finished(int)), proc, SLOT(deleteLater()));

  proc->start("osascript", QStringList()
              << "-e"
              << "do shell script \""
                   "/bin/mkdir -vp /usr/local/bin; "
                   "/bin/ln -s -f '" +  QCoreApplication::applicationDirPath() +
                     "/../Resources/ndn" + "' /usr/local/bin/ndn;"
                   "\" with administrator privileges");
#endif
}

#ifdef OSX_BUILD
void
TrayMenu::checkForUpdates()
{
  m_sparkle.checkForUpdates();
}
#endif // OSX_BUILD

void
TrayMenu::scheduleDelayedTask(std::chrono::milliseconds delays, const std::function<void()>& task)
{
  auto timer = new QTimer();
  timer->setSingleShot(true);
  timer->setInterval(delays);
  connect(timer, &QTimer::timeout,
          [task, timer] {
            task();
            timer->deleteLater();
          });
  timer->start();
}

} // namespace ncc
} // namespace ndn
