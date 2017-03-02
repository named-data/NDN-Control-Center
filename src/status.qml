import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: false
    id: window
    title: "NFD Status"
    minimumWidth: 700
    minimumHeight: 400

    TabView {
        anchors.fill: parent
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        Tab {
            title: "Forwarder"
            TableView {
                anchors.fill: parent
                anchors.topMargin: 20
                anchors.bottomMargin: 20
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                model: forwarderModel
                TableViewColumn{
                    role: "type"
                    title: "Type"
                    width: 200
                }
                TableViewColumn{
                    role: "value"
                    title: "Value"
                    width: 300
                }
            }
        }
        Tab {
            title: "FIB"
            TableView {
                anchors.fill: parent
                anchors.topMargin: 20
                anchors.bottomMargin: 20
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                model: fibModel
                TableViewColumn{
                    role: "prefix"
                    title: "NDN prefix"
                    width: 300
                }
                TableViewColumn{
                    role: "faceId"
                    title: "Face ID"
                    width: 100
                }
                TableViewColumn{
                    role: "cost"
                    title: "Cost"
                    width: 100
                }
            }
        }
        Tab {
            title: "RIB"
            TableView {
                anchors.fill: parent
                anchors.topMargin: 20
                anchors.bottomMargin: 20
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                model: ribModel
                TableViewColumn{
                    role: "prefix"
                    title: "NDN prefix"
                    width: 200
                }
                TableViewColumn{
                    role: "faceId"
                    title: "Face ID"
                    width: 65
                }
                TableViewColumn{
                    role: "origin"
                    title: "Origin"
                    width: 50
                }
                TableViewColumn{
                    role: "cost"
                    title: "Cost"
                    width: 50
                }
                TableViewColumn{
                    role: "childinherit"
                    title: "ChildInherit"
                    width: 90
                }
                TableViewColumn{
                    role: "ribcapture"
                    title: "RibCapture"
                    width: 90
                }
                TableViewColumn{
                    role: "expiresin"
                    title: "Expires in"
                    width: 90
                }
            }
        }
    }
    Connections {
        target: statusViewer;
        onShowStatus: {
            window.show()
            window.raise()
        }
    }
}
