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
#include <QStandardItemModel>
#include <QtXml>
#include <QThread>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "fib-input-dialog.h"
#include "quit-dialog.h"
#include "network-manager.h"

#define ALLOW_SOFTWARE_UPDATES "AllowAutomaticUpdates"
#define ENABLE_HUB_DISCOVERY "EnableHubDiscovery"
#define ENABLE_START_ON_LOGIN "enableStartOnLogin"
#define SHUTDOWN_ON_EXIT "ShutdownOnExit"

#define AUTOSTART_DIRECTORY "/.config/autostart/"
#define SHORTCUT_FILE "ndnxcontrolcenter.desktop"
#define STATUS_XSLT_FILE "status.xslt"
#define FIB_XSLT_FILE "status-to-fib.xslt"

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

    void resizeEvent (QResizeEvent * event);

    bool daemonStarted;

    QSettings *persistentSettings;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QTimer *daemonStatusTimer;
    QThread *statusUpdateThread;

    QProcess *processLookup;
    QProcess *applyStatusXslt;
    QProcess *applyFibXslt;

    QString statusXml;
    QString fibContentsXml;

    QStandardItemModel *model;
    int selectedRow;
    int scrollPosition;

    QAction *statusIndicator;
    QAction *displayStatus;
    QAction *interestSentRecv;
    QAction *dataSentRecv;
    QAction *open;
    QAction *close;
    QAction *openGeneralSettings;
    QAction *openForwardingSettings;
    QAction *openSecuritySettings;

    FibInputDialog *dialog;

    bool enableHubDiscovery;
    bool enableStartOnLogin;

    NetworkManager *networkManager;
    QNetworkAccessManager *urlManager;

private slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason);
    void generalSettingsClicked();
    void forwardingSettingsClicked();
    void securitySettingsClicked();
    void openTrafficMap();
    void openRoutingStatus();
    void openCertificationPage();
    void displayPopup();
    // void confirmQuit();
    void showFibInputDialog();
    // void terminateDaemonAndClose();
    void addFibEntry();
    void daemonStatusUpdate();
    void selectTableRow();
    void deleteFibEntry();
    void changeHubDiscovery();
    void changeLoginStart();
    void copyFile();
    void runXmlProc(QNetworkReply *reply);
    void parseStatusXml();
    void parseFibXml();
    void parseProcessLookup();
};

#endif // TRAYMENU_H
