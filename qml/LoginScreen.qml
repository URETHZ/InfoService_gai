import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes
import Project_GAI_information_system

// Страница авторизации
Page {
    id: authFrame

    // Первое подключение - когда включаем приложение
    Component.onCompleted:  {
        mainFrame.title = "АИС ГАИ - Авторизация"
        mainFrame.minimumHeight = 400
        mainFrame.minimumWidth = 350
        statusConnFrame.visible = true
        statusConnFrame.setText = "Подключение..."
        Authorization.initConnectionAsync(AppSettings.serverIp, AppSettings.serverPort)
    }

    // Когда возвращаемся из настроек
    StackView.onActivating: {
        mainFrame.title = "АИС ГАИ - Авторизация"
        mainFrame.minimumHeight = 400
        mainFrame.minimumWidth = 350

        statusConnFrame.visible = true
        statusConnFrame.setText = "Подключение..."
        Authorization.initConnectionAsync(AppSettings.serverIp, AppSettings.serverPort)
    }

    Timer {
        id: hideStatusTimer
        interval: 3000
        repeat: false
        onTriggered: statusConnFrame.visible = false
    }

    Connections {
        target: Authorization
        property bool firstQuery: false
        property bool secondQuery: false

        function onConnectionSuccess() {
            statusConnFrame.setText = "Подключено"
            statusConnFrame.isOnline = true
            hideStatusTimer.restart()
        }

        function onConnectionFailed(error) {
            statusConnFrame.setText = error + " Переподключение..."
            statusConnFrame.isOnline = false
            statusConnFrame.visible = true
            Authorization.initConnectionAsync(AppSettings.serverIp, AppSettings.serverPort);
        }

        function onQueryFinished(requestId, data) {
            switch (requestId)
            {
            case 1000:
                var hashFromDb = data?.[0]?.password ?? ""
                loginBtn.isProcessing = false
                switch (Authorization.authorize(password.text, hashFromDb))
                {
                case 0:
                    // Загружаем описание меню
                    Authorization.takeMenuList(500)
                    // Загружаем права доступа.
                    Authorization.takeRights(400, {":login" : login.text} )
                    break
                case 1:
                    errorLabel.text = "Неверный логин"
                    errorLabel.visible = true
                    break
                case 2:
                    errorLabel.text = "Неверный пароль"
                    errorLabel.visible = true
                    forgotPasswordLink.visible = true
                    break
                case 3:
                    errorLabel.text = "Учетная запись заблокирована"
                    errorLabel.visible = true
                    break
                case 4:
                    errorLabel.text = "Ошибка сервера"
                    errorLabel.visible = true
                    break
                default:
                    errorLabel.text = "Неизвестная ошибка авторизации"
                    errorLabel.visible = true
                    break
                }
            break
            case 500:
                Authorization.setMenuList(data)
                firstQuery = true
                if (secondQuery){
                    mainStack.replace("General.qml")
                }

                break
            case 400:
                Authorization.setRights(data)
                secondQuery = true
                if (firstQuery){
                    mainStack.replace("General.qml")
                }
                break
            }
        }
        function onQueryError(requestId, error) {
            errorLabel.text = "["+requestId+"]: " + error
            loginBtn.isProcessing = false
        }

    }
    RowLayout {
        anchors.fill: parent
        spacing: 0
        // ЛЕВАЯ ЧАСТЬ
        Rectangle {
            id: leftPanel
            Layout.fillHeight: true
            Layout.preferredWidth: (authFrame.width < 600 || authFrame.height < 350) ? 0 : authFrame.width * 0.6
            color: '#060617'
            clip: true

            Rectangle {
                id: rotatingBox
                width: parent.width * 1.7
                height: parent.height * 1.7
                anchors.centerIn: parent
                color: "transparent"

                // Эти значения всегда от 0.0 до 1.0
                property real cX_percent: 0.3
                property real cY_percent: 0.6
                property real fX_percent: 0.3
                property real fY_percent: 0.6

                Shape {
                    anchors.fill: parent
                    ShapePath {

                        fillGradient: RadialGradient {
                            centerX: rotatingBox.width * rotatingBox.cX_percent
                            centerY: rotatingBox.height * rotatingBox.cY_percent
                            centerRadius:rotatingBox.width > rotatingBox.height ?
                                             rotatingBox.width : rotatingBox.height
                            focalX: rotatingBox.width * rotatingBox.fX_percent
                            focalY: rotatingBox.height * rotatingBox.fY_percent

                            GradientStop { position: 0.0; color: "#9000ff" }
                            GradientStop { position: 1.0; color: "#00c4ff" }
                        }
                        // Круг рисуем всегда по центру
                        PathAngleArc {
                            property double rXY: rotatingBox.width > rotatingBox.height ?
                                        rotatingBox.width * 0.5 : rotatingBox.height * 0.5
                            centerX: rotatingBox.width * 0.5
                            centerY: rotatingBox.height * 0.5
                            radiusX: rXY
                            radiusY: rXY
                            startAngle: 0
                            sweepAngle: 360
                        }
                    }
                }

                // 1. Вращение (остается без изменений)
                RotationAnimation on rotation {
                    from: 0; to: 360; duration: 6000; loops: Animation.Infinite; running: true
                }

                // 2. Анимация центра X
                SequentialAnimation on cX_percent {
                    loops: Animation.Infinite; running: true
                    NumberAnimation { from: 0.3; to: 0.5; duration: 7000; easing.type: Easing.InOutSine }
                    NumberAnimation { from: 0.5; to: 0.3; duration: 5000; easing.type: Easing.InOutSine }
                }

                // 3. Анимация центра Y
                SequentialAnimation on cY_percent {
                    loops: Animation.Infinite; running: true
                    NumberAnimation { from: 0.6; to: 0.9; duration: 4000; easing.type: Easing.InOutSine }
                    NumberAnimation { from: 0.9; to: 0.2; duration: 8000; easing.type: Easing.InOutSine }
                    NumberAnimation { from: 0.2; to: 0.6; duration: 5000; easing.type: Easing.InOutSine }
                }

                // 4. Анимация фокуса X
                SequentialAnimation on fX_percent {
                    loops: Animation.Infinite; running: true
                    NumberAnimation { from: 0.3; to: 0.5; duration: 7000; easing.type: Easing.InOutSine }
                    NumberAnimation { from: 0.5; to: 0.3; duration: 5000; easing.type: Easing.InOutSine }
                }

                // 5. Анимация фокуса Y
                SequentialAnimation on fY_percent {
                    loops: Animation.Infinite; running: true
                    NumberAnimation { from: 0.6; to: 0.9; duration: 4000; easing.type: Easing.InOutSine }
                    NumberAnimation { from: 0.9; to: 0.2; duration: 8000; easing.type: Easing.InOutSine }
                    NumberAnimation { from: 0.2; to: 0.6; duration: 5000; easing.type: Easing.InOutSine }
                }
            }
            Image {
                id: logoImage
                source: AppSettings.loginImagePath
                width: parent.width < parent.height ? parent.width * 0.8 : parent.height * 0.8
                height: width
                fillMode: Image.PreserveAspectFit

                // Центрируем по горизонтали
                anchors.horizontalCenter: parent.horizontalCenter

                property real floatOffset: 0

                y: (parent.height - logoImage.height) / 2 + floatOffset

                SequentialAnimation on floatOffset {
                    loops: Animation.Infinite
                    running: true
                    NumberAnimation {
                        from: 0; to: -15;
                        duration: 1500; easing.type: Easing.InOutSine
                    }
                    NumberAnimation {
                        from: -15; to: 0;
                        duration: 1500; easing.type: Easing.InOutSine
                    }
                }

                visible: leftPanel.width > 0
            }
            Rectangle {
                width: 4
                height: parent.height
                anchors.right: parent.right // Прижат к правому краю
                visible: leftPanel.width > 0 // Скрываем, если панель исчезла

                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 0.5; color: "#229ED9" }
                    GradientStop { position: 1.0; color: "transparent" }
                }
            }
        }
        // ПРАВАЯ ЧАСТЬ
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: '#1e1e24' // Темный фон

            ColumnLayout {
                id:loginForm
                anchors.centerIn: parent
                Layout.fillWidth: true
                spacing: 25
                property double tfWidht: authFrame.width < 600 ?
                    authFrame.width * 0.75 : authFrame.width * 0.32
                width: tfWidht
                Label {
                    text: "Авторизация"
                    color: "white"
                    Layout.alignment: Qt.AlignHCenter
                    font.pixelSize: 22
                    font.bold: true
                }

                TextField {
                    id: login
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    font.pixelSize: 18
                    maximumLength: 50
                    placeholderText: "login"
                    verticalAlignment: TextInput.AlignVCenter

                    background: Rectangle {
                        color: "#2c2d30"
                        radius: 7
                        border.color: "#4d4e4f"
                        border.width: 1
                    }
                }
                ColumnLayout {
                    Layout.fillWidth: true
                    TextField {
                        id: password
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40
                        maximumLength: 50
                        font.pixelSize: 18
                        placeholderText: "password"
                        verticalAlignment: TextInput.AlignVCenter

                        echoMode: showPassword.checked ?  TextInput.Password: TextInput.Normal

                        background: Rectangle {
                            color: "#2c2d30"
                            radius: 7
                            border.color: "#4d4e4f"
                            border.width: 1
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 5

                        Text {
                            id: errorLabel
                            visible: false
                            text: ""
                            color: "#a62828"
                            font.pixelSize: 16
                            Layout.alignment: Qt.AlignRight
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }


                        Text {
                            id: forgotPasswordLink
                            visible: false
                            text: "Восстановить"
                            color: mouseArea.containsMouse ? "#001847" : "#808080"
                            font.pixelSize: 16
                            font.underline: mouseArea.containsMouse
                            Layout.alignment: Qt.AlignRight

                            Behavior on color {
                                ColorAnimation { duration: 150 }
                            }
                            MouseArea {
                                id: mouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor

                                onClicked: {
                                    // Здесь будет логика: mainStack.push("ForgotPassword.qml")
                                }
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        CheckBox {
                            id: showPassword
                            checked: true
                            text: "Скрыть пароль"

                            indicator: Rectangle {
                                    implicitWidth: 18
                                    implicitHeight: 18
                                    x: showPassword.leftPadding
                                    y: (showPassword.height - height) / 2
                                    radius: 4
                                    color: "#2c2d30"
                                    border.color: showPassword.checked ? "#229ED9" : "#4d4e4f"
                                    border.width: 1

                                    Rectangle {
                                        anchors.centerIn: parent
                                        width: 10
                                        height: 10
                                        radius: 2
                                        color: "#229ED9"
                                        visible: showPassword.checked
                                    }
                                }

                            contentItem: Text {
                                    text: showPassword.text
                                    color: "#808080"
                                    font.pixelSize: 16
                                    verticalAlignment: Text.AlignVCenter
                                    leftPadding: showPassword.indicator.width + 5
                                }
                        }
                    }


                }

                Button {
                    id: loginBtn
                    property bool isProcessing: false
                    text: isProcessing ? "Вход..." : "Войти"
                    Layout.preferredWidth: parent.tfWidht*0.7
                    Layout.preferredHeight: 45
                    Layout.alignment: Qt.AlignCenter
                    enabled: !isProcessing
                    font.pixelSize: 18



                    background: Rectangle {
                        color: loginBtn.down ? "#0056b3" : (loginBtn.enabled ? "#229ED9" : "#2c2d30")
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
                        forgotPasswordLink.visible = false
                        errorLabel.visible = false
                        errorLabel.text = ""

                        if (login.text.trim() === "") {
                            errorLabel.text = "Введите логин."
                            errorLabel.visible = true
                            login.forceActiveFocus() // Ставим курсор в поле логина
                            return
                        }

                        if (password.text === "") {
                            errorLabel.text = "Введите пароль."
                            errorLabel.visible = true
                            forgotPasswordLink.visible = true
                            password.forceActiveFocus()
                            return
                        }

                        loginBtn.isProcessing = true

                        Authorization.takePassword(1000, {":login" : login.text})
                    }
                }
            }

            // Полоса сооцсетей
            Rectangle {
                width: parent.width
                height: 40
                anchors.bottom: parent.bottom
                color: '#1e1e24'

                RowLayout {
                    id: linksLayout
                    anchors.centerIn: parent

                    spacing: 20

                    property var socialIcons: [
                        { icon: "vk.svg", url: "https://vk.com/1beresnev1" },
                        { icon: "telegram.svg", url: "https://t.me/alcomarket25" },
                        { icon: "github.svg", url: "https://github.com/URETHZ" },
                        { icon: "tiktok.svg", url: "https://example.com" }
                    ]

                    Repeater {
                        model: linksLayout.socialIcons

                        delegate: Item {
                            width: 20
                            height: 20

                            HoverHandler {
                                id: hover
                            }

                            Image {
                                id: iconImage
                                source: "images/icons/" + modelData.icon
                                anchors.fill: parent
                                fillMode: Image.PreserveAspectFit

                                scale: hover.hovered ? 1.2 : 1.0
                                Behavior on scale {
                                    NumberAnimation { duration: 200; easing.type: Easing.OutBack }
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    Qt.openUrlExternally(modelData.url)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Item {

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        width: 35
        height: 35

        HoverHandler {
            id: hoverSettings
        }

        Image {
            id: settingsIcon
            source: "images/icons/settings_icon.svg"
            anchors.fill: parent
            opacity: hoverSettings.hovered ? 1 : 0.7
            fillMode: Image.PreserveAspectFit
            scale: hoverSettings.hovered ? 1.1 : 1.0
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
                mainStack.push("SettingsAuthFrame.qml")
            }
        }
    }
    Rectangle {
        id: statusConnFrame
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 5
        z: 10
        visible: false
        radius: height / 2
        color: isOnline ? Qt.alpha("green", 0.5) : Qt.alpha("red", 0.5)

        property bool isOnline: false
        property string setText: "Статус подключения"
        readonly property real hPadding: 30
        readonly property real maxWidth: parent.width * 0.7

        width: Math.min(metrics.width + 5 + hPadding, maxWidth)
        implicitHeight: label.implicitHeight + 14
        height: implicitHeight

        Behavior on width {
            NumberAnimation { duration: 50; easing.type: Easing.OutCubic }
        }

        // измеряет ширину текста в одну строку, без переноса
        TextMetrics {
            id: metrics
            font: label.font
            text: statusConnFrame.setText
        }

        Text {
            id: label
            anchors.centerIn: parent
            width: parent.width - statusConnFrame.hPadding
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 14
            color: "white"
            text: statusConnFrame.setText
        }
    }
}