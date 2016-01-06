###############################################################################################
#                                                                                             #
# This file is part of MailUnit.                                                              #
#                                                                                             #
# MailUnit is free software: you can redistribute it and/or modify it under the terms of      #
# the GNU General Public License as published by the Free Software Foundation,                #
# either version 3 of the License, or (at your option) any later version.                     #
#                                                                                             #
# MailUnit is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;      #
# without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  #
# See the GNU General Public License for more details.                                        #
#                                                                                             #
# You should have received a copy of the GNU General Public License along with MailUnit.      #
# If not, see <http://www.gnu.org/licenses/>.                                                 #
#                                                                                             #
###############################################################################################

###############################################################################################
#
# Allowed flags:
#
# ENABLE_TESTS=ON
#    Enables unit tests. The Boost.Test library is required.
# ENABLE_GUI=ON
#    Enables graphic user interface. The Qt 4 or later is required.
# QT5_DIR=<path to Qt5 installation>
#    Provides a path to search Qt5 installation. This variable can be also specified as
#    a system environvent variable.
#    Example: -DQT5_DIR=/opt/qt/5.4/gcc_64/
#
###############################################################################################

cmake_minimum_required(VERSION 3.0)

project(MailUnit)

if(POLICY CMP0054)
    cmake_policy(SET CMP0054 OLD)
endif()


set(BINARY_NAME       mailunit)
set(TARGET_SERVER_LIB mailunit-server-lib)
set(TARGET_SERVER     mailunit-server)
set(TARGET_LIB        mailunit-lib)
set(TARGET_TESTS      mailunit-tests)
set(TARGET_SQLITE     sqlite)
set(TARGET_GUI        mailunitui)

if(APPLE)
    message(FATAL_ERROR "OS X is not supported yet")
endif()

message(STATUS "Compiler: " ${CMAKE_CXX_COMPILER_ID} " " ${CMAKE_CXX_COMPILER_VERSION})
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_GNU 1)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9")
        message(FATAL_ERROR "g++ 4.9 or greater is required")
    else()
        list(APPEND CMAKE_CXX_FLAGS "-std=c++14 -Wall")
    endif()
#elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(COMPILER_MSVC 1)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.0")
        message(FATAL_ERROR "Microsoft Visual Studio 2015 or greater is required")
    else()
        #list(APPEND CMAKE_CXX_FLAGS "-std=c++14 -Wall")
    endif()
else()
    message(FATAL_ERROR "Unsupported compiler")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Application will be built in the Debug configuration")
    set(CMAKE_VERBOSE_MAKEFILE ON)
    add_definitions(-DMU_DEBUG)
endif()

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

####
# Begin the Boost libraries initialization
####
if(ENABLE_TESTS)
    set(BOOST_TEST unit_test_framework)
endif(ENABLE_TESTS)

if(WIN32)
    if(COMPILER_MSVC)
        set(Boost_USE_STATIC_LIBS ON)
        add_definitions(-DBOOST_ASIO_DISABLE_BUFFER_DEBUGGING) # TODO: Bug in the boost. Try new version (1.59+)
    else(COMPILER_MSVC) # TODO: try to build with static boost libraries using MinGW
        set(Boost_USE_STATIC_LIBS OFF)
        add_definitions(-DBOOST_TEST_DYN_LINK)
    endif(COMPILER_MSVC)
else(WIN32)
    set(Boost_USE_STATIC_LIBS OFF)
    add_definitions(-DBOOST_TEST_DYN_LINK)
endif(WIN32)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS
    system
    filesystem
    date_time
    program_options
    regex
    iostreams
    ${BOOST_TEST}
)
link_directories(${Boost_LIBRARY_DIRS})
####
# End the Boost libraries initialization
####

####
# Begin Qt libraries initialization
####
if(ENABLE_GUI)
    if(QT5_DIR)
        set(CMAKE_PREFIX_PATH ${QT5_DIR})
    elseif(DEFINED ENV{QT5_DIR})
        set(CMAKE_PREFIX_PATH $ENV{QT5_DIR})
    endif()
    find_package(Qt5Core)
    if(Qt5Core_FOUND)
        message(STATUS "Qt version: ${Qt5Core_VERSION_STRING}")
    else()
        message(FATAL_ERROR "ERROR: Qt5 not found. Please specify path to Qt5 installation in the QT5_DIR cmake variable or the QT5_DIR system environment variable")
    endif()
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5Network REQUIRED)
    find_package(Qt5Xml REQUIRED)
    find_package(Qt5WebKitWidgets REQUIRED)
