cmake_minimum_required(VERSION 3.13...3.21)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(azmq LANGUAGES CXX)

if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CMAKE_MASTER_PROJECT YES)
endif()

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

set(ADDITIONAL_LIBS)

# TODO: unused option? CK
if(USE_LIBCXX)
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lc++abi")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lc++abi")
    list(APPEND ADDITIONAL_LIBS "pthread")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config")

# ---- dependency ----

find_package(
    Boost 1.71
    COMPONENTS system date_time thread chrono regex random
    REQUIRED
)

#FIXME: set(ZeroMQ_ROOT ../libzmq/stage)
# use CONFIG package:  set(ZeroMQ_DIR /Users/clausklein/Workspace/cpp/libzmq/stage/lib/cmake/ZeroMQ)
#XXX find_package(ZeroMQ 4.3 CONFIG REQUIRED)
find_package(ZeroMQ 4.1 REQUIRED)

#TODO: add missing namspace ZeroMQ:: to cmake config package: add_library(ZeroMQ::libzmq SHARED IMPORTED)
#XXX add_library(ZeroMQ::libzmq ALIAS libzmq)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

# ---- Create library ----

# Note: for header-only libraries change all PUBLIC flags to INTERFACE
add_library(${PROJECT_NAME} INTERFACE)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD})
target_include_directories(
    ${PROJECT_NAME} BEFORE INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}> $<INSTALL_INTERFACE:include>
)
# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(${PROJECT_NAME} INTERFACE "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

#XXX target_link_libraries(${PROJECT_NAME} INTERFACE ZeroMQ::libzmq Boost::thread)
target_link_libraries(${PROJECT_NAME} INTERFACE ${CMAKE_THREAD_LIBS_INIT} ${ZeroMQ_LIBRARIES} ${Boost_LIBRARIES} ${ADDITIONAL_LIBS})
include_directories(${Boost_INCLUDE_DIRS} ${ZeroMQ_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

add_library(ZeroMQ::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# TODO: unused option? CK
if(ZMQ_STATIC)
    target_compile_definitions(${PROJECT_NAME} INTERFACE ZMQ_STATIC)
endif()

# FIXME: this option does not comile yet! CK
if(AZMQ_NO_DEPRECATED)
    target_compile_definitions(${PROJECT_NAME} INTERFACE BOOST_ASIO_NO_DEPRECATED)
endif()

option(BUILD_TESTS "Build no tests" ${CMAKE_MASTER_PROJECT})
if(NOT AZMQ_NO_TESTS AND BUILD_TESTS)
    enable_testing()

    macro(add_catch_test name)
        target_link_libraries(${name} PRIVATE ZeroMQ::azmq)
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
