/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include <QtDBus>
#include <QProcess>

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


#endif // NETWORKMANAGER_H
