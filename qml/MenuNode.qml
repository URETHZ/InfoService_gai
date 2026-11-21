import QtQuick
import QtQuick.Controls

Menu {
    id: root
    property var nodeData: null

    // Поднимается вверх по дереву до General.qml
    signal itemTriggered(var node)

    title: nodeData ? nodeData.name : ""

    readonly property Component itemComp: Component {
        MenuItem {
            property var node: null
            text: node ? node.name : ""
            onTriggered: root.itemTriggered(node)
        }
    }

    Component.onCompleted: {
        if (!nodeData || !nodeData.childrens)
            return

        const subComp = Qt.createComponent("MenuNode.qml")
        if (subComp.status !== Component.Ready)
            console.error(subComp.errorString())

        for (const child of nodeData.childrens) {
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