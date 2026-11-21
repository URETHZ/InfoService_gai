import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: mainFrame
    width: 700
    height: 500
    visible: true
    title: "АИС ГАИ"
    color: "#1e1e24"

    // Стек экранов
    StackView {
        id: mainStack
        anchors.fill: parent

        // При запуске программы показываем экран логина
        initialItem: "LoginScreen.qml"
    }
}
