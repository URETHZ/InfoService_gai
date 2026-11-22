import QtQuick
import QtQuick.Controls

MenuItem {
    id: control
    implicitHeight: 34

    contentItem: Text {
        text: control.text
        color: control.enabled ? "white" : "#808080"
        font.pixelSize: 15
        verticalAlignment: Text.AlignVCenter
        leftPadding: 6
    }


    // стрелка у пунктов с подменю
    arrow: Text {
        x: control.width - width - 10
        anchors.verticalCenter: parent.verticalCenter
        visible: control.subMenu !== null
        text: "›"
        color: "white"
        font.pixelSize: 18
    }

    background: Rectangle {
        color: control.highlighted ? "#0056b3" : "transparent"
        radius: 4
    }
}