endif(ENABLE_GUI)
####
# End Qt libraries initialization
####

if(WIN32)
    # Windows sockets
    set(WINSOCKET_LIBS ws2_32 mswsock)
    set(LIBPREFIX)
else(WIN32)
    set(LIBPREFIX lib)
endif(WIN32)

add_definitions(
    -DBOOST_FILESYSTEM_NO_DEPRECATED
    -D_MU_DISABLE_NOT_STANDARD_CPP_API
    -D_MU_SERVER_BINARY_NAME=${BINARY_NAME}
    -D_MU_GUI_BINARY_NAME=${TARGET_GUI}
    -D_MU_CONFIG_DIRECTORY=${BINARY_NAME}
)


#######################################################
#                                                     #
#                     SOURCES                         #
#                                                     #
#######################################################

set(SRC_LIB
    LibMailUnit/Api/Include/Def.h
    LibMailUnit/Api/Include/StringList.h
    LibMailUnit/Api/Include/Message.h
    LibMailUnit/Api/Include/Message/Mime.h
    LibMailUnit/Api/Include/Message/MailHeader.h
    LibMailUnit/Api/Include/Message/Mailbox.h
    LibMailUnit/Api/Include/Message/DateTime.h
    LibMailUnit/Api/Include/Message/MessageId.h
    LibMailUnit/Api/Include/Message/ContentType.h
    LibMailUnit/Api/Include/Mqp/Client.h
    ####
    LibMailUnit/Api/Impl/ApiObject.h
    LibMailUnit/Api/Impl/ApiObject.cpp
    LibMailUnit/Api/Impl/StringList.h
    LibMailUnit/Api/Impl/StringList.cpp
    LibMailUnit/Api/Impl/Message/Mime.h
    LibMailUnit/Api/Impl/Message/Mime.cpp
    LibMailUnit/Api/Impl/Message/Headers.h
    LibMailUnit/Api/Impl/Message/Headers.cpp
    LibMailUnit/Api/Impl/Message/Mailbox.h
    LibMailUnit/Api/Impl/Message/Mailbox.cpp
    LibMailUnit/Api/Impl/Message/DateTime.cpp
    LibMailUnit/Api/Impl/Message/MessageId.h
    LibMailUnit/Api/Impl/Message/MessageId.cpp
    LibMailUnit/Api/Impl/Message/ContentType.h
    LibMailUnit/Api/Impl/Message/ContentType.cpp
    LibMailUnit/Api/Impl/Mqp/Client.h
    LibMailUnit/Api/Impl/Mqp/Client.cpp
    ####
    LibMailUnit/Message/Headers.h
    LibMailUnit/Message/Headers.cpp
    LibMailUnit/Message/ContentType.h
    LibMailUnit/Message/ContentType.cpp
    LibMailUnit/Message/Mailbox.h
    LibMailUnit/Message/Mailbox.cpp
    LibMailUnit/Message/Mime.h
    LibMailUnit/Message/Mime.cpp
    LibMailUnit/Mqp/Client.h
    LibMailUnit/Mqp/Client.cpp
    LibMailUnit/Mqp/Command.h
    LibMailUnit/Mqp/Response.h
    LibMailUnit/Mqp/Message.h
)

set(SRC_SERVER_LIB
    MailUnit/String.h
    MailUnit/DeferredPointer.h
    MailUnit/OS/FileSystem.h
    MailUnit/OS/FileSystem.cpp
    MailUnit/Logger.h
    MailUnit/Logger.cpp
    MailUnit/Exception.h
    MailUnit/IO/IODef.h
    MailUnit/IO/AsyncWriter.h
    MailUnit/IO/AsyncSequenceOperation.h
    MailUnit/IO/AsyncOperation.h
    MailUnit/IO/AsyncFileWriter.h
    MailUnit/IO/AsyncFileWriter.cpp
    MailUnit/IO/AsyncLambdaWriter.h
    MailUnit/IO/AsyncLambdaWriter.cpp
    MailUnit/Server/RequestHandler.h
    MailUnit/Server/Session.h
    MailUnit/Server/TlsContext.h
    MailUnit/Server/TlsContext.cpp
    MailUnit/Server/Tcp/TcpServer.h
    MailUnit/Server/Tcp/TcpServer.cpp
    MailUnit/Server/Tcp/TcpSession.h
    MailUnit/Server/Tcp/TcpSession.cpp
    MailUnit/Smtp/ServerRequestHandler.h
    MailUnit/Smtp/ServerRequestHandler.cpp
    MailUnit/Smtp/Protocol.h
    MailUnit/Smtp/Protocol.cpp
    MailUnit/Smtp/ProtocolExtension.h
    MailUnit/Smtp/Response.h
    MailUnit/Smtp/Response.cpp
    MailUnit/Storage/StorageException.h
    MailUnit/Storage/Edsl.h
    MailUnit/Storage/Edsl.cpp
    MailUnit/Storage/Repository.h
    MailUnit/Storage/Repository.cpp
    MailUnit/Storage/Email.h
    MailUnit/Storage/Email.cpp
    MailUnit/Mqp/ServerRequestHandler.h
    MailUnit/Mqp/ServerRequestHandler.cpp
    MailUnit/Mqp/Error.h
)

