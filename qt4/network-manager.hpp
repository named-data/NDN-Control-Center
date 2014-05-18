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

#include <QtDBus>
#include <QProcess>

#ifndef NCC_QT_NETWORK_MANAGER_HPP
#define NCC_QT_NETWORK_MANAGER_HPP

#define NM_STATE_DISCONNECTED 20
#define NM_STATE_CONNECTING 40
#define NM_STATE_CONNECTED_GLOBAL 70

class NetworkManager : public QObject
{
     Q_OBJECT

public:
    NetworkManager();
    bool IsAutoconfigRunning();

public slots:
    void autoconfigDaemon();

private:
    QProcess *autoconfigProcess;
    bool isAutoconfigRunning;

private slots:
    void stateChanged(uint state);
    void autoconfigFinished();
};


#endif // NCC_QT_NETWORK_MANAGER_HPP
