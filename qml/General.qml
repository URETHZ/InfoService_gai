import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: window
    width: 700
    height: 500
    visible: true
    title: "Контроль и учет ГАИ"
    Component.onCompleted: {
            mainFrame.title = "АИС ГАИ - Главное меню"
        }
    Rectangle {
        anchors.centerIn: parent
        Text {
            text: "Победа!"
        }
    }
}