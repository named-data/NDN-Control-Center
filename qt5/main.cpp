/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2013-2014, Regents of the University of California,
 *
 * This file is part of NFD Control Center.  See AUTHORS.md for complete list of NFD
 * authors and contributors.
 *
 * NFD Control Center is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD Control Center is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with NFD
 * Control Center, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtQml/QQmlApplicationEngine>
#include <QtWidgets/QApplication>
#include <QtQml/QQmlContext>

#include "forwarder-status.hpp"
#include "fib-status.hpp"
#include "tray-menu.hpp"

#include <ndn-cxx/face.hpp>
#include <boost/thread.hpp>

namespace ndn {

class Ncc
{
public:
  Ncc()
    : m_isActive(true)
    , m_forwarderModel(m_face)
    , m_fibModel(m_face)
  {
    QQmlContext* context = m_engine.rootContext();

    context->setContextProperty("forwarderModel", &m_forwarderModel);
    context->setContextProperty("fibModel", &m_fibModel);
    context->setContextProperty("trayModel", &m_tray);

    m_engine.load((QUrl("qrc:/main.qml")));

    m_nfdThread = boost::thread(&Ncc::nfdConnectionLoop, this);
  }

  void
  nfdConnectionLoop()
  {
    try {
#ifdef BOOST_THREAD_USES_CHRONO
      time::seconds retryTimeout = time::seconds(5);
#else
      boost::posix_time::time_duration retryTimeout = boost::posix_time::seconds(1);
#endif

      while (m_isActive) {
        try {
          while (m_isActive) {
            m_face.expressInterest(Interest("/localhost/nfd/status"),
                                   bind(&Ncc::onStatusRetrieved, this),
                                   bind(&Ncc::onStatusTimeout, this));
            m_face.processEvents(time::milliseconds::zero(), true);
          }
        }
        catch (const std::exception&) {
          emit m_tray.nfdActivityUpdate(false);
#ifdef BOOST_THREAD_USES_CHRONO
          boost::this_thread::sleep_for(retryTimeout);
#else
          boost::this_thread::sleep(retryTimeout);
#endif
        }
      }
    }
    catch (const boost::thread_interrupted&) {
      // done
    }
  }

  void
  onStatusRetrieved()
  {
    emit m_tray.nfdActivityUpdate(true);
  }

  void
  onStatusTimeout()
  {
    emit m_tray.nfdActivityUpdate(false);

    std::cerr << "Should not really happen, most likely a serious problem" << std::endl;
    m_face.expressInterest(Interest("/localhost/nfd/status"),
                           bind(&Ncc::onStatusRetrieved, this),
                           bind(&Ncc::onStatusTimeout, this));
  }

  void
  stop()
  {
    m_isActive = false;
    m_face.shutdown();
    m_nfdThread.interrupt();
    m_nfdThread.join();
  }

private:
  volatile bool m_isActive;
  boost::thread m_nfdThread;

  Face m_face;

  QQmlApplicationEngine m_engine;

  ForwarderStatusModel m_forwarderModel;
  FibStatusModel m_fibModel;
  TrayMenu m_tray;
};

} // namespace ndn

Q_DECLARE_METATYPE(ndn::shared_ptr<const ndn::Data>)

int
main(int argc, char *argv[])
{
  qRegisterMetaType<ndn::shared_ptr<const ndn::Data> >();

  QApplication app(argc, argv);

  ndn::Ncc controlCenterGui;

  QApplication::setQuitOnLastWindowClosed(false);
  int retval = app.exec();

  controlCenterGui.stop();

  return retval;
}
