/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "quit-dialog.h"
#include <QVBoxLayout>
#include <QApplication>

QuitDialog::QuitDialog(QWidget *parent)
    : QDialog(parent)
{
    question = new QLabel(tr("Shutdown NDN daemon as well?"));
    information = new QLabel(tr("All NDN operations will become unavailable."));

    confirmButton = new QPushButton(tr("Yes"));
    noButton = new QPushButton(tr("No"));
    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setDefault(true);

    buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(noButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(confirmButton, QDialogButtonBox::ActionRole);

    connect(confirmButton,SIGNAL(pressed()), parent, SLOT(terminateDaemonAndClose()));
    connect(noButton,SIGNAL(pressed()), qApp, SLOT(quit()));
    connect(cancelButton,SIGNAL(pressed()), this, SLOT(hide()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(question);
    layout->addWidget(information);
    layout->addWidget(buttonBox);

    setLayout(layout);

    setWindowTitle(tr("NDNx Control Center"));
}

#if WAF
#include "quit-dialog.moc"
#include "quit-dialog.cpp.moc"
#endif
