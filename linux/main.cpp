/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#include <QtGui/QApplication>
#include "traymenu.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TrayMenu w;
    
    return a.exec();
}
