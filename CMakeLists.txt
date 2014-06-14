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
    ${Boost_INCLUDE_DIRS}
)

set(SRC
    StubMTP/Application.h
    StubMTP/Application.cpp
    StubMTP/Aux.h
    StubMTP/Config.h
    StubMTP/Config.cpp
    StubMTP/Logger.h
    StubMTP/Logger.cpp
    StubMTP/Exception.h
    StubMTP/SmtpController.h
    StubMTP/SmtpController.cpp
    StubMTP/Email/Mime.h
    StubMTP/Email/Mime.cpp
    StubMTP/Email/Header.h
    StubMTP/Email/Header.cpp
    StubMTP/Email/Address.h
    StubMTP/Email/Address.cpp
    StubMTP/Email/DateTime.h
    StubMTP/Email/DateTime.cpp
    StubMTP/Smtp/ProtocolDef.h
    StubMTP/Smtp/Server.h
    StubMTP/Smtp/Server.cpp
    StubMTP/Smtp/ResponseCode.h
    StubMTP/Smtp/ResponseCode.cpp
    StubMTP/Smtp/SessionProvider.h
    StubMTP/Smtp/SessionProvider.cpp
    StubMTP/Smtp/Message.h
    StubMTP/Smtp/StateMachine/StateMachine.h
    StubMTP/Smtp/StateMachine/StateBase.h
    StubMTP/Smtp/StateMachine/SingleLineCmdState.h
    StubMTP/Smtp/StateMachine/SingleLineCmdState.cpp
    StubMTP/Smtp/StateMachine/StartState.h
    StubMTP/Smtp/StateMachine/EhloState.h
    StubMTP/Smtp/StateMachine/MailFromState.h
    StubMTP/Smtp/StateMachine/MailFromState.cpp
    StubMTP/Smtp/StateMachine/RcptToState.h
    StubMTP/Smtp/StateMachine/RcptToState.cpp
    StubMTP/Smtp/StateMachine/DataState.h
    StubMTP/Smtp/StateMachine/DataState.cpp
    StubMTP/Smtp/StateMachine/QuitState.h
)

set(LIBS
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

string(TOLOWER ${PROJECT_NAME} TARGETNAME)
add_executable(${TARGETNAME} ${SRC})
target_link_libraries(${TARGETNAME} ${LIBS})
