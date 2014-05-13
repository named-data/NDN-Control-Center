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

#include "network-manager.hpp"
#include "config.hpp"

NetworkManager::NetworkManager()
  : isAutoconfigRunning(false)
{
  if (!QDBusConnection::systemBus().isConnected())
    {
      return;
    }

  autoconfigProcess = new QProcess(this);
  connect(autoconfigProcess,SIGNAL(finished(int)),this,SLOT(autoconfigFinished()));
  connect(autoconfigProcess, SIGNAL(error(QProcess::ProcessError)),
          this, SLOT(autoconfigFinished()));

  QDBusConnection::systemBus().connect("org.freedesktop.NetworkManager",
                                       "/org/freedesktop/NetworkManager",
                                       "org.freedesktop.NetworkManager",
                                       "StateChanged", this, SLOT(stateChanged(uint)));

  QDBusConnection::systemBus().connect("org.freedesktop.UPower",
                                       "/org/freedesktop/UPower",
                                       "org.freedesktop.UPower",
                                       "Resuming", this, SLOT(autoconfigDaemon()));
}

void NetworkManager::stateChanged(uint state)
{
  if (state == NM_STATE_CONNECTED_GLOBAL)
    autoconfigDaemon();
}

void NetworkManager::autoconfigDaemon()
{
  if (IsAutoconfigRunning())
    return;

  isAutoconfigRunning = true;

  if (autoconfigProcess != 0)
    autoconfigProcess->start(NFD_AUTOCONFIG_COMMAND);
}

void NetworkManager::autoconfigFinished()
{
  isAutoconfigRunning = false;
}

bool NetworkManager::IsAutoconfigRunning()
{
  return isAutoconfigRunning;
}

#if WAF
#include "network-manager.moc"
#include "network-manager.cpp.moc"
#endif
