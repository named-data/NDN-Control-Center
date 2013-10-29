/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#ifndef TRAYMENU_H
#define TRAYMENU_H

#include <QMainWindow>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer>
#include <QProcess>
#include <QThread>
#include <QXmlStreamReader>
#include <QStandardItemModel>
#include <QtXml>
#include <QThread>
#include <QSettings>

#include "fib-input-dialog.h"
#include "quit-dialog.h"
#include "network-manager.h"

#define NDND_START_COMMAND "/usr/local/bin/ndndstart"
#define NDND_STOP_COMMAND "/usr/local/bin/ndndstop"
#define NDND_STATUS_COMMAND "/usr/local/bin/ndndsmoketest"
#define NDND_FIB_COMMAND "/usr/local/bin/ndndc"

#define ALLOW_SOFTWARE_UPDATES "AllowAutomaticUpdates"
#define ENABLE_HUB_DISCOVERY "EnableHubDiscovery"
#define ENABLE_START_ON_LOGIN "enableStartOnLogin"
#define SHUTDOWN_ON_EXIT "ShutdownOnExit"

#define AUTOSTART_DIRECTORY "/.config/autostart/"
#define SHORTCUT_FILE "ndnxcontrolcenter.desktop"

namespace Ui
{
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

    bool daemonStarted;

    QSettings *persistentSettings;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QToolBar *toolBar;

    QTimer *daemonStatusTimer;
    QThread *statusUpdateThread;

    QString statusXml;
    QString fibContentsXml;

    QStandardItemModel *model;
    int selectedRow;

    QAction *statusIndicator;
    QAction *open;
    QAction *close;
    QAction *openGeneralSettings;
    QAction *openForwardingSettings;
    QAction *openSecuritySettings;

    FibInputDialog *dialog;

    bool allowAutomaticUpdates;
    bool enableHubDiscovery;
    bool enableStartOnLogin;
    bool shutdownOnExit;

    NetworkManager *networkManager;

private slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason);
    void generalSettingsClicked();
    void forwardingSettingsClicked();
    void securitySettingsClicked();
    void openTrafficMap();
    void openRoutingStatus();
    void openCertificationPage();
    void displayPopup();
    void confirmQuit();
    void showFibInputDialog();
    void terminateDaemonAndClose();
    void addFibEntry();
    void daemonStatusUpdate();
    void selectTableRow();
    void deleteFibEntry();
    void changeSoftwareUpdate();
    void changeHubDiscovery();
    void changeLoginStart();
    void changeShutdownExit();
    void copyFile();
};

#endif // TRAYMENU_H
