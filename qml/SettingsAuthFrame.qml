import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes
import Project_GAI_information_system

// Страница с настройками авторизации
Page {
    id: settingsFrame
    StackView.onActivating: {
            mainFrame.title = "Настройки"
            mainFrame.minimumHeight = 250
            mainFrame.minimumWidth = 450
        }
    Rectangle {
        color: '#1e1e24'
        anchors.fill: parent
        RowLayout {
            id: leftPan
            anchors.fill: parent
            spacing: 4
            // Панель настроек
            Item {
                id: valPan
                Layout.fillHeight: true
                Layout.preferredWidth: settingsFrame.width * 0.55

                GridLayout {
                    id: settingsGrid
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    columns: 2
                    rows: 4
                    columnSpacing: 10
                    rowSpacing: 10

                    // Строка 1: заголовок на обе колонки
                    Text {
                        Layout.columnSpan: 2
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40
                        text: "Настройки подключения"
                        color: "white"
                        font.pixelSize: 18
                        font.bold: true
                        verticalAlignment: Text.AlignVCenter
                    }

                    // Строка 2: IP
                    Text {
                        Layout.fillWidth: true
                        Layout.preferredWidth: settingsGrid.width * 0.4
                        Layout.preferredHeight: 40
                        text: "IP-адрес"
                        color: "white"
                        font.pixelSize: 18
                        verticalAlignment: Text.AlignVCenter
                    }
                    TextField {
                        id: ip_address_field
                        Layout.fillWidth: true
                        Layout.preferredWidth: settingsGrid.width * 0.6
                        Layout.preferredHeight: 40
                        font.pixelSize: 18
                        maximumLength: 15
                        text: AppSettings.serverIp
                        onEditingFinished: AppSettings.serverIp = text
                        verticalAlignment: TextInput.AlignVCenter

                        background: Rectangle {
                            color: "#2c2d30"
                            radius: 7
                            border.color: "#4d4e4f"
                            border.width: 1
                        }
                    }

                    // Строка 3: порт
                    Text {
                        Layout.fillWidth: true
                        Layout.preferredWidth: settingsGrid.width * 0.4
                        Layout.preferredHeight: 40
                        text: "Port"
                        color: "white"
                        font.pixelSize: 18
                        verticalAlignment: Text.AlignVCenter
                    }
                    TextField {
                        id: port_field
                        Layout.fillWidth: true
                        Layout.preferredWidth: settingsGrid.width * 0.6
                        Layout.preferredHeight: 40
                        font.pixelSize: 18
                        maximumLength: 5
                        text: AppSettings.serverPort
                        validator: IntValidator { bottom: 1; top: 65535 }
                        onEditingFinished: AppSettings.serverPort = parseInt(text)
                        verticalAlignment: TextInput.AlignVCenter

                        background: Rectangle {
                            color: "#2c2d30"
                            radius: 7
                            border.color: "#4d4e4f"
                            border.width: 1
                        }
                    }
                    Button {
                        id: saveByn
                        text: "Сохранить"
                        Layout.columnSpan: 2
                        Layout.preferredWidth: valPan.width * 0.4
                        Layout.preferredHeight: 35
                        Layout.alignment: Qt.AlignCenter
                        font.pixelSize: 18

                        background: Rectangle {
                            color: saveByn.down ? "#2c2d30" : "#0056b3"
                            radius: 10
                        }
                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            font: parent.font
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: {
                            AppSettings.serverIp = ip_address_field.text
                            AppSettings.serverPort = port_field.text
                            AppSettings.loginImagePath = imageSwitcher.images[imageSwitcher.currentIndex]
                            AppSettings.save()
                        }
                    }
                }
            }

            // Разделитльная стенка
            Rectangle {
                Layout.fillHeight: true
                Layout.preferredWidth: 5
                color: "#202020"
            }

            // Интерактивный переключатель иконок
            Rectangle {
                id: imageSwitcher
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "transparent"

                readonly property var images: Array.from({ length: 7 },
                    (_, i) => "images/authFrameImage/i" + (i + 1) + ".svg")
                property int currentIndex: 0

                function next() { currentIndex = (currentIndex + 1) % images.length }
                function prev() { currentIndex = (currentIndex - 1 + images.length) % images.length }

                // При открытии выбираем ту картинку, что сохранена в настройках
                Component.onCompleted: {
                    let i = images.indexOf(AppSettings.loginImagePath)
                    if (i >= 0) currentIndex = i
                }

                // Запоминаем выбор (в файл он попадёт по кнопке "Сохранить")
                onCurrentIndexChanged: AppSettings.loginImagePath = images[currentIndex]

                // Кнопка влево
                Image {
                    id: leftNext
                    width: 30
                    height: 30
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    source: "images/icons/next_icon.svg"
                    fillMode: Image.PreserveAspectFit
                    mirror: true
                    scale: leftHover.hovered ? 1.1 : 1.0
                    Behavior on scale {
                        NumberAnimation { duration: 200; easing.type: Easing.OutBack }
                    }

                    HoverHandler { id: leftHover; cursorShape: Qt.PointingHandCursor }
                    TapHandler { onTapped: imageSwitcher.prev() }
                }

                // Картинка
                Image {
                    id: preview
                    anchors.left: leftNext.right
                    anchors.right: rightNext.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.margins: 10
                    source: imageSwitcher.images[imageSwitcher.currentIndex]
                    fillMode: Image.PreserveAspectFit
                }

                // Кнопка вправо
                Image {
                    id: rightNext
                    width: 30
                    height: 30
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    source: "images/icons/next_icon.svg"
                    fillMode: Image.PreserveAspectFit
                    scale: rightHover.hovered ? 1.1 : 1.0
                    Behavior on scale {
                        NumberAnimation { duration: 200; easing.type: Easing.OutBack }
                    }

                    HoverHandler { id: rightHover; cursorShape: Qt.PointingHandCursor }
                    TapHandler { onTapped: imageSwitcher.next() }
                }
            }
        }
        // Кнопка назад
        Item {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 10
            width: 30
            height: 30

            HoverHandler {
                id: backBtn
            }

            Image {
                id: backIcon
                source: "images/icons/back_icon.svg"
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                scale: backBtn.hovered ? 1.1 : 1.0
                Behavior on scale {
                    NumberAnimation { duration: 200; easing.type: Easing.OutBack }
                }
                Behavior on opacity {
                    NumberAnimation { duration: 200; easing.type: Easing.OutBack }
                }
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    mainStack.pop()
                }
            }
        }
    }
}