set(SRC_SERVER
    MailUnit/Main.cpp
    MailUnit/Config.h
    MailUnit/Config.cpp
)

set(SRC_GUI_MOC
    MailUnitUI/MqpClient/ServerConfig.h
    MailUnitUI/MqpClient/MqpClient.h
    MailUnitUI/Gui/MainWindow.h
    MailUnitUI/Gui/ServerDialog.h
    MailUnitUI/Gui/AboutDialog.h
    MailUnitUI/Gui/QueryWidget.h
    MailUnitUI/Gui/MessageListView.h
    MailUnitUI/Gui/HtmlView.h
)

set(SRC_GUI_UI
    MailUnitUI/Gui/MainWindow.ui
    MailUnitUI/Gui/ServerDialog.ui
    MailUnitUI/Gui/AboutDialog.ui
    MailUnitUI/Gui/QueryWidget.ui
)

set(SRC_GUI_RSC
    MailUnitUI/Resources/Resources.qrc
)

set(SRC_GUI_BIN
    MailUnitUI/Resources/mu-logo-100.png
)

set(SRC_GUI
    ${SRC_GUI_MOC}
    ${SRC_GUI_UI}
    MailUnit/OS/FileSystem.cpp
    MailUnitUI/Main.cpp
    MailUnitUI/Config.h
    MailUnitUI/Config.cpp
    MailUnitUI/MqpClient/ServerConfig.cpp
    MailUnitUI/MqpClient/MqpClient.cpp
    MailUnitUI/MqpClient/MimeMessage.h
    MailUnitUI/MqpClient/MimeMessage.cpp
    MailUnitUI/MqpClient/MqpMessage.h
    MailUnitUI/MqpClient/MqpMessage.cpp
    MailUnitUI/Gui/MainWindow.cpp
    MailUnitUI/Gui/QueryWidget.cpp
    MailUnitUI/Gui/MessageListView.cpp
    MailUnitUI/Gui/HtmlView.cpp
)

set(SQLITE_SRC
    SQLite/sqlite3.c
    SQLite/sqlite3ext.h
    SQLite/sqlite3.h
)

set(SRC_TESTS
    Tests/Main.cpp
    Tests/LibMailUnit/Memory.cpp
    Tests/LibMailUnit/Headers.cpp
    Tests/LibMailUnit/MessageId.cpp
    Tests/LibMailUnit/DateTime.cpp
    Tests/LibMailUnit/Address.cpp
    Tests/LibMailUnit/ContentType.cpp
    Tests/LibMailUnit/Mime.cpp
    Tests/MailUnit/DeferredPointer.cpp
    Tests/MailUnit/Edsl.cpp
    Tests/MailUnit/File.cpp
    Tests/MailUnit/Repository.cpp
    Tests/MailUnit/SmtpPorotocol.cpp
)

set(OTHER_FILES
    .gitignore
    Cert/cert.pem
    Cert/key.pem
    Cert/password.txt
    Doxygen/Doxyfile
    Doxygen/Pages/Main.dox
    Doxygen/Snippets/RFC/HeaderSpec.html
    Doxygen/Snippets/RFC/DateSpec.html
    Doxygen/Snippets/RFC/AddressSpec.html
    Doxygen/Snippets/RFC/IdentificationFieldsSpec.html
    Doxygen/Snippets/RFC/ContentTypeSpec.html
    Doxygen/Groups/EMail.dox
)





#######################################################
#                                                     #
#                     TARGETS                         #
#                                                     #
#######################################################

set(COMMON_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}
    ${Boost_INCLUDE_DIRS}
)

