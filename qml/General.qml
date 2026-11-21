import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Project_GAI_information_system

Page {
    id: generalFrame

    Component.onCompleted: {
        mainFrame.title = "АИС ГАИ - Главное меню"
        buildMenu()
    }

    function buildMenu() {
        const comp = Qt.createComponent("MenuNode.qml")
        if (comp.status !== Component.Ready) {
            console.error(comp.errorString())
            return
        }
        for (const node of Authorization.menuTree) {
            const menu = comp.createObject(menuBar, { nodeData: node })
            menu.itemTriggered.connect(generalFrame.openModule)
            menuBar.addMenu(menu)
        }
    }

    // Сюда придёт клик по любому листу меню
    function openModule(node) {
        console.log("Нажато:", node.name, node.dll_name, node.func_name)
        // дальше: загрузка модуля по node.dll_name / node.func_name
    }

    MenuBar {
        id: menuBar
        width: parent.width
    }

    StackView {
        id: generalStack
        anchors.top: menuBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        initialItem: Rectangle {
            color: "#202020"
            Text {
                anchors.centerIn: parent
                text: "Добро пожаловать!"
                font.italic: true
                font.pixelSize: 16
                color: "white"
            }
        }
    }
}