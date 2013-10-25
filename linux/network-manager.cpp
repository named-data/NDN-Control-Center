/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "network-manager.h"
#include <QProcess>

NetworkManager::NetworkManager()
{
    if (!QDBusConnection::systemBus().isConnected())
    {
        qDebug() << "Cannot connect to the D-Bus session bus.\n";
        return;
    }

    QDBusConnection::systemBus().connect("org.freedesktop.NetworkManager",
                                          "/org/freedesktop/NetworkManager",
                                          "org.freedesktop.NetworkManager",
                                          "StateChanged", this, SLOT(stateChanged(uint)));
}

void NetworkManager::stateChanged(uint state)
{
    qDebug() << "State = " << state;

    if(state == NM_STATE_CONNECTED_GLOBAL)
        autoconfigDaemon();
}

void NetworkManager::autoconfigDaemon()
{
    QProcess *process = new QProcess();
    process->start(NDND_AUTOCONFIG_COMMAND);
}
