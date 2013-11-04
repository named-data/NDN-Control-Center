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

    autoconfigProcess = new QProcess();
    connect(autoconfigProcess,SIGNAL(finished(int)),this,SLOT(autoconfigFinished()));
    connect(autoconfigProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(autoconfigFinished()));

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
    if(IsAutoconfigRunning())
        return;

     qDebug() << "started running";
     isAutoconfigRunning = true;
     autoconfigProcess->start(NDND_AUTOCONFIG_COMMAND);
}

void NetworkManager::autoconfigFinished()
{
    qDebug() << "stoped running";
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
