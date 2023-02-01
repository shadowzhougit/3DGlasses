import QtQuick 2.5
import QtQuick.Controls 2.5

ApplicationWindow {
    id: main
    width: 1024
    height: 768
    visible: true

    property real multiplierH: (main.height/640)*1000
    property real multiplierW: (main.width/360)*1000

    function dpH(numbers) {
        return numbers*multiplierH/1000
    }

    function dpW(numbers) {
        return numbers*multiplierW/1000
    }

    function dpX(numbers){
        return (dpW(numbers)+dpH(numbers))/2
    }

    property bool isPlaying: false
    Rectangle{
        width: parent.width
        height: dpW(35)
        color: "lightblue"
        Text{
            id:btn_text
            visible: true
            anchors.centerIn: parent
            text: isPlaying ? qsTr("停止播放") : qsTr("开始播放")
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
               playVideo()
            }
        }
    }

    StackView {
        anchors.fill: parent
        id:stackView
        initialItem: Rectangle {
        }
    }

    function playVideo() {
        if (isPlaying) {
            stackView.pop()
        } else {
            stackView.push("qrc:/res/DigiVideo.qml")
        }
        isPlaying = !isPlaying
    }

    Connections {
        target: engine
        function onStartPlay() {
            playVideo()
        }
    }
}
