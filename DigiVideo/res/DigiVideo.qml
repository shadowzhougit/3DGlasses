import QtQuick 2.5
import DigiVideo 1.0

Rectangle {
    anchors.fill: parent
    DigiVideo {
        videoWidth : 1024
        videoHeight: 768
        width: 1024
        height: 768
        videoPath: "rtsp://192.168.0.103/live/test"
    }
}
