import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Project_GAI_information_system

Page {
    id: generalFrame
    Component.onCompleted: {
            mainFrame.title = "АИС ГАИ - Главное меню"
        }

    MenuBar {
        width: parent.width

        Repeater {
            model: Authorization.menuTree

            delegate: MenuNode {
                nodeData: modelData
            }
        }
    }
    // Боковая панель с иконками
    Rectangle {

    }
    // Панель уведомлений
    Item {

    }
    // Центрольная панель
    StackView {
        id: generalStack
        anchors.fill: parent
        initialItem: Rectangle {
            color: "#202020"
            Text {
                anchors.centerIn: parent
                text: "Добро пожаловать!"
                font.Cursive: true
                font.pixelSize: 16

            }
        }
    }

    // Строка состояния
    Rectangle {

    }

}