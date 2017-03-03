/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2013-2017, Regents of the University of California,
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
#include <QtQml/QQmlContext>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>

#include "tray-menu.hpp"

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/name.hpp>
#include <ndn-cxx/util/scheduler.hpp>
#include <ndn-cxx/mgmt/nfd/fib-entry.hpp>
#include <ndn-cxx/mgmt/nfd/rib-entry.hpp>
#include <ndn-cxx/mgmt/nfd/controller.hpp>
#include <ndn-cxx/mgmt/nfd/status-dataset.hpp>

#include <boost/thread.hpp>

namespace ndn {

class Ncc
{
public:
  Ncc()
    : m_isActive(true)
    , m_localhopFibEntry(Name("/localhop/nfd"))
    , m_face(nullptr, m_keyChain)
    , m_controller(m_face, m_keyChain)
    , m_scheduler(m_face.getIoService())
    , m_tray(m_engine.rootContext(), m_face, m_keyChain)
  {
    QQmlContext* context = m_engine.rootContext();

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
            requestNfdStatus();
            m_face.processEvents(time::milliseconds::zero(), true);
          }
        }
        catch (const std::exception& e) {
          emit m_tray.nfdActivityUpdate(false);
          m_face.shutdown();
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
  onStatusRetrieved(const nfd::ForwarderStatus& status)
  {
    emit m_tray.nfdActivityUpdate(true);

    m_controller.fetch<ndn::nfd::FibDataset>(bind(&Ncc::onFibStatusRetrieved, this, _1),
                                             bind(&Ncc::onStatusTimeout, this));

    m_scheduler.scheduleEvent(time::seconds(6), bind(&Ncc::requestNfdStatus, this));
  }

  void
  onFibStatusRetrieved(const std::vector<nfd::FibEntry>& status)
  {
    bool isConnectedToHub = false;
    for (auto const& fibEntry : status) {
      if (fibEntry.getPrefix() == m_localhopFibEntry) {
        isConnectedToHub = true;
      }
    }
    emit m_tray.connectivityUpdate(isConnectedToHub);
  }

  void
  onStatusTimeout()
  {
    emit m_tray.nfdActivityUpdate(false);

    std::cerr << "Should not really happen, most likely a serious problem" << std::endl;
    m_scheduler.scheduleEvent(time::seconds(6), bind(&Ncc::requestNfdStatus, this));
  }

  void
  stop()
  {
    m_isActive = false;
    m_face.shutdown();
    m_scheduler.cancelAllEvents();
    m_nfdThread.interrupt();
    m_nfdThread.join();
  }

private:
  void
  requestNfdStatus()
  {
    m_controller.fetch<ndn::nfd::ForwarderGeneralStatusDataset>(bind(&Ncc::onStatusRetrieved, this, _1),
                                                                bind(&Ncc::onStatusTimeout, this));
  }

private:
  volatile bool m_isActive;
  boost::thread m_nfdThread;
  const Name m_localhopFibEntry;

  KeyChain m_keyChain;
  Face m_face;
  nfd::Controller m_controller;
  Scheduler m_scheduler;

  QQmlApplicationEngine m_engine;

  ncc::TrayMenu m_tray;
};

} // namespace ndn

Q_DECLARE_METATYPE(ndn::shared_ptr<const ndn::Data>)
Q_DECLARE_METATYPE(ndn::nfd::ForwarderStatus)
Q_DECLARE_METATYPE(std::vector<ndn::nfd::FibEntry>)
Q_DECLARE_METATYPE(std::vector<ndn::nfd::RibEntry>)
Q_DECLARE_METATYPE(std::vector<ndn::nfd::FaceStatus>)

int
main(int argc, char *argv[])
{
  qRegisterMetaType<ndn::shared_ptr<const ndn::Data>>();
  qRegisterMetaType<ndn::nfd::ForwarderStatus>();
  qRegisterMetaType<std::vector<ndn::nfd::FibEntry>>();
  qRegisterMetaType<std::vector<ndn::nfd::RibEntry>>();
  qRegisterMetaType<std::vector<ndn::nfd::FaceStatus>>();

  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QApplication app(argc, argv);

  ndn::Ncc controlCenterGui;

  QApplication::setQuitOnLastWindowClosed(false);
  int retval = app.exec();

  controlCenterGui.stop();

  return retval;
}
