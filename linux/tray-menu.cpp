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
#include "network-manager.h"

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
#include <QDir>
#include <QWidgetAction>
#include <QTextStream>

TrayMenu::TrayMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrayMenu)
{
    networkManager = new NetworkManager();

    QCoreApplication::setOrganizationName("UCLA");
    QCoreApplication::setOrganizationDomain("named-data.net");
    QCoreApplication::setApplicationName("NDNx Control Center");

    persistentSettings = new QSettings();

    ui->setupUi(this);

    createTrayIcon();
    setIcon(true);
    createToolbar();
    createTableView();

    loadSettings();

    connect(ui->openTrafficMapButton, SIGNAL(pressed()), this, SLOT(openTrafficMap()));
    connect(ui->openRoutingStatusButton, SIGNAL(pressed()), this, SLOT(openRoutingStatus()));
    connect(ui->addFibButton, SIGNAL(pressed()), this, SLOT(showFibInputDialog()));
    connect(ui->deleteFibButton, SIGNAL(released()), this, SLOT(deleteFibEntry()));
    connect(ui->softwareUpdateCheckbox, SIGNAL(stateChanged(int)), this, SLOT(changeSoftwareUpdate()));
    connect(ui->hubDiscoveryCheckbox, SIGNAL(stateChanged(int)), this, SLOT(changeHubDiscovery()));
    connect(ui->loginStartCheckbox, SIGNAL(stateChanged(int)), this, SLOT(changeLoginStart()));
    connect(ui->shutdownCheckbox, SIGNAL(stateChanged(int)), this, SLOT(changeShutdownExit()));
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectTableRow()));
    connect(ui->openNdnCertificationButton, SIGNAL(released()), this, SLOT(openCertificationPage()));

    statusUpdateThread = new QThread();
    statusUpdateThread->start();

    daemonStatusTimer = new QTimer(statusUpdateThread);
    connect(daemonStatusTimer, SIGNAL(timeout()), this, SLOT(daemonStatusUpdate()));
    daemonStatusTimer->start(1000);

    trayIcon->show();
}

void TrayMenu::loadSettings()
{
    QVariant value;

    value = persistentSettings->value(ALLOW_SOFTWARE_UPDATES);
    if(!value.isNull())
    {
        allowAutomaticUpdates = value.toBool();
        ui->softwareUpdateCheckbox->setChecked(allowAutomaticUpdates);
    }
    else
    {
        ui->softwareUpdateCheckbox->setChecked(true);
        changeSoftwareUpdate();
    }

    value = persistentSettings->value(ENABLE_HUB_DISCOVERY);
    if(!value.isNull())
    {
        enableHubDiscovery = value.toBool();
        ui->hubDiscoveryCheckbox->setChecked(enableHubDiscovery);
    }
    else
    {
        ui->hubDiscoveryCheckbox->setChecked(true);
        changeHubDiscovery();
    }

    value = persistentSettings->value(ENABLE_START_ON_LOGIN);
    if(!value.isNull())
    {
        enableStartOnLogin = value.toBool();
        ui->loginStartCheckbox->setChecked(enableStartOnLogin);
    }
    else
    {
        ui->loginStartCheckbox->setChecked(false);
        changeLoginStart();
    }

    value = persistentSettings->value(SHUTDOWN_ON_EXIT);
    if(!value.isNull())
    {
        shutdownOnExit = value.toBool();
        ui->shutdownCheckbox->setChecked(shutdownOnExit);
    }
    else
    {
        ui->shutdownCheckbox->setChecked(false);
        changeShutdownExit();
    }
}

void TrayMenu::changeSoftwareUpdate()
{
    if(ui->softwareUpdateCheckbox->isChecked())
    {
        allowAutomaticUpdates = true;
        persistentSettings->setValue(ALLOW_SOFTWARE_UPDATES, true);
    }
    else
    {
        allowAutomaticUpdates = false;
        persistentSettings->setValue(ALLOW_SOFTWARE_UPDATES, false);
    }
}

