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
set(BINARY_NAME    mailunit)
set(TARGET_SERVER  mailunit-server)
set(TARGET_CLIENT  mailunit-client)
set(TARGET_COMMON  mailunit-common)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Application will be built in the Debug configuration")
    set(CMAKE_VERBOSE_MAKEFILE ON)
    add_definitions(-DMU_DEBUG)
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
    ${Boost_INCLUDE_DIRS}/MailUnit
)

set(HDR_COMMON
    MailUnit/Common/Def.h
    MailUnit/Common/Memory.h
    MailUnit/Common/Email.h
    MailUnit/Common/Email/Address.h
    MailUnit/Common/Email/DateTime.h
    MailUnit/Common/Email/Header.h
    MailUnit/Common/Email/MessageId.h
)

set(SRC_COMMON
    ${HDR_COMMON}
    MailUnit/Common/Memory.cpp
    MailUnit/Common/Email/Address.cpp
    MailUnit/Common/Email/DateTime.cpp
    MailUnit/Common/Email/Header.cpp
    MailUnit/Common/Email/MessageId.cpp
)

set(SRC_SERVER
    ${HDR_COMMON}
    MailUnit/Server/Application.h
    MailUnit/Server/Application.cpp
    MailUnit/Server/Aux.h
    MailUnit/Server/Config.h
    MailUnit/Server/Config.cpp
    MailUnit/Server/Logger.h
    MailUnit/Server/Logger.cpp
    MailUnit/Server/Exception.h
    MailUnit/Server/SmtpController.h
    MailUnit/Server/SmtpController.cpp
    MailUnit/Server/Email/Mime.h
    MailUnit/Server/Email/Mime.cpp
    MailUnit/Server/Email/Header.h
    MailUnit/Server/Email/Header.cpp
    MailUnit/Server/Email/Address.h
    MailUnit/Server/Email/Address.cpp
    MailUnit/Server/Email/DateTime.h
    MailUnit/Server/Email/DateTime.cpp
    MailUnit/Server/Email/MessageId.h
    MailUnit/Server/Email/MessageId.cpp
    MailUnit/Server/Smtp/ProtocolDef.h
    MailUnit/Server/Smtp/Server.h
    MailUnit/Server/Smtp/Server.cpp
    MailUnit/Server/Smtp/ResponseCode.h
    MailUnit/Server/Smtp/ResponseCode.cpp
    MailUnit/Server/Smtp/SessionProvider.h
    MailUnit/Server/Smtp/SessionProvider.cpp
    MailUnit/Server/Smtp/Message.h
    MailUnit/Server/Smtp/StateMachine/StateMachine.h
    MailUnit/Server/Smtp/StateMachine/StateBase.h
    MailUnit/Server/Smtp/StateMachine/SingleLineCmdState.h
    MailUnit/Server/Smtp/StateMachine/SingleLineCmdState.cpp
    MailUnit/Server/Smtp/StateMachine/StartState.h
    MailUnit/Server/Smtp/StateMachine/EhloState.h
    MailUnit/Server/Smtp/StateMachine/MailFromState.h
    MailUnit/Server/Smtp/StateMachine/MailFromState.cpp
    MailUnit/Server/Smtp/StateMachine/RcptToState.h
    MailUnit/Server/Smtp/StateMachine/RcptToState.cpp
    MailUnit/Server/Smtp/StateMachine/DataState.h
    MailUnit/Server/Smtp/StateMachine/DataState.cpp
    MailUnit/Server/Smtp/StateMachine/QuitState.h
    MailUnit/Server/Database/Sql.h
)

set(SRC_CLIENT
    ${HDR_COMMON}
    MailUnit/Client/Temp.cpp
)

set(LIBS
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)


add_definitions(-DMU -DMU_USENAMESPACE)
add_library(${TARGET_COMMON} STATIC ${SRC_COMMON})

add_definitions(-DMU_CLIENT)
add_library(${TARGET_CLIENT} SHARED ${SRC_CLIENT})
target_link_libraries(${TARGET_CLIENT} ${LIBS} ${TARGET_COMMON})
set_target_properties(${TARGET_CLIENT} PROPERTIES
    OUTPUT_NAME ${BINARY_NAME})
remove_definitions(-DMU_CLIENT)

add_definitions(-DMU_SERVER)
add_executable(${TARGET_SERVER} ${SRC_SERVER})
target_link_libraries(${TARGET_SERVER} ${LIBS} ${TARGET_COMMON})
set_target_properties(${TARGET_SERVER} PROPERTIES
    OUTPUT_NAME ${BINARY_NAME})
remove_definitions(-DMU_SERVER)

