import QtQuick
import QtQuick.Controls 2.15

import Client.Frame

Window {
    id: clientRealScreen
    width: 1920 / 2
    height: 1080 / 2
    visible: true
    title: qsTr("Client - Real Screen")

    Rectangle {
        id: virtualScreenContainer
        width:  (1920 / 4) + 6          // +6 for border
        height: (1080 / 4) + 6 + 20     // +20 for header
        anchors.centerIn: parent
        color: "#282828"
        radius: 4

        Text {
            id: virtualScreenContainerTitle
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: (20 - height) / 2    // Vertically centered in header
            color: "white"
            text: "Client - Remote Render"
            font.family: "Helvetica"
        }

        FrameStreamView {
            id: remoteRender
            width: 1920 / 4     // Should match clients virtual screen width
            height: 1080 / 4    // Should match clients virtual screen height
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
            frame: FrameStream.frame
        }
    }
}