#
# sqlite
#
add_library(${TARGET_SQLITE} STATIC ${SQLITE_SRC})
target_include_directories(${TARGET_SQLITE} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/SQLite
)
target_compile_definitions(${TARGET_SQLITE} PRIVATE
    -DSQLITE_THREADSAFE=1
)
target_link_libraries(${TARGET_SQLITE}
    ${CMAKE_DL_LIBS}
)

#
# libmailunit
#
add_library(${TARGET_LIB} SHARED ${SRC_LIB})
set_target_properties(${TARGET_LIB} PROPERTIES
    OUTPUT_NAME ${BINARY_NAME}
    PREFIX "${LIBPREFIX}"
)
target_include_directories(${TARGET_LIB} PRIVATE
    ${COMMON_INCLUDE_DIRS}
)
target_compile_definitions(${TARGET_LIB} PRIVATE
    -D_MU_LIB
)
target_link_libraries(${TARGET_LIB}
    ${CMAKE_DL_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${WINSOCKET_LIBS}
)

#
# mailunit-server-lib
#
add_library(${TARGET_SERVER_LIB} STATIC ${SRC_SERVER_LIB})
target_include_directories(${TARGET_SERVER_LIB} PRIVATE
    ${COMMON_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)
target_compile_definitions(${TARGET_SERVER_LIB} PRIVATE
    -D_MU_SERVER_NAME=${PROJECT_NAME}
)
target_link_libraries(${TARGET_SERVER_LIB}
    ${TARGET_SQLITE}
    ${TARGET_LIB}
    ${OPENSSL_LIBRARIES}
)

#
# mailunit-server
#
add_executable(${TARGET_SERVER} ${SRC_SERVER})
set_target_properties(${TARGET_SERVER} PROPERTIES
    OUTPUT_NAME ${BINARY_NAME}
)
add_dependencies(${TARGET_SERVER}
    ${TARGET_LIB}
    ${TARGET_SQLITE}
    ${TARGET_SERVER_LIB}
)
target_include_directories(${TARGET_SERVER} PRIVATE
    ${COMMON_INCLUDE_DIRS}
)
target_compile_definitions(${TARGET_SERVER} PRIVATE
    -D_MU_SERVER
    -D_MU_BINARY_NAME=${BINARY_NAME}
    -D_MU_SERVER_NAME=${PROJECT_NAME}
)
target_link_libraries(${TARGET_SERVER}
    ${CMAKE_DL_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${TARGET_LIB}
    ${TARGET_SERVER_LIB}
    ${WINSOCKET_LIBS}
)


#
# mailunitui
#
if(ENABLE_GUI)
    qt5_wrap_cpp(GUI_MOC ${SRC_GUI_MOC})
    qt5_wrap_ui(GUI_UI ${SRC_GUI_UI})
    qt5_add_resources(GUI_RSC ${SRC_GUI_RSC})
    add_executable(${TARGET_GUI}
        ${SRC_GUI}
        ${GUI_MOC}
        ${GUI_UI}
        ${GUI_RSC}
        ${SRC_GUI_BIN}
    )
    qt5_use_modules(${TARGET_GUI}
        Widgets
        Network
        Xml
        WebKitWidgets
    )
    add_dependencies(${TARGET_GUI}
        ${TARGET_LIB}
    )
    target_include_directories(${TARGET_GUI} PRIVATE
        ${COMMON_INCLUDE_DIRS}
        ${CMAKE_BINARY_DIR}
    )
    target_compile_definitions(${TARGET_GUI} PRIVATE
        -D_MU_GUI
    )
    target_link_libraries(${TARGET_GUI}
        ${TARGET_LIB}
    )
endif(ENABLE_GUI)

#
# mailunit-tests
#
if(ENABLE_TESTS)
    add_executable(${TARGET_TESTS} ${SRC_TESTS})
    target_include_directories(${TARGET_TESTS} PRIVATE
        ${COMMON_INCLUDE_DIRS}
    )
    add_dependencies(${TARGET_TESTS}
        ${TARGET_LIB}
        ${TARGET_SQLITE}
        ${TARGET_SERVER_LIB}
    )
    target_compile_definitions(${TARGET_TESTS} PRIVATE
        -D_MU_TESTS
    )
    target_link_libraries(${TARGET_TESTS}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES}
        ${TARGET_LIB}
        ${TARGET_SERVER_LIB}
        ${WINSOCKET_LIBS}
    )
endif(ENABLE_TESTS)

#
# misc.
#
add_custom_target(other SOURCES ${OTHER_FILES})
