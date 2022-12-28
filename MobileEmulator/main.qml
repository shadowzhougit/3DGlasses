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

        RoundButton {
            id: btnConnect
            onClicked: {
                tcpSocket.connectToServer(udpSocket.getServerIP())
            }
        }

        RoundButton {
            id: btnSend
            onClicked: {
                tcpSocket.sendData(udpSocket.getServerIP())
            }
        }
    }
    Connections {
        target: udpSocket
        function onRecvDataSignal(data) {
            console.log("rece:" + data)
            receivedLbl.text += data
            receivedLbl.text += udpSocket.getServerIP()
        }
    }
}
