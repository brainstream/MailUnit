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
find_package(Boost REQUIRED COMPONENTS system)

include_directories(
    ${Boost_INCLUDE_DIRS}
)

set(SRC
    src/Main.cpp
    src/Aux.h
    src/SmtpController.h
    src/SmtpController.cpp
    src/Smtp/Server.h
    src/Smtp/Server.cpp
    src/Smtp/ResponseCode.h
    src/Smtp/ResponseCode.cpp
    src/Smtp/SessionProvider.h
    src/Smtp/SessionProvider.cpp
    src/Smtp/Message.h
    src/Smtp/StateMachine/StateMachine.h
    src/Smtp/StateMachine/StateBase.h
    src/Smtp/StateMachine/StartState.h
    src/Smtp/StateMachine/EhloState.h
    src/Smtp/StateMachine/MailFromState.h
    src/Smtp/StateMachine/RcptToState.h
    src/Smtp/StateMachine/DataState.h
    src/Smtp/StateMachine/QuitState.h
)

set(LIBS
    ${Boost_LIBRARIES}
)

string(TOLOWER ${PROJECT_NAME} TARGETNAME)
add_executable(${TARGETNAME} ${SRC})
target_link_libraries(${TARGETNAME} ${LIBS})
