/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "fib-input-dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

FibInputDialog::FibInputDialog(QWidget *parent)
    : QDialog(parent)
{
    prefixLabel = new QLabel(tr("NDN name (prefix): "));
    prefixTextField = new QLineEdit;
    prefixLabel->setBuddy(prefixTextField);

    tunnelLabel = new QLabel(tr("Tunnel type: "));
    tunnelComboBox = new QComboBox;
    tunnelComboBox->addItem(tr("TCP"));
    tunnelComboBox->addItem(tr("UDP"));
    tunnelComboBox->setEditable(false);
    tunnelComboBox->setCurrentIndex(0);

    endpointLabel = new QLabel(tr("Endpoint (IP address): "));
    endpointTextField = new QLineEdit;
    endpointLabel->setBuddy(endpointTextField);

    okButton = new QPushButton(tr("&Ok"));
    cancelButton = new QPushButton(tr("&Cancel"));

    connect(okButton,SIGNAL(pressed()), parent, SLOT(addFibEntry()));
    connect(cancelButton,SIGNAL(pressed()), this, SLOT(hide()));

    buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(okButton, QDialogButtonBox::ActionRole);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(tunnelLabel);
    hlayout->addWidget(tunnelComboBox);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(prefixLabel);
    layout->addWidget(prefixTextField);
    layout->addLayout(hlayout);
    layout->addWidget(endpointLabel);
    layout->addWidget(endpointTextField);
    layout->addWidget(buttonBox);

    setLayout(layout);

    setWindowTitle(tr("NDNx Control Center"));
}

QString
FibInputDialog::getPrefixName()
{
    return prefixTextField->text();
}

QString
FibInputDialog::getEndpoint()
{
    return endpointTextField->text();
}

QString
FibInputDialog::getTunnelType()
{
    return tunnelComboBox->currentText();
}
