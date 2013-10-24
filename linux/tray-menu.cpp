/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "tray-menu.h"
#include "ui_traymenu.h"   //generated from traymenu.ui
#include "quit-dialog.h"
#include "fib-input-dialog.h"

#include <QMenu>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QProcess>
#include <QXmlStreamReader>
#include <QXmlQuery>
#include <QDebug>
#include <QtXml>
#include <QStandardItemModel>

TrayMenu::TrayMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrayMenu)
{
    ui->setupUi(this);

    createTrayIcon();
    setIcon(true);
    createToolbar();
    createTableView();

    startDaemon();

    connect(ui->openTrafficMapButton, SIGNAL(pressed()), this, SLOT(openTrafficMap()));
    connect(ui->openRoutingStatusButton, SIGNAL(pressed()), this, SLOT(openRoutingStatus()));
    connect(ui->addFibButton, SIGNAL(pressed()), this, SLOT(showFibInputDialog()));
    connect(ui->deleteFibButton, SIGNAL(released()), this, SLOT(deleteFibEntry()));
    connect(ui->softwareUpdateCheckbox, SIGNAL(stateChanged(int)), this, SLOT(changeSoftwareUpdate()));
    connect(ui->hubDiscoveryCheckbox, SIGNAL(stateChanged(int)), this, SLOT(changeHubDiscovery()));
    connect(ui->loginStartCheckbox, SIGNAL(stateChanged(int)), this, SLOT(changeLoginStart()));
    connect(ui->shutdownCheckbox, SIGNAL(stateChanged(int)), this, SLOT(changeShutdownExit()));
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectTableRow()));

    statusUpdateThread = new QThread();
    statusUpdateThread->start();

    daemonStatusTimer = new QTimer(statusUpdateThread);
    connect(daemonStatusTimer, SIGNAL(timeout()), this, SLOT(daemonStatusUpdate()));
    daemonStatusTimer->start(1000);

    trayIcon->show();
}

void TrayMenu::changeSoftwareUpdate()
{
    if(ui->softwareUpdateCheckbox->isChecked())
        allowAutomaticUpdates = true;
    else
        allowAutomaticUpdates = false;
}

void TrayMenu::changeHubDiscovery()
{
    if(ui->hubDiscoveryCheckbox->isChecked())
        enableHubDiscovery = true;
    else
        enableHubDiscovery = false;
}

void TrayMenu::changeLoginStart()
{
    if(ui->loginStartCheckbox->isChecked())
        enableStartOnLogin = true;
    else
        enableStartOnLogin = false;
}

void TrayMenu::changeShutdownExit()
{
    if(ui->shutdownCheckbox->isChecked())
        shutdownOnExit = true;
    else
        shutdownOnExit = false;
}

void TrayMenu::showFibInputDialog()
{
    dialog = new FibInputDialog(this);
    dialog->exec();
}

void TrayMenu::openTrafficMap()
{
    QDesktopServices::openUrl(QUrl("http://ndnmap.arl.wustl.edu/", QUrl::TolerantMode));
}

void TrayMenu::openRoutingStatus()
{
    QDesktopServices::openUrl(QUrl("http://netlab.cs.memphis.edu/script/htm/status.htm", QUrl::TolerantMode));
}

void TrayMenu::createTrayIcon()
{
    trayIconMenu = new QMenu(this);

    statusIndicator = new QAction(tr("Inactive"), this);
    trayIconMenu->addAction(statusIndicator);

    trayIconMenu->addSeparator();

    QAction *displayStatus = new QAction(tr("Status"), this);
    connect(displayStatus, SIGNAL(triggered()), this, SLOT(displayPopup()));
    trayIconMenu->addAction(displayStatus);

    trayIconMenu->addSeparator();

    open = new QAction(tr("Preferences..."), this);
    connect(open, SIGNAL(triggered()), this, SLOT(show()));
    trayIconMenu->addAction(open);

    close = new QAction(tr("Quit..."), this);
    connect(close, SIGNAL(triggered()), this, SLOT(confirmQuit()));
    trayIconMenu->addAction(close);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    connect(
            trayIcon,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason))
           );
}

void TrayMenu::createToolbar()
{
    toolBar = new QToolBar(this);
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->setAllowedAreas(Qt::TopToolBarArea);
    toolBar->setOrientation(Qt::Horizontal);
    toolBar->setIconSize(QSize(32,32));
    toolBar->resize(this->width(), 64);

    openGeneralSettings = new QAction(tr("General"), this);
    openGeneralSettings->setIcon(QIcon(":/resource/Resources/preferences-desktop.png"));
    connect(openGeneralSettings,SIGNAL(triggered()),this, SLOT(generalSettingsClicked()));

    openForwardingSettings = new QAction(tr("Forwarding"), this);
    openForwardingSettings->setIcon(QIcon(":/resource/Resources/emblem-web.png"));
    connect(openForwardingSettings,SIGNAL(triggered()),this, SLOT(forwardingSettingsClicked()));

    openSecuritySettings = new QAction(tr("Security"), this);
    openSecuritySettings->setIcon(QIcon(":/resource/Resources/emblem-system.png"));
    connect(openSecuritySettings,SIGNAL(triggered()),this, SLOT(securitySettingsClicked()));

    toolBar->addAction(openGeneralSettings);
    toolBar->addAction(openForwardingSettings);
    toolBar->addAction(openSecuritySettings);
}