void TrayMenu::changeHubDiscovery()
{
    if(ui->hubDiscoveryCheckbox->isChecked())
    {
        enableHubDiscovery = true;
        persistentSettings->setValue(ENABLE_HUB_DISCOVERY, true);
    }
    else
    {
        enableHubDiscovery = false;
        persistentSettings->setValue(ENABLE_HUB_DISCOVERY, false);
    }
}

void TrayMenu::changeLoginStart()
{
    if(ui->loginStartCheckbox->isChecked())
    {
        enableStartOnLogin = true;
        persistentSettings->setValue(ENABLE_START_ON_LOGIN, true);
        makeAutostartDirectory();
    }
    else
    {
        enableStartOnLogin = false;
        persistentSettings->setValue(ENABLE_START_ON_LOGIN, false);

        QProcess *process = new QProcess();
        QStringList arguments;
        arguments << QDir::homePath() + AUTOSTART_DIRECTORY + SHORTCUT_FILE;
        process->start("rm", arguments);
    }
}

void TrayMenu::makeAutostartDirectory()
{
    QProcess *process = new QProcess();
    connect(process, SIGNAL(finished(int)), this, SLOT(copyFile()));
    QStringList arguments;
    arguments << QDir::homePath() + AUTOSTART_DIRECTORY;
    process->start("mkdir", arguments);
}

void TrayMenu::copyFile()
{
    QProcess *process = new QProcess();
    QStringList arguments;
    arguments << QApplication::applicationDirPath() + "/" + SHORTCUT_FILE << QDir::homePath() + AUTOSTART_DIRECTORY;
    process->start("cp",arguments);
}

