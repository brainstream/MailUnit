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
#
###############################################################################################

cmake_minimum_required(VERSION 2.8)

project(MailUnit)
set(BINARY_NAME       mailunit)
set(TARGET_SERVER_LIB mailunit-server-lib)
set(TARGET_SERVER     mailunit-server)
set(TARGET_LIB        mailunit-lib)
set(TARGET_TESTS      mailunit-tests)
set(TARGET_SQLITE     sqlite)
set(TARGET_GUI        mailunitui)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Application will be built in the Debug configuration")
    set(CMAKE_VERBOSE_MAKEFILE ON)
    add_definitions(-DMU_DEBUG)
endif()

message(STATUS "Compiler: " ${CMAKE_CXX_COMPILER_ID} " " ${CMAKE_CXX_COMPILER_VERSION})
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9")
        message("g++ 4.9 or greater is required")
    else()
        list(APPEND CMAKE_CXX_FLAGS "-std=c++14 -Wall")
    endif()
#elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
else()
    message(FATAL_ERROR "Unsupported compiler")
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
    set(Boost_USE_STATIC_LIBS ON)
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
####
# End the Boost libraries initialization
####

####
# Begin Qt libraries initialization
####
if(ENABLE_GUI)
    find_package(Qt4 REQUIRED)
    include(${QT_USE_FILE})
    set(GUI_LIBS
        Qt4::QtCore
        Qt4::QtGui
        Qt4::QtNetwork
        Qt4::QtXml
    )
    set(GUI_INCLUDE_DIRS
        ${QT_INCLUDE_DIR}
    )
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
    LibMailUnit/MailUnit.h
    LibMailUnit/Def.h
    LibMailUnit/Memory.h
    LibMailUnit/InternalMemory.h
    LibMailUnit/Memory.cpp
    LibMailUnit/Mail/Headers.h
    LibMailUnit/Mail/Headers.cpp
    LibMailUnit/Mail/MessageId.h
    LibMailUnit/Mail/MessageId.cpp
    LibMailUnit/Mail/DateTime.h
    LibMailUnit/Mail/DateTime.cpp
    LibMailUnit/Mail/Address.h
    LibMailUnit/Mail/Address.cpp
    LibMailUnit/Mail/Mailbox.h
    LibMailUnit/Mail/Mailbox.cpp
    LibMailUnit/Mail/MailboxGroup.h
    LibMailUnit/Mail/MailboxGroup.cpp
)

set(SRC_SERVER_LIB
    MailUnit/StringEx.h
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
    MailUnitUI/ServerConfig.h
    MailUnitUI/MqpClient.h
    MailUnitUI/Gui/MainWindow.h
    MailUnitUI/Gui/ServerDialog.h
    MailUnitUI/Gui/AboutDialog.h
    MailUnitUI/Gui/QueryWidget.h
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
    MailUnitUI/Message.h
    MailUnitUI/Config.h
    MailUnitUI/Config.cpp
    MailUnitUI/ServerConfig.cpp
    MailUnitUI/MqpClient.cpp
    MailUnitUI/Gui/MainWindow.cpp
    MailUnitUI/Gui/QueryWidget.cpp
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
    Tests/MailUnit/DeferredPointer.cpp
    Tests/MailUnit/Edsl.cpp
    Tests/MailUnit/File.cpp
)

set(OTHER_FILES
    .gitignore
    Cert/cert.pem
    Cert/key.pem
    Doxygen/Doxyfile
    Doxygen/Pages/Main.dox
    Doxygen/Snippets/RFC/DateSpec.html
    Doxygen/Snippets/RFC/AddressSpec.html
    Doxygen/Snippets/RFC/IdentificationFieldsSpec.html
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
    QT4_WRAP_CPP(GUI_MOC ${SRC_GUI_MOC})
    QT4_WRAP_UI(GUI_UI ${SRC_GUI_UI})
    QT4_ADD_RESOURCES(GUI_RSC ${SRC_GUI_RSC})
    add_executable(${TARGET_GUI}
        ${SRC_GUI}
        ${GUI_MOC}
        ${GUI_UI}
        ${GUI_RSC}
        ${SRC_GUI_BIN}
    )
    add_dependencies(${TARGET_GUI}
        ${TARGET_LIB}
        ${TARGET_SQLITE}
    )
    target_include_directories(${TARGET_GUI} PRIVATE
        ${COMMON_INCLUDE_DIRS}
        ${CMAKE_BINARY_DIR}
    )
    target_compile_definitions(${TARGET_GUI} PRIVATE
        -D_MU_GUI
    )
    target_link_libraries(${TARGET_GUI}
        ${TARGET_SQLITE}
        ${TARGET_LIB}
        ${GUI_LIBS}
        ${Boost_LIBRARIES}
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
    )
endif(ENABLE_TESTS)

#
# misc.
#
add_custom_target(other SOURCES ${OTHER_FILES})
