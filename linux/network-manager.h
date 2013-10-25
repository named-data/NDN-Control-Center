/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include <QtDBus>

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#define NDND_AUTOCONFIG_COMMAND "/usr/local/bin/ndnd-autoconfig"
#define NM_STATE_DISCONNECTED 20
#define NM_STATE_CONNECTING 40
#define NM_STATE_CONNECTED_GLOBAL 70

class NetworkManager : public QObject
{
     Q_OBJECT

public:
    NetworkManager();

private:
    void autoconfigDaemon();

private slots:
    void stateChanged(uint state);
};


#endif // NETWORKMANAGER_H
