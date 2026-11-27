cmake_minimum_required(VERSION 3.16)

project(Gai VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 Sql)

qt_standard_project_setup(REQUIRES 6.10)

set(CPP_SOURCES
    src/Main.cpp
    src/parse/1.cpp
    src/server/databaseController.cpp
    src/server/databaseController.h
)
set(QML_SOURCES
    qml/Main.qml
    qml/LoginScreen.qml
    qml/General.qml
    qml/ChangePasswordFrame.qml
    qml/SettingsAuthFrame.qml
    qml/modules/documents/DtpForm.qml
    qml/modules/documents/MileageReportPage.qml
    qml/modules/documents/OverdueInspectionPage.qml
    qml/modules/documents/StolenCarsPage.qml
    qml/modules/documents/RightHandCarsPage.qml
    qml/modules/about/AboutPage.qml
    qml/modules/other/SettingsPage.qml
    qml/modules/about/HelpPage.qml
    qml/modules/other/ChangePasswordPage.qml
    qml/modules/DefaultPage.qml
    qml/MenuNode.qml
    qml/StyledMenuItem.qml
    qml/modules/dict/DictionaryPage.qml
)

file(GLOB IMAGE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/qml/images/icons/*.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/qml/images/authFrameImage/*.svg"
)

qt_add_executable(appGai
    ${CPP_SOURCES}
)

qt_add_qml_module(appGai
    URI Gai
    QML_FILES
        ${QML_SOURCES}
    RESOURCES
        ${IMAGE_FILES}
    SOURCES
        src/auth/authorization.h
        src/auth/authorization.cpp
        src/auth/hexByArgon2.cpp
        src/auth/hexByArgon2.h
        src/config/appSettings.h
        src/config/appSettings.cpp
        src/server/databaseWorker.cpp
        src/server/databaseWorker.h
        src/config/libsLoader.h
        src/config/libsLoader.cpp
        src/include/IDictionaryBackend.h
        src/include/moduleAPI.h
        SOURCES libs/dicts/towns.cpp
        SOURCES libs/docs/any.cpp
        SOURCES libs/dicts/tableModel_Dictionary.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appGai PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appGai
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appGai
    PRIVATE Qt6::Quick Qt6::Sql Qt6::QuickControls2
)

# исходники Argon2
target_sources(appGai PRIVATE
    thirdparty/argon2/src/argon2.c
    thirdparty/argon2/src/core.c
    thirdparty/argon2/src/encoding.c
    thirdparty/argon2/src/ref.c
    thirdparty/argon2/src/thread.c
    thirdparty/argon2/src/blake2/blake2b.c
    thirdparty/argon2/include/argon2.h
)

target_include_directories(appGai PRIVATE
    thirdparty/argon2/include
    thirdparty/argon2/src
    thirdparty/argon2/src/blake2
    src/auth
    src/server
    src/lib
    src/parse
    src/config
    src/include
    libs
    libs/dicts
)

install(TARGETS appGai
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
