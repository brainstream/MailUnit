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
set(BINARY_NAME      mailunit)
set(TARGET_SERVER    mailunit-server)
set(TARGET_LIB       mailunit-lib)
set(TARGET_TESTS     mailunit-tests)
set(TARGET_SQLITE    sqlite)

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
        list(APPEND CMAKE_CXX_FLAGS "-std=c++14")
    endif()
#elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
else()
    message(FATAL_ERROR "Unsupported compiler")
endif()

find_package(Threads REQUIRED)

#### Begin Boost ####
if(ALLOW_TESTS)
    set(BOOST_TEST unit_test_framework)
endif(ALLOW_TESTS)
if(WIN32)
    set(Boost_USE_STATIC_LIBS ON)
else()
    set(Boost_USE_STATIC_LIBS OFF)
    add_definitions(-DBOOST_TEST_DYN_LINK)
endif()
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
#### End Boost ####

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/Source
    ${CMAKE_CURRENT_LIST_DIR}/Include
    ${CMAKE_CURRENT_LIST_DIR}/ThirdParty
    ${Boost_INCLUDE_DIRS}
)

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

set(TESTABLE_SRC_SERVER
    Source/MailUnit/Storage/Dsel.h
    Source/MailUnit/Storage/Dsel.cpp
)

set(SRC_SERVER
    ${TESTABLE_SRC_SERVER}
    Source/MailUnit/Application.h
    Source/MailUnit/Application.cpp
    Source/MailUnit/EnumMap.h
    Source/MailUnit/Config.h
    Source/MailUnit/Config.cpp
    Source/MailUnit/Logger.h
    Source/MailUnit/Logger.cpp
    Source/MailUnit/Exception.h
    Source/MailUnit/Email.h
    Source/MailUnit/Email.cpp
    Source/MailUnit/Server/RequestHandler.h
    Source/MailUnit/Server/TcpServer.h
    Source/MailUnit/Server/TcpServer.cpp
    Source/MailUnit/Smtp/ProtocolDef.h
    Source/MailUnit/Smtp/ResponseCode.h
    Source/MailUnit/Smtp/ResponseCode.cpp
    Source/MailUnit/Smtp/Message.h
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
    Source/MailUnit/Storage/DatabaseException.h
    Source/MailUnit/Storage/Database.h
    Source/MailUnit/Storage/SqlDatabase.h
    Source/MailUnit/Storage/SqlDatabase.cpp
    Source/MailUnit/Storage/SqliteDatabase.h
    Source/MailUnit/Storage/DBObject.h
    Source/MailUnit/Mqp/ServerRequestHandler.h
    Source/MailUnit/Mqp/ServerRequestHandler.cpp
    Source/MailUnit/Mqp/DBObjectCompressor.h
    Source/MailUnit/Mqp/DBObjectCompressor.cpp
)

set(TESTS_SRC
    ${TESTABLE_SRC_SERVER}
    Tests/Main.cpp
    Tests/TestFixture.h
    Tests/TestFixture.cpp
    Tests/LibMailUnit/Memory.cpp
    Tests/LibMailUnit/Headers.cpp
    Tests/LibMailUnit/MessageId.cpp
    Tests/LibMailUnit/DateTime.cpp
    Tests/LibMailUnit/Address.cpp
    Tests/MailUnit/Edsl.cpp
)

set(SQLITE_SRC
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

set(LIBS
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

add_library(${TARGET_SQLITE} STATIC ${SQLITE_SRC})
target_compile_definitions(${TARGET_SQLITE} PRIVATE -DSQLITE_THREADSAFE=1)

add_library(${TARGET_LIB} SHARED ${SRC_LIB})
target_link_libraries(${TARGET_LIB} ${LIBS})
target_compile_definitions(${TARGET_LIB} PRIVATE -D_MU_LIB)
set_target_properties(${TARGET_LIB} PROPERTIES OUTPUT_NAME ${BINARY_NAME})

add_executable(${TARGET_SERVER} ${SRC_SERVER})
target_compile_definitions(${TARGET_SERVER} PRIVATE -D_MU_SERVER)
target_link_libraries(${TARGET_SERVER} ${LIBS} ${TARGET_LIB} ${TARGET_SQLITE} ${CMAKE_DL_LIBS})
set_target_properties(${TARGET_SERVER} PROPERTIES OUTPUT_NAME ${BINARY_NAME})

if(ALLOW_TESTS)
    add_executable(${TARGET_TESTS} ${TESTS_SRC})
    target_link_libraries(${TARGET_TESTS} ${LIBS} ${TARGET_LIB})
endif(ALLOW_TESTS)

add_custom_target(other SOURCES ${OTHER_FILES})
