import QtQuick
import QtQuick.Controls

Loader {
    id: loader
    property var nodeData: null // Данные текущего пункта

    // Выбираем, что рисовать: Menu (если есть дети) или MenuItem (если это лист)
    sourceComponent: (nodeData && nodeData.childrens && nodeData.childrens.length > 0)
                     ? submenuComp
                     : itemComp

    // Компонент для папки (с подменю)
    Component {
        id: submenuComp
        Menu {
            title: nodeData.name

            // РЕКУРСИЯ: перебираем детей и снова вызываем MenuNode
            Repeater {
                model: nodeData.childrens
                delegate: MenuNode {
                    nodeData: modelData // Передаем данные ребенка
                }
            }
        }
    }

    // Компонент для конечного пункта (без детей)
    Component {
        id: itemComp
        MenuItem {
            text: nodeData.name

            // Ловим клик
            onTriggered: {
                console.log("Нажато:", nodeData.name)
                // auth.loadModule(nodeData.dll_name, nodeData.func_name)
            }
        }
    }
}
