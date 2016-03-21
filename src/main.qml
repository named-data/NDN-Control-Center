import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: false
    id: window
    title: "NFD Control Center"
    minimumWidth: 600
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
                            enabled: false
                            text: "Automatically start NFD Control Center on login"
                        }
                        CheckBox {
                            id: discoverHub
                            text: "Discover nearest NDN hub"
                            onCheckedChanged: {
                                if (this.checked) {
                                    trayModel.autoConfig()
                                }
                            }
                        }
                        // CheckBox {
                        //     id: checkUpdate
                        //     enabled: false
                        //     text: "Check for software updates"
                        // }
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
                Button {
                    id: startStopButton
                    text: startStopButtonText

                    anchors.top: status.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 20
                    anchors.leftMargin: 20
                    anchors.rightMargin: 20
                    anchors.bottomMargin: 20

                    onClicked: trayModel.startStopNfd()
                }
            }
        }
        // Tab {
        //     title: "FIB status"
        //     TableView {
        //         anchors.fill: parent
        //         anchors.topMargin: 20
        //         anchors.bottomMargin: 20
        //         anchors.leftMargin: 20
        //         anchors.rightMargin: 20
        //         TableViewColumn{
        //             role: "prefix"
        //             title: "NDN prefix"
        //             width: 300
        //         }
        //         TableViewColumn{
        //             role: "faceId"
        //             title: "Face ID"
        //             width: 50
        //         }
        //         TableViewColumn{
        //             role: "cost"
        //             title: "Cost"
        //             width: 50
        //         }
        //         model: fibModel
        //     }
        // }
        Tab {
            title: "Forwarder status"
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
            title: "Security"
            Column {
                spacing: 2
                anchors.fill: parent
                anchors.topMargin: 20
                anchors.bottomMargin: 20
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                Button {
                    text: "Obtain NDN Certificate"
                    onClicked: Qt.openUrlExternally('http://ndncert.named-data.net')
                }
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
    // Timer {
    //     interval: 1000; running: true; repeat: true
    //     onTriggered: {
    //         fibModel.fetchFibInformation()
    //     }
    // }
}
