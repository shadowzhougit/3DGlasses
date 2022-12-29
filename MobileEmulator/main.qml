import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.2
Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Column {
        Text {
            id: receivedLbl
            text: qsTr("text")
        }

        Button {
            id: btnConnect
            text: "connect"
            onClicked: {
                tcpSocket.connectToServer(udpSocket.getServerIP())
            }
        }

        Button {
            id: btnSwitch
            text: "Switch"
            onClicked: {
                tcpSocket.sendData("0, switch, 0\n")
            }
        }

        Button {
            id: btnPush
            text: "Push"
            onClicked: {
                tcpSocket.sendData("1, push, 'just a test push message'\n")
            }
        }

        Button {
            id: btnPull
            text: "Pull"
            onClicked: {
                tcpSocket.sendData("2, pull, 'just a test pull message'\n")
            }
        }
    }
    Connections {
        target: udpSocket
        function onRecvDataSignal(data) {
            console.log("rece:" + data)
            var currentDate = new Date()
            receivedLbl.text = data + udpSocket.getServerIP() + currentDate.toLocaleString(Qt.locale("de_DE"), "yyyy-MM-dd HH:mm:ss")
        }
    }
}
