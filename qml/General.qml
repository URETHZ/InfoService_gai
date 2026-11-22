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
            if(Authorization.haveRightsToModule(node.menu_id, 1)) {
                const menu = comp.createObject(menuBar, { nodeData: node })
                menu.itemTriggered.connect(generalFrame.openModule)
                menuBar.addMenu(menu)
            }
        }
    }

    // Сюда придёт клик по любому листу меню
    function openModule(node) {
        console.log("Нажато:", node.name, node.dll_name, node.func_name)
        // дальше: загрузка модуля по node.dll_name / node.func_name
    }

    // Верхняя панель
    Rectangle {
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 30
        color: "#2c2d30"
        z: 10

        // Меню слева
        MenuBar {
            id: menuBar
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            padding: 0
            spacing: 0
            background: Rectangle { color: "transparent" }

            delegate: MenuBarItem {
                id: barItem
                padding: 0
                horizontalPadding: 10
                implicitHeight: 30

                contentItem: Text {
                    text: barItem.text
                    color: "white"
                    font.pixelSize: 15
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: barItem.highlighted ? "#0056b3" : "transparent"
                }
            }
        }

        // Кнопка пользователя справа
        Item {
            id: userBtn
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: userRow.implicitWidth + 24

            Rectangle {
                anchors.fill: parent
                color: (userHover.hovered || userMenu.opened) ? "#0056b3" : "transparent"
            }

            Row {
                id: userRow
                anchors.centerIn: parent
                spacing: 6

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: Authorization.currentLogin !== "" ? Authorization.currentLogin : "Пользователь"
                    color: "white"
                    font.pixelSize: 15
                }
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: userMenu.opened ? "▾" : "▸"
                    color: "white"
                    font.pixelSize: 12
                }
            }

            HoverHandler { id: userHover; cursorShape: Qt.PointingHandCursor }
            TapHandler {
                onTapped: {
                    userMenu.opened ? userMenu.close() : userMenu.open()
                }
            }

            // Меню действий сессии (parent = userBtn, поэтому x и y считаются от кнопки)
            Menu {
                id: userMenu
                y: userBtn.height
                x: userBtn.width - width
                width: 200
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                background: Rectangle {
                    color: "#2c2d30"
                    border.color: "#4d4e4f"
                    radius: 6
                }

                StyledMenuItem {
                    text: "Сменить пользователя"
                    onTriggered: {
                        Authorization.logout()
                        mainStack.replace("LoginScreen.qml")
                    }
                }
                StyledMenuItem {
                    text: "Сменить пароль"
                    onTriggered: mainStack.push("ChangePasswordFrame.qml")
                }
                MenuSeparator {
                    contentItem: Rectangle {
                        implicitHeight: 1
                        color: "#4d4e4f"
                    }
                }
                StyledMenuItem {
                    text: "Выход"
                    onTriggered: Qt.quit()
                }
            }
        }
    }

    // ---------- Рабочая область ----------
    StackView {
        id: generalStack
        anchors.top: topBar.bottom
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