void TrayMenu::generalSettingsClicked()
{
    ui->generalSettingsWidget->show();
    ui->forwardingSettingsWidget->hide();
    ui->securitySettingsWidget->hide();
}

void TrayMenu::forwardingSettingsClicked()
{
    ui->generalSettingsWidget->hide();
    ui->forwardingSettingsWidget->show();
    ui->securitySettingsWidget->hide();
}

void TrayMenu::securitySettingsClicked()
{
    ui->generalSettingsWidget->hide();
    ui->forwardingSettingsWidget->hide();
    ui->securitySettingsWidget->show();
}

void TrayMenu::displayPopup()
{
    trayIcon->showMessage(tr("NDNx Status"), statusXml);
}

void TrayMenu::addFibEntry()
{
    QString name = dialog->getPrefixName();
    QString tunnelType = dialog->getTunnelType();
    QString endpoint = dialog->getEndpoint();

    QStringList arguments;
    arguments << "add" << name << tunnelType << endpoint;

    dialog->hide();

    QProcess *process = new QProcess(this);
    process->start(NDND_FIB_COMMAND,arguments);
}

void TrayMenu::confirmQuit()
{
    QuitDialog dialog(this);
    dialog.exec();
}

void TrayMenu::terminateDaemonAndClose()
{
    QProcess *process = new QProcess(this);
    process->start(NDND_STOP_COMMAND);
    connect(process,SIGNAL(finished(int)), qApp, SLOT(quit()));
}

void TrayMenu::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}

void TrayMenu::showEvent(QShowEvent * event)
{
    ui->generalSettingsWidget->show();
    ui->forwardingSettingsWidget->hide();
    ui->securitySettingsWidget->hide();
}

void TrayMenu::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
        this->show();
}

void TrayMenu::setIcon(bool isConnected)
{
    if(isConnected)
        trayIcon->setIcon(QIcon(":/resource/Resources/icon-connected-white.png"));
    else
        trayIcon->setIcon(QIcon(":/resource/Resources/icon-disconnected-white.png"));
}

void TrayMenu::startDaemon()
{
    QProcess *process = new QProcess(this);
    process->start(NDND_START_COMMAND);
}

void TrayMenu::daemonStatusUpdate()
{
    QXmlQuery query(QXmlQuery::XSLT20);
    query.setFocus(QUrl("http://localhost:9695/?f=xml"));

    query.setQuery(QUrl("qrc:/resource/Resources/status.xslt")); // TODO: I suspect it's being read from HDD each time
    query.evaluateTo(&statusXml);

    query.setQuery(QUrl("qrc:/resource/Resources/status-to-fib.xslt"));  // TODO: I suspect it's being read from HDD each time
    query.evaluateTo(&fibContentsXml);

    QDomDocument xmldoc;
    QDomElement root;

    xmldoc.setContent(fibContentsXml);
    root=xmldoc.documentElement();

    model = new QStandardItemModel(root.childNodes().count(),3);
    model->setHorizontalHeaderItem(0, new QStandardItem(tr("NDN prefix")));
    model->setHorizontalHeaderItem(1, new QStandardItem(tr("Face ID")));
    model->setHorizontalHeaderItem(2, new QStandardItem(tr("Endpoint")));

    int row = 0;
    QDomNode fibEntry=root.firstChild();
    while (!fibEntry.isNull())
    {
        QDomNodeList properties = fibEntry.childNodes();

        QDomNode faceID = properties.at(0);
        QDomNode ip =  properties.at(1);
        QDomNode prefix = properties.at(2);

        // bad: causes scroll to go up every time and overall flickering
        /*QList<QStandardItem*> row = QList<QStandardItem*>() << new QStandardItem(prefix.toElement().text())
                                                                       << new QStandardItem(faceID.toElement().text())
                                                                       << new QStandardItem(ip.toElement().text());
        model->appendRow(row);*/

        model->setItem(row, 0, new QStandardItem(prefix.toElement().text()));
        model->setItem(row, 1, new QStandardItem(faceID.toElement().text()));
        model->setItem(row, 2, new QStandardItem(ip.toElement().text()));

        fibEntry = fibEntry.nextSibling();
        row++;
   }

    ui->tableView->setModel(model);

    if(selectedRow >= 0)
        ui->tableView->selectRow(selectedRow);
}

void TrayMenu::createTableView()
{
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    selectedRow = -1;
}

void TrayMenu::selectTableRow()
{
    selectedRow = ui->tableView->selectionModel()->selection().indexes().at(0).row();
}

void TrayMenu::deleteFibEntry()
{
    if(selectedRow < 0)
        return;

    QStandardItem *prefix = model->item(selectedRow,0);
    if(prefix == NULL)
        return;

    QStandardItem *faceID = model->item(selectedRow,1);
    if(faceID == NULL)
        return;

    QStringList arguments;
    arguments << "del" << prefix->text() << "face" << faceID->text();

    QProcess *process = new QProcess(this);
    process->start(NDND_FIB_COMMAND,arguments);
}

TrayMenu::~TrayMenu()
{
    daemonStatusTimer->stop();
    statusUpdateThread->exit();
    delete ui;
    delete trayIcon;
    delete trayIconMenu;
    delete open;
    delete close;
    delete openGeneralSettings;
    delete openForwardingSettings;
    delete openSecuritySettings;
    delete toolBar;
    delete statusUpdateThread;
    delete daemonStatusTimer;
    delete dialog;
}
