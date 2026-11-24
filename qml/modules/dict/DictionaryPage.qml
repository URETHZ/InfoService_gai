import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    anchors.fill: true
    property var controller
    property string pageTitle: "Справочник"

    header: ToolBar {
        RowLayout {
            Label { text: pageTitle; font.bold: true }
            Item { Layout.fillWidth: true }
            Button { text: "Добавить"; onClicked: controller.addNew() }
            Button { text: "Удалить"; onClicked: controller.deleteCurrent() }
        }
    }
    // Таблица для отображения данных
    TableView {
        anchors.fill: parent
        model: controller.model // Модель данных из C++

        // Колонки можно настраивать, но QSqlTableModel сама подтянет их из БД
        delegate: Rectangle {
            implicitWidth: 150
            implicitHeight: 30
            border.color: "#333"
            Text {
                text: display // QML автоматически берет данные из модели
                anchors.centerIn: parent
                color: "white"
            }
        }
    }
}