/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "config.h"

#include "tray-menu.h"
#include "ui_traymenu.h"   //generated from traymenu.ui
#include "quit-dialog.h"
#include "fib-input-dialog.h"
#include "network-manager.h"

#include <QMenu>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDir>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QVBoxLayout>

TrayMenu::TrayMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrayMenu)
{
    processLookup = new QProcess(this);
    QStringList arguments;
    arguments << "-al";
    connect(processLookup,SIGNAL(finished(int)), this, SLOT(parseProcessLookup()));
    processLookup->start("ps",arguments);

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
    connect(ui->hubDiscoveryCheckbox, SIGNAL(stateChanged(int)), this, SLOT(changeHubDiscovery()));
    connect(ui->loginStartCheckbox, SIGNAL(stateChanged(int)), this, SLOT(changeLoginStart()));
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectTableRow()));
    connect(ui->openNdnCertificationButton, SIGNAL(released()), this, SLOT(openCertificationPage()));

    statusUpdateThread = new QThread();
    statusUpdateThread->start();

    daemonStatusTimer = new QTimer(statusUpdateThread);
    connect(daemonStatusTimer, SIGNAL(timeout()), this, SLOT(daemonStatusUpdate()));
    daemonStatusTimer->start(2000);

    trayIcon->show();
    model = NULL;
    dialog = new FibInputDialog(this);

    urlManager = new QNetworkAccessManager(this);
    connect(urlManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(runXmlProc(QNetworkReply*)));
}

void TrayMenu::loadSettings()
{
    QVariant value;

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
        connect(process,SIGNAL(finished(int)),process, SLOT(deleteLater()));
        QStringList arguments;
        arguments << QDir::homePath() + AUTOSTART_DIRECTORY + SHORTCUT_FILE;
        process->start("rm", arguments);
    }
}

void TrayMenu::makeAutostartDirectory()
{
    QProcess *process = new QProcess();
    connect(process, SIGNAL(finished(int)), this, SLOT(copyFile()));
    connect(process,SIGNAL(finished(int)),process, SLOT(deleteLater()));
    QStringList arguments;
    arguments << QDir::homePath() + AUTOSTART_DIRECTORY;
    process->start("mkdir", arguments);
}

void TrayMenu::copyFile()
{
    QProcess *process = new QProcess();
    connect(process,SIGNAL(finished(int)),process, SLOT(deleteLater()));
    QStringList arguments;
    arguments << QString(RESOURCES_DIR) + "/" + SHORTCUT_FILE << QDir::homePath() + AUTOSTART_DIRECTORY;
    process->start("cp",arguments);
}

void TrayMenu::showFibInputDialog()
{
    dialog->clear();
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

    statusIndicator = new QAction("Inactive", this);
    trayIconMenu->addAction(statusIndicator);

    trayIconMenu->addSeparator();

    displayStatus = new QAction("                           Sent / Recv   ", this);
    trayIconMenu->addAction(displayStatus);
    interestSentRecv = new QAction("Interests      0 / 0", this);
    trayIconMenu->addAction(interestSentRecv);
    dataSentRecv = new QAction("Data               0 / 0", this);
    trayIconMenu->addAction(dataSentRecv);

    trayIconMenu->addSeparator();

    open = new QAction("Preferences...", this);
    connect(open, SIGNAL(triggered()), this, SLOT(show()));
    trayIconMenu->addAction(open);

    close = new QAction("Quit", this);
    // connect(close, SIGNAL(triggered()), this, SLOT(confirmQuit()));
    connect(close, SIGNAL(triggered()), qApp, SLOT(quit()));
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
    ui->toolBar->setFloatable(false);
    ui->toolBar->setMovable(false);
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolBar->setAllowedAreas(Qt::TopToolBarArea);
    ui->toolBar->setOrientation(Qt::Horizontal);
    ui->toolBar->setIconSize(QSize(32,32));
    ui->toolBar->resize(this->width(), 64);

    openGeneralSettings = new QAction("General", this);
    openGeneralSettings->setIcon(QIcon(":/resource/Resources/preferences-desktop.png"));
    connect(openGeneralSettings,SIGNAL(triggered()),this, SLOT(generalSettingsClicked()));

    openForwardingSettings = new QAction("Forwarding", this);
    openForwardingSettings->setIcon(QIcon(":/resource/Resources/emblem-web.png"));
    connect(openForwardingSettings,SIGNAL(triggered()),this, SLOT(forwardingSettingsClicked()));

    openSecuritySettings = new QAction("Security", this);
    openSecuritySettings->setIcon(QIcon(":/resource/Resources/emblem-system.png"));
    connect(openSecuritySettings,SIGNAL(triggered()),this, SLOT(securitySettingsClicked()));

    ui->toolBar->addAction(openGeneralSettings);
    ui->toolBar->addAction(openForwardingSettings);
    ui->toolBar->addAction(openSecuritySettings);
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
    trayIcon->showMessage("NDNx Status", statusXml);
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
    connect(process,SIGNAL(finished(int)),process, SLOT(deleteLater()));
    process->start(NDND_FIB_COMMAND, arguments);
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
    urlManager->get(QNetworkRequest(QUrl("http://localhost:6363/?f=xml")));
}