void TrayMenu::changeShutdownExit()
{
    if(ui->shutdownCheckbox->isChecked())
    {
        shutdownOnExit = true;
        persistentSettings->setValue(SHUTDOWN_ON_EXIT, true);
    }
    else
    {
        shutdownOnExit = false;
        persistentSettings->setValue(SHUTDOWN_ON_EXIT, false);
    }
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

void TrayMenu::openCertificationPage()
{
    QDesktopServices::openUrl(QUrl("http://ndncert.named-data.net", QUrl::TolerantMode));
}

void TrayMenu::createTrayIcon()
{
    trayIconMenu = new QMenu(this);

    statusIndicator = new QAction(tr("Inactive"), this);
    trayIconMenu->addAction(statusIndicator);

    trayIconMenu->addSeparator();

    displayStatus = new QAction("                           Sent / Recv   ", this);
    //connect(displayStatus, SIGNAL(triggered()), this, SLOT(displayPopup()));
    trayIconMenu->addAction(displayStatus);
    interestSentRecv = new QAction("Interests      0 / 0", this);
    trayIconMenu->addAction(interestSentRecv);
    dataSentRecv = new QAction("Data               0 / 0", this);
    trayIconMenu->addAction(dataSentRecv);


    trayIconMenu->addSeparator();

    open = new QAction(tr("Preferences..."), this);
    connect(open, SIGNAL(triggered()), this, SLOT(show()));
    trayIconMenu->addAction(open);

    close = new QAction(tr("Quit..."), this);
    connect(close, SIGNAL(triggered()), this, SLOT(confirmQuit()));
    trayIconMenu->addAction(close);




    /*QWidgetAction * wa = new QWidgetAction(this);
    wa->setDefaultWidget(new QPushButton("Default"));

    trayIconMenu->setDefaultAction(wa);
*/
    //trayIconMenu->addAction();
    /*QVBoxLayout *layout = new QVBoxLayout(wa->defaultWidget());

    QLineEdit *edit = new QLineEdit("", wa->defaultWidget());
    layout->addWidget(edit);

    trayIconMenu->addAction(wa);*/

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

    QProcess *process = new QProcess();
    process->start(NDND_FIB_COMMAND, arguments);
}

void TrayMenu::confirmQuit()
{
    if(shutdownOnExit)
        terminateDaemonAndClose();
    else
    {
        QuitDialog dialog(this);
        dialog.exec();
    }
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

void TrayMenu::daemonStatusUpdate()
{
    QXmlQuery query(QXmlQuery::XSLT20);
    query.setFocus(QUrl("http://localhost:9695/?f=xml"));

    query.setQuery(QUrl("qrc:/resource/Resources/status.xslt")); // TODO: I suspect it's being read from HDD each time
    query.evaluateTo(&statusXml);

    if(statusXml == "") // there was an error during Query evaluation
    {
        daemonStarted = false;
        setIcon(false);
        statusIndicator->setText(tr("Starting..."));

        QProcess *process = new QProcess();
        if(enableHubDiscovery)
            connect(process, SIGNAL(finished(int)), networkManager, SLOT(autoconfigDaemon()));
        process->start(NDND_START_COMMAND);
    }
    else
    {
        daemonStarted = true;
        setIcon(true);
        statusIndicator->setText(tr("Active"));
        //qDebug() << statusXml;
        QString interestIn = statusXml.mid(statusXml.indexOf("<in>")+4, statusXml.indexOf("</in>") - (statusXml.indexOf("<in>")+4));
        QString interestOut = statusXml.mid(statusXml.indexOf("<out>")+5, statusXml.indexOf("</out>") - (statusXml.indexOf("<out>")+5));
        QString dataIn = statusXml.mid(statusXml.lastIndexOf("<in>")+4, statusXml.lastIndexOf("</in>") - (statusXml.lastIndexOf("<in>")+4));
        QString dataOut = statusXml.mid(statusXml.lastIndexOf("<out>")+5, statusXml.lastIndexOf("</out>") - (statusXml.lastIndexOf("<out>")+5));

        int i = 0;
        int k = 0;
        if((dataOut.length() - interestOut.length()) > 0)
        {
            i = dataOut.length() - interestOut.length();
            i*=2; //because space takes less space than a letter
        }

        if((interestOut.length() - dataOut.length()) > 0)
        {
            k = interestOut.length() - dataOut.length();
            k*=2; //because space takes less space than a letter
        }

        QString interestStats = QString("%1%2%3%4").arg("Interests",-16,' ').arg(interestOut,6+i,' ').arg(" / ",3).arg(interestIn,-6,' ');
        QString dataStats = QString("%1%2%3%4").arg("Data",-20,' ').arg(dataOut,6+k,' ').arg(" / ",3).arg(dataIn,-6,' ');

        //Now I try to align header "Sent / Recv" centrally with the upper line
        QString padding;
        for(int j = 0; j < interestStats.indexOf(interestOut); j++)
        {
            if(interestStats.at(j)==' ')
                padding +=" ";
            else
                padding += "  "; //because space takes less space than a letter
        }

        QString header;
        int m = 0;
        if(interestOut.length() - QString("Sent").length() > 0)
        {
            m = interestOut.length() - QString("Sent").length();
            m *=3;
            //qDebug() << "m=" << m;
            header = QString("%1%2").arg(padding).arg("  Sent / Recv",QString("  Sent / Recv").length() + m,' ');
        }
        else if(interestOut.length() - QString("Sent").length() < 0)
        {
            //qDebug() << "truncating";
            padding.truncate(padding.length()-(QString("Sent").length() - interestOut.length()));
            header = padding + "Sent / Recv";
        }
        else
        {
            header = padding + "  Sent / Recv";
        }

        interestSentRecv->setText(interestStats);
        dataSentRecv->setText(dataStats);
        displayStatus->setText(header);
    }

    query.setQuery(QUrl("qrc:/resource/Resources/status-to-fib.xslt"));  // TODO: I suspect it's being read from HDD each time
    query.evaluateTo(&fibContentsXml);

    if ((enableHubDiscovery) && (fibContentsXml.indexOf("ndn:/autoconf-route",0,Qt::CaseInsensitive) == -1))
    {
        networkManager->autoconfigDaemon();
    }

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

    QProcess *process = new QProcess();
    process->start(NDND_FIB_COMMAND, arguments);
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

#if WAF
#include "tray-menu.moc"
#include "tray-menu.cpp.moc"
#endif
