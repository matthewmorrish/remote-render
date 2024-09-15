import QtQuick
import QtQuick.Controls 2.15

Window {
    id: realScreen
    width: 1920 / 2
    height: 1080 / 2
    visible: true
    title: qsTr("Daemon - Real Screen")

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
            text: "Daemon - Virtual Screen"
            font.family: "Helvetica"
        }

        Rectangle {
            id: virtualScreen
            width: 1920 / 4
            height: 1080 / 4
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
            radius: 3

            property bool _clicked: false
            color: _clicked ? "blue" : "red"

            Button {
                id: button
                width: parent.width / 4
                height: parent.height / 4
                anchors.centerIn: parent
                text: "Click Me!"
                onClicked: {
                    parent._clicked = !parent._clicked
                }
            }
        }
    }
}
