import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: false
    id: window
    title: "NDN Control Center"
    minimumWidth: 700
    minimumHeight: 400

    TabView {
        anchors.fill: parent
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        Tab {
            title: "General"
            ColumnLayout {
                anchors.fill: parent
                GroupBox {
                    title: "Basic"
                    id: checkboxControl
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 20
                    anchors.leftMargin: 20
                    anchors.rightMargin: 20
                    anchors.bottomMargin: 20
                    Column {
                        spacing: 5
                        anchors.fill: parent
                        anchors.topMargin: 10
                        anchors.bottomMargin: 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        CheckBox {
                            id: startOnLogin
                            text: "Launch NDN Control Center on login"
                            checked: trayModel.isNccAutoStartEnabled()
                            onCheckedChanged: trayModel.enableDisableNccAutoStart(this.checked)
                        }
                        CheckBox {
                            id: autoStart
                            text: "Automatically start NFD"
                            checked: trayModel.isNfdAutoStartEnabled()
                            onCheckedChanged: trayModel.enableDisableNfdAutoStart(this.checked)
                        }
                        CheckBox {
                            id: autoConfig
                            text: "Automatically start NDN auto configuration"
                            checked: trayModel.isNdnAutoConfigEnabled()
                            onCheckedChanged: trayModel.enableDisableNdnAutoConfig(this.checked)
                        }
                        CheckBox {
                            id: shutdownOnQuit
                            text: "Shutdown NFD daemon on quit"
                            checked: trayModel.isNfdStopOnExitEnabled()
                            onCheckedChanged: trayModel.enableDisableNfdStopOnExit(this.checked)
                        }
                    }
                }
                GroupBox {
                    title: "Status"
                    id: status
                    anchors.top: checkboxControl.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 20
                    anchors.leftMargin: 20
                    anchors.rightMargin: 20
                    anchors.bottomMargin: 20
                    Row {
                        spacing: 20
                        anchors.topMargin: 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 10
                        anchors.fill: parent
                        Button {
                            text: "Traffic map"
                            onClicked: Qt.openUrlExternally('http://ndnmap.arl.wustl.edu')
                        }
                        Button {
                            text: "Routing status"
                            onClicked: Qt.openUrlExternally('http://netlab.cs.memphis.edu/script/htm/status.htm')
                        }
                    }
                }
                GridLayout {
                     anchors.left: parent.left
                     anchors.bottom: parent.bottom
                     Label {
                         text: "NDN Control Center version:"
                         font.pixelSize: 10
                     }
                     Label {
                         text: nccVersion
                         font.pixelSize: 10
                         font.bold: true
                     }
                }
            }
        }
        Tab {
            title: "Auto-config"
            TextArea {
                id: ndnAutoConfigTextId
                anchors.fill: parent
                anchors.topMargin: 20
                anchors.bottomMargin: 20
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                readOnly: true
                text: ndnAutoConfigText
            }
        }
        Tab {
            title: "Add/Delete Route"
            Column {
                spacing: 2
                anchors.fill: parent
                anchors.topMargin: 20
                anchors.bottomMargin: 20
                anchors.leftMargin: 20
            }
            Row {
                spacing: 20
                anchors.topMargin: 30
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                anchors.bottomMargin: 10
                anchors.fill: parent
                TextField {
                          focus: true
                }
                Button {
                    text: "Add Route"
                    onClicked: trayModel.addRoute(TextField.text);
                }
                Button {
                    text: "Delete Route"
                    onClicked: trayModel.deleteRoute(TextField.text);
                }
            }
        }
    }
    Connections {
        target: trayModel;
        onShowApp: {
            window.show()
            window.raise()
        }
    }
}
