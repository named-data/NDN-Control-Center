/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include <QDialog>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>

#ifndef QUITDIALOG_H
#define QUITDIALOG_H

class QuitDialog : public QDialog
{
    Q_OBJECT

public:
    QuitDialog(QWidget *parent = 0);

private:
    QLabel *question;
    QLabel *information;
    QDialogButtonBox *buttonBox;
    QPushButton *confirmButton;
    QPushButton *noButton;
    QPushButton *cancelButton;
};

#endif // QUITDIALOG_H
