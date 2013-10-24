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
#include <QLineEdit>
#include <QComboBox>
#include <QString>

#ifndef FIBINPUTDIALOG_H
#define FIBINPUTDIALOG_H

class FibInputDialog : public QDialog
{
    Q_OBJECT

public:
    FibInputDialog(QWidget *parent = 0);
    QString getPrefixName();
    QString getEndpoint();
    QString getTunnelType();

private:
    QLabel *prefixLabel;
    QLineEdit *prefixTextField;

    QLabel *tunnelLabel;
    QComboBox *tunnelComboBox;

    QLabel *endpointLabel;
    QLineEdit *endpointTextField;

    QDialogButtonBox *buttonBox;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // FIBINPUTDIALOG_H
