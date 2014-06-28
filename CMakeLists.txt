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
set(TARGET_LIB     mailunit-lib)

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
    ${CMAKE_CURRENT_LIST_DIR}/Source
    ${CMAKE_CURRENT_LIST_DIR}/Include
    ${Boost_INCLUDE_DIRS}
)

set(SRC_LIB
    Include/LibMailUnit/MailUnit.h
    Include/LibMailUnit/Def.h
    Include/LibMailUnit/Memory.h
    Source/LibMailUnit/InternalMemory.h
    Source/LibMailUnit/Memory.cpp
    Include/LibMailUnit/Mime/MessageId.h
    Source/LibMailUnit/Mime/MessageId.cpp
)

set(SRC_SERVER
    Source/MailUnit/Application.h
    Source/MailUnit/Application.cpp
    Source/MailUnit/Config.h
    Source/MailUnit/Config.cpp
    Source/MailUnit/Logger.h
    Source/MailUnit/Logger.cpp
    Source/MailUnit/Exception.h
    Source/MailUnit/SmtpController.h
    Source/MailUnit/SmtpController.cpp
    Source/MailUnit/Email/Mime.h
    Source/MailUnit/Email/Mime.cpp
    Source/MailUnit/Email/Header.h
    Source/MailUnit/Email/Header.cpp
    Source/MailUnit/Email/Address.h
    Source/MailUnit/Email/Address.cpp
    Source/MailUnit/Email/DateTime.h
    Source/MailUnit/Email/DateTime.cpp
    Source/MailUnit/Smtp/ProtocolDef.h
    Source/MailUnit/Smtp/Server.h
    Source/MailUnit/Smtp/Server.cpp
    Source/MailUnit/Smtp/ResponseCode.h
    Source/MailUnit/Smtp/ResponseCode.cpp
    Source/MailUnit/Smtp/SessionProvider.h
    Source/MailUnit/Smtp/SessionProvider.cpp
    Source/MailUnit/Smtp/Message.h
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
    Source/MailUnit/Database/Sql.h
)

set(LIBS
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)


add_library(${TARGET_LIB} SHARED ${SRC_LIB})
target_link_libraries(${TARGET_LIB} ${LIBS})
target_compile_definitions(${TARGET_LIB} PRIVATE -D_MU_LIB)
set_target_properties(${TARGET_LIB} PROPERTIES OUTPUT_NAME ${BINARY_NAME})

add_executable(${TARGET_SERVER} ${SRC_SERVER})
target_compile_definitions(${TARGET_SERVER} PRIVATE -D_MU_SERVER)
target_link_libraries(${TARGET_SERVER} ${LIBS} ${TARGET_LIB})
set_target_properties(${TARGET_SERVER} PROPERTIES OUTPUT_NAME ${BINARY_NAME})
