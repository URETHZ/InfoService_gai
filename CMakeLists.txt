cmake_minimum_required(VERSION 3.16)

project(Project_GAI_information_system VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql)

qt_standard_project_setup(REQUIRES 6.10)

set(CPP_SOURCES
    src/Main.cpp
    src/lib/1.cpp
    src/parse/1.cpp
    src/server/databaseController.cpp
    src/server/databaseController.h
)
set(QML_SOURCES
    qml/Main.qml
    qml/LoginScreen.qml
    qml/General.qml
    qml/ChangePasswordFrame.qml
)

file(GLOB IMAGE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/qml/images/icons/*.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/qml/images/authFrameImage/*.svg"
)

qt_add_executable(appProject_GAI_information_system
    ${CPP_SOURCES}
)

qt_add_qml_module(appProject_GAI_information_system
    URI Project_GAI_information_system
    QML_FILES
        ${QML_SOURCES}
    RESOURCES
        ${IMAGE_FILES}
    SOURCES
        src/auth/authorization.h
        src/auth/authorization.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appProject_GAI_information_system PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appProject_GAI_information_system
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appProject_GAI_information_system
    PRIVATE Qt6::Quick Qt6::Sql
)

# исходники Argon2
target_sources(appProject_GAI_information_system PRIVATE
    thirdparty/argon2/src/argon2.c
    thirdparty/argon2/src/core.c
    thirdparty/argon2/src/encoding.c
    thirdparty/argon2/src/ref.c
    thirdparty/argon2/src/opt.c
    thirdparty/argon2/src/thread.c
    thirdparty/argon2/src/blake2/blake2b.c
)

target_include_directories(appProject_GAI_information_system PRIVATE
    thirdparty/argon2/include
    thirdparty/argon2/src
    thirdparty/argon2/src/blake2
    src/auth
    src/server
    src/lib
    src/parse
)

install(TARGETS appProject_GAI_information_system
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
