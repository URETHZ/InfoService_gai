import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes
import Gai

Page {
    id: defPage
    Rectangle{
        anchors.fill: parent
        color: "#202020"
        Image {
            id: defIcon
            anchors.centerIn: parent
            width: Math.min(parent.width, parent.height) * 0.35
            height: width
            source: "../images/icons/logo_gray.svg"
            fillMode: Image.PreserveAspectFit
        }
    }
}
