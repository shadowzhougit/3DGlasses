import QtQuick 2.5
import DigiVideo 1.0
Rectangle {
    width: dpW(360)
    height: dpH(640)
    DigiVideo {
        anchors.centerIn: parent
        videoWidth :dpW(360)
        videoHeight: dpH(200)
        width: dpW(360)
        height: dpH(200)
        videoPath: "rtsp://47.102.115.26/live/test"
    }
}
