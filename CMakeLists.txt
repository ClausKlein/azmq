cmake_minimum_required(VERSION 3.13...3.21)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(azmq LANGUAGES CXX)

include(AzmqCPack.cmake)

# config setting:
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# TODO: use modern cmake and do not set global cmake variables! CK
#
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

if(${CMAKE_CXX_PLATFORM_ID} STREQUAL "Windows")
    # Build for Windows Vista / Windows Server 2008 and later
    add_definitions(-D_WIN32_WINNT=0x0600)

    add_definitions(-DNOMINMAX -DWIN32_LEAN_AND_MEAN -D_WINSOCK_DEPRECATED_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)

    if(NOT DEFINED Boost_USE_STATIC_LIBS)
        set(Boost_USE_STATIC_LIBS ON) # On Windows, Boost is compiled as static libraries by default
    endif()
endif()

# option
if(ZMQ_STATIC)
    add_definitions(-D ZMQ_STATIC)
endif()

set(ADDITIONAL_LIBS)

# option
if(USE_LIBCXX)
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lc++abi")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lc++abi")
    list(APPEND ADDITIONAL_LIBS "pthread")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config")

find_package(
    Boost 1.71
    COMPONENTS system date_time thread chrono regex random
    REQUIRED
)
find_package(ZeroMQ 4.0 REQUIRED)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${ZeroMQ_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# option
if(AZMQ_NO_DEPRECATED)
    add_definitions(-D BOOST_ASIO_NO_DEPRECATED)
endif()

# option
if(NOT AZMQ_NO_TESTS)
    enable_testing()

    macro(add_catch_test name)
        if(TEST_REPORT_FORMAT)
            add_test(NAME ${name} COMMAND ${name} -r ${TEST_REPORT_FORMAT} -o "${name}.test_out.xml")
        else()
            add_test(NAME ${name} COMMAND ${name})
        endif()
    endmacro()

    add_subdirectory(test)
endif()

add_subdirectory(doc)

# TODO: export cmake package config setting! CK
install(DIRECTORY ${PROJECT_SOURCE_DIR}/azmq DESTINATION include)