void TrayMenu::runXmlProc(QNetworkReply *reply)
{
    QByteArray buffer = reply->readAll();
    applyStatusXslt = new QProcess();

    QStringList arguments;
    arguments << QString(RESOURCES_DIR) + "/" + STATUS_XSLT_FILE << "-";
    connect(applyStatusXslt,SIGNAL(finished(int)), this, SLOT(parseStatusXml()));
    //connect(applyStatusXslt,SIGNAL(finished(int)), applyStatusXslt, SLOT(deleteLater()));
    applyStatusXslt->start(XSLTPROC,arguments);
    applyStatusXslt->write(buffer);
    applyStatusXslt->closeWriteChannel();

    applyFibXslt = new QProcess();
    QStringList args;
    args << QString(RESOURCES_DIR) + "/" + FIB_XSLT_FILE << "-";
    connect(applyFibXslt,SIGNAL(finished(int)), this, SLOT(parseFibXml()));
    //connect(applyFibXslt,SIGNAL(finished(int)), applyFibXslt, SLOT(deleteLater()));
    applyFibXslt->start(XSLTPROC,args);
    applyFibXslt->write(buffer);
    applyFibXslt->closeWriteChannel();
}

void TrayMenu::parseStatusXml()
{
    QByteArray stdout = applyStatusXslt->readAllStandardOutput();
    QByteArray stderr = applyStatusXslt->readAllStandardError();
    applyStatusXslt->deleteLater();

    statusXml = QString(stdout);

    if((statusXml == "") || (stderr.length()>0)) // there was an error during Query evaluation
    {
        daemonStarted = false;
        setIcon(false);
        statusIndicator->setText("Inactive");

        // QProcess *process = new QProcess();
        // if(enableHubDiscovery)
        //     connect(process, SIGNAL(finished(int)), networkManager, SLOT(autoconfigDaemon()));
        // 
        // connect(process,SIGNAL(finished(int)), process, SLOT(deleteLater()));
        // process->start(NDND_START_COMMAND);
    }
    else
    {
        daemonStarted = true;
        setIcon(true);
        statusIndicator->setText("Active");

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
            header = QString("%1%2").arg(padding).arg("  Sent / Recv",QString("  Sent / Recv").length() + m,' ');
        }
        else if(interestOut.length() - QString("Sent").length() < 0)
        {
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
}

void TrayMenu::parseFibXml()
{
    QByteArray stdout = applyFibXslt->readAllStandardOutput();
    QByteArray stderr = applyFibXslt->readAllStandardError();
    applyFibXslt->deleteLater();

    fibContentsXml = QString(stdout);

    if((stdout == "") || (stderr.length()>0))
        return;

    if ((enableHubDiscovery) && (fibContentsXml.indexOf("ndn:/autoconf-route",0,Qt::CaseInsensitive) == -1))
    {
        networkManager->autoconfigDaemon();
    }

    QDomDocument xmldoc;
    QDomElement root;

    xmldoc.setContent(fibContentsXml);
    root=xmldoc.documentElement();

    if(model != NULL)
    {
        scrollPosition = ui->tableView->verticalScrollBar()->value();
        model->clear();
        delete model;
    }

    model = new QStandardItemModel(root.childNodes().count(),3);
    model->setHorizontalHeaderItem(0, new QStandardItem("NDN prefix"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Face ID"));
    model->setHorizontalHeaderItem(2, new QStandardItem("Endpoint"));

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

    ui->tableView->setColumnWidth(0, ui->tableView->size().width() / 2 );
    ui->tableView->setColumnWidth(1, ui->tableView->size().width() / 6 );
    
    if(selectedRow >= 0)
        ui->tableView->selectRow(selectedRow);

    if(scrollPosition >= 0)
        ui->tableView->verticalScrollBar()->setValue(scrollPosition);
}

void TrayMenu::createTableView()
{
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    selectedRow = -1;
    scrollPosition = -1;

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(ui->addFibButton,0, Qt::AlignLeft);
    hlayout->addWidget(ui->deleteFibButton, 20, Qt::AlignLeft);

    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->addWidget(ui->label);
    vlayout->addWidget(ui->tableView);
    vlayout->addLayout(hlayout);

    ui->forwardingSettingsWidget->setLayout(vlayout);

    QVBoxLayout *expanding = new QVBoxLayout();
    expanding->addWidget(ui->forwardingSettingsWidget);
    ui->centralWidget->setLayout(expanding); 
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
    connect(process,SIGNAL(finished(int)), process, SLOT(deleteLater()));
    process->start(NDND_FIB_COMMAND, arguments);
}

void TrayMenu::resizeEvent(QResizeEvent *  event)
{
    ui->tableView->setColumnWidth(0, ui->tableView->size().width() / 2);
    ui->tableView->setColumnWidth(1, ui->tableView->size().width() / 6);
}


void TrayMenu::parseProcessLookup()
{
    QByteArray stdout = processLookup->readAllStandardOutput();
    processLookup->deleteLater();
     
    QString s = QString(stdout);

    if(s == "")
      return;
    else if(s.indexOf("ndnx-control") != s.lastIndexOf("ndnx-control"))
      qApp->quit();
}

TrayMenu::~TrayMenu()
{
    daemonStatusTimer->stop();
    statusUpdateThread->exit(0);
    delete ui;
    delete trayIcon;
    delete trayIconMenu;
    delete open;
    delete close;
    delete openGeneralSettings;
    delete openForwardingSettings;
    delete openSecuritySettings;
    delete statusUpdateThread;
    delete dialog;
    delete networkManager;
    delete persistentSettings;
}

#if WAF
#include "tray-menu.moc"
#include "tray-menu.cpp.moc"
#endif
