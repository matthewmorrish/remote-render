cmake_minimum_required(VERSION 3.16)

project(client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appclient
    main.cpp
)

qt_add_qml_module(appclient
    URI client
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES ../common/symmetricpulsegenerator.h ../common/symmetricpulsegenerator.cpp
    SOURCES ../common/concurrentsegment.h       ../common/concurrentsegment.cpp
    SOURCES framestream.h framestream.cpp
    SOURCES framestreamview.h framestreamview.cpp
    SOURCES qmleventlistener.h qmleventlistener.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appclient PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appclient
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appclient
    PRIVATE Qt6::Quick
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

include(GNUInstallDirs)

install(TARGETS appclient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
