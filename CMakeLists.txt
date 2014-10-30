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

cmake_minimum_required(VERSION 2.8)

project(MailUnit)
set(BINARY_NAME       mailunit)
set(TARGET_SERVER_LIB mailunit-server-lib)
set(TARGET_SERVER     mailunit-server)
set(TARGET_LIB        mailunit-lib)
set(TARGET_TESTS      mailunit-tests)
set(TARGET_SQLITE     sqlite)

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
add_definitions(-DBOOST_FILESYSTEM_NO_DEPRECATED)
####
# End the Boost libraries initialization
####

if(WIN32)
    # Windows sockets
    set(WINSOCKET_LIBS ws2_32 mswsock)
endif(WIN32)



#######################################################
#                                                     #
#                     SOURCES                         #
#                                                     #
#######################################################

set(SRC_LIB
    Include/LibMailUnit/MailUnit.h
    Include/LibMailUnit/Def.h
    Include/LibMailUnit/Memory.h
    Source/LibMailUnit/InternalMemory.h
    Source/LibMailUnit/Memory.cpp
    Include/LibMailUnit/Mail/Headers.h
    Source/LibMailUnit/Mail/Headers.cpp
    Include/LibMailUnit/Mail/MessageId.h
    Source/LibMailUnit/Mail/MessageId.cpp
    Include/LibMailUnit/Mail/DateTime.h
    Source/LibMailUnit/Mail/DateTime.cpp
    Include/LibMailUnit/Mail/Address.h
    Source/LibMailUnit/Mail/Address.cpp
    Source/LibMailUnit/Mail/Mailbox.h
    Source/LibMailUnit/Mail/Mailbox.cpp
    Source/LibMailUnit/Mail/MailboxGroup.h
    Source/LibMailUnit/Mail/MailboxGroup.cpp
)

set(SRC_SERVER_LIB
    Source/MailUnit/Application.h
    Source/MailUnit/StringEx.h
    Source/MailUnit/OS/FileSystem.h
    Source/MailUnit/OS/FileSystem.cpp
    Source/MailUnit/Config.h
    Source/MailUnit/Config.cpp
    Source/MailUnit/Logger.h
    Source/MailUnit/Logger.cpp
    Source/MailUnit/Exception.h
    Source/MailUnit/IO/IODef.h
    Source/MailUnit/IO/AsyncSequenceOperation.h
    Source/MailUnit/IO/AsyncOperation.h
    Source/MailUnit/IO/AsyncFileWriter.h
    Source/MailUnit/IO/AsyncLambdaWriter.h
    Source/MailUnit/Server/RequestHandler.h
    Source/MailUnit/Server/TcpServer.h
    Source/MailUnit/Server/TcpServer.cpp
    Source/MailUnit/Smtp/ProtocolDef.h
    Source/MailUnit/Smtp/ResponseCode.h
    Source/MailUnit/Smtp/ResponseCode.cpp
    Source/MailUnit/Smtp/ServerRequestHandler.h
    Source/MailUnit/Smtp/ServerRequestHandler.cpp
    Source/MailUnit/Smtp/StateMachine/StateMachine.h
    Source/MailUnit/Smtp/StateMachine/StateBase.h
    Source/MailUnit/Smtp/StateMachine/SingleLineCmdState.h
    Source/MailUnit/Smtp/StateMachine/SingleLineCmdState.cpp
    Source/MailUnit/Smtp/StateMachine/StartState.h
    Source/MailUnit/Smtp/StateMachine/EhloState.h
    Source/MailUnit/Smtp/StateMachine/MailFromState.h
    Source/MailUnit/Smtp/StateMachine/MailFromState.cpp
    Source/MailUnit/Smtp/StateMachine/RcptToState.h
    Source/MailUnit/Smtp/StateMachine/RcptToState.cpp
    Source/MailUnit/Smtp/StateMachine/DataState.h
    Source/MailUnit/Smtp/StateMachine/DataState.cpp
    Source/MailUnit/Smtp/StateMachine/QuitState.h
    Source/MailUnit/Storage/StorageException.h
    Source/MailUnit/Storage/Edsl.h
    Source/MailUnit/Storage/Edsl.cpp
    Source/MailUnit/Storage/Repository.h
    Source/MailUnit/Storage/Repository.cpp
    Source/MailUnit/Storage/Email.h
    Source/MailUnit/Storage/Email.cpp
    Source/MailUnit/Mqp/ServerRequestHandler.h
    Source/MailUnit/Mqp/ServerRequestHandler.cpp
)

set(SRC_SERVER
    Source/MailUnit/Application.cpp
)

set(SRC_TESTS
    Tests/Main.cpp
    Tests/LibMailUnit/Memory.cpp
    Tests/LibMailUnit/Headers.cpp
    Tests/LibMailUnit/MessageId.cpp
    Tests/LibMailUnit/DateTime.cpp
    Tests/LibMailUnit/Address.cpp
    Tests/MailUnit/Edsl.cpp
    Tests/MailUnit/File.cpp
)

set(SRC_SQLITE
    ThirdParty/SQLite/sqlite3.c
    ThirdParty/SQLite/sqlite3ext.h
    ThirdParty/SQLite/sqlite3.h
)

set(OTHER_FILES
    .gitignore
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
    ${CMAKE_CURRENT_LIST_DIR}/Source
    ${CMAKE_CURRENT_LIST_DIR}/Include
    ${CMAKE_CURRENT_LIST_DIR}/ThirdParty
    ${Boost_INCLUDE_DIRS}
)

#
# sqlite
#
add_library(${TARGET_SQLITE} STATIC ${SRC_SQLITE})
target_include_directories(${TARGET_SQLITE} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ThirdParty
)
target_compile_definitions(${TARGET_SQLITE} PRIVATE
    -DSQLITE_THREADSAFE=1
)

#
# libmailunit
#
add_library(${TARGET_LIB} SHARED ${SRC_LIB})
set_target_properties(${TARGET_LIB} PROPERTIES
    OUTPUT_NAME ${BINARY_NAME}
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
)
target_link_libraries(${TARGET_SERVER_LIB}
    ${TARGET_SQLITE}
    ${TARGET_LIB}
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
