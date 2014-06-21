###############################################################################################
#                                                                                             #
# This file is part of StubMTP.                                                               #
#                                                                                             #
# StubMTP is free software: you can redistribute it and/or modify it under the terms of       #
# the GNU General Public License as published by the Free Software Foundation,                #
# either version 3 of the License, or (at your option) any later version.                     #
#                                                                                             #
# StubMTP is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;       #
# without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  #
# See the GNU General Public License for more details.                                        #
#                                                                                             #
# You should have received a copy of the GNU General Public License along with StubMTP.       #
# If not, see <http://www.gnu.org/licenses/>.                                                 #
#                                                                                             #
###############################################################################################

cmake_minimum_required(VERSION 2.8)

project(StubMTP)
set(BINARY_NAME    stubmtp)
set(TARGET_SERVER  stubmtp-server)
set(TARGET_CLIENT  stubmtp-client)
set(TARGET_COMMON  stubmtp-common)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Application will be built in the Debug configuration")
    set(CMAKE_VERBOSE_MAKEFILE ON)
    add_definitions(-DDEBUG)
endif()

message(STATUS "Compiler: " ${CMAKE_CXX_COMPILER_ID} " " ${CMAKE_CXX_COMPILER_VERSION})
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.7")
        message("g++ 4.7 or greater is required")
    else()
        list(APPEND CMAKE_CXX_FLAGS "-std=c++11")
    endif()
#elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
else()
    message(FATAL_ERROR "Unsupported compiler")
endif()

find_package(Threads REQUIRED)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system filesystem date_time program_options regex)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${Boost_INCLUDE_DIRS}/StubMTP
)

set(HDR_COMMON
    StubMTP/Common/Email/Address.h
    StubMTP/Common/Email/DateTime.h
    StubMTP/Common/Email/Header.h
    StubMTP/Common/Email/MessageId.h
    StubMTP/Common/Email.h
)

set(SRC_COMMON
    ${HDR_COMMON}
    StubMTP/Common/Email/Address.cpp
    StubMTP/Common/Email/DateTime.cpp
    StubMTP/Common/Email/Header.cpp
    StubMTP/Common/Email/MessageId.cpp
)

set(SRC_SERVER
    ${HDR_COMMON}
    StubMTP/Server/Application.h
    StubMTP/Server/Application.cpp
    StubMTP/Server/Aux.h
    StubMTP/Server/Config.h
    StubMTP/Server/Config.cpp
    StubMTP/Server/Logger.h
    StubMTP/Server/Logger.cpp
    StubMTP/Server/Exception.h
    StubMTP/Server/SmtpController.h
    StubMTP/Server/SmtpController.cpp
    StubMTP/Server/Email/Mime.h
    StubMTP/Server/Email/Mime.cpp
    StubMTP/Server/Email/Header.h
    StubMTP/Server/Email/Header.cpp
    StubMTP/Server/Email/Address.h
    StubMTP/Server/Email/Address.cpp
    StubMTP/Server/Email/DateTime.h
    StubMTP/Server/Email/DateTime.cpp
    StubMTP/Server/Email/MessageId.h
    StubMTP/Server/Email/MessageId.cpp
    StubMTP/Server/Smtp/ProtocolDef.h
    StubMTP/Server/Smtp/Server.h
    StubMTP/Server/Smtp/Server.cpp
    StubMTP/Server/Smtp/ResponseCode.h
    StubMTP/Server/Smtp/ResponseCode.cpp
    StubMTP/Server/Smtp/SessionProvider.h
    StubMTP/Server/Smtp/SessionProvider.cpp
    StubMTP/Server/Smtp/Message.h
    StubMTP/Server/Smtp/StateMachine/StateMachine.h
    StubMTP/Server/Smtp/StateMachine/StateBase.h
    StubMTP/Server/Smtp/StateMachine/SingleLineCmdState.h
    StubMTP/Server/Smtp/StateMachine/SingleLineCmdState.cpp
    StubMTP/Server/Smtp/StateMachine/StartState.h
    StubMTP/Server/Smtp/StateMachine/EhloState.h
    StubMTP/Server/Smtp/StateMachine/MailFromState.h
    StubMTP/Server/Smtp/StateMachine/MailFromState.cpp
    StubMTP/Server/Smtp/StateMachine/RcptToState.h
    StubMTP/Server/Smtp/StateMachine/RcptToState.cpp
    StubMTP/Server/Smtp/StateMachine/DataState.h
    StubMTP/Server/Smtp/StateMachine/DataState.cpp
    StubMTP/Server/Smtp/StateMachine/QuitState.h
    StubMTP/Server/Database/Sql.h
)

set(SRC_CLIENT
    ${HDR_COMMON}
    StubMTP/Client/Temp.cpp
)

set(LIBS
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)


add_library(${TARGET_COMMON} STATIC ${SRC_COMMON})

add_library(${TARGET_CLIENT} SHARED ${SRC_CLIENT})
target_link_libraries(${TARGET_CLIENT} ${LIBS} ${TARGET_COMMON})
set_target_properties(${TARGET_CLIENT} PROPERTIES
    OUTPUT_NAME ${BINARY_NAME})

add_executable(${TARGET_SERVER} ${SRC_SERVER})
target_link_libraries(${TARGET_SERVER} ${LIBS} ${TARGET_COMMON})
set_target_properties(${TARGET_SERVER} PROPERTIES
    OUTPUT_NAME ${BINARY_NAME})


