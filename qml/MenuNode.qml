import QtQuick
import QtQuick.Controls
import Gai

Menu {
    id: root
    property var nodeData: null


    // Поднимается вверх по дереву до General.qml
    signal itemTriggered(var node)

    title: nodeData ? nodeData.name : ""

    readonly property Component itemComp: Component {
        StyledMenuItem {
            property var node: null
            text: node ? node.name : ""
            onTriggered: root.itemTriggered(node)
        }
    }

    background: Rectangle {
            implicitWidth: 240
            color: "#2c2d30"
            border.color: "#4d4e4f"
            radius: 4
        }
        delegate: StyledMenuItem {}

    Component.onCompleted: {
        if (!nodeData || !nodeData.childrens)
            return
        const subComp = Qt.createComponent("MenuNode.qml")
        if (subComp.status !== Component.Ready)
            console.error(subComp.errorString())

        for (const child of nodeData.childrens) {
            if(Authorization.haveRightsToModule(child.menu_id, 1)) {
                if (child.childrens && child.childrens.length > 0) {
                    const sub = subComp.createObject(root, { nodeData: child })
                    sub.itemTriggered.connect(root.itemTriggered)
                    root.addMenu(sub)
                } else {
                    root.addItem(itemComp.createObject(root, { node: child }))
                }
            }
        }


    }
}