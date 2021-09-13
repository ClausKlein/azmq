cmake_minimum_required(VERSION 3.0)
project(azmq-super-build)

include(ExternalProject)

set(DEVROOT ${CMAKE_BINARY_DIR}/devroot)

set(CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=Debug
        -DCMAKE_PREFIX_PATH=${DEVROOT}
        -DCMAKE_INSTALL_PREFIX=${DEVROOT}
        )

ExternalProject_add(
        zmq
        CMAKE_ARGS ${CMAKE_ARGS} -DZMQ_BUILD_TESTS=OFF -DWITH_PERF_TOOL=OFF -DWITH_LIBSODIUM=OFF
        URL http://github.com/zeromq/libzmq/releases/download/v4.3.2/zeromq-4.3.2.tar.gz
        URL_HASH SHA256=ebd7b5c830d6428956b67a0454a7f8cbed1de74b3b01e5c33c5378e22740f763
)

ExternalProject_add(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.13.7
        CMAKE_ARGS ${CMAKE_ARGS} -DBUILD_TESTING=OFF -DCATCH_ENABLE_WERROR=OFF
)

find_package(Boost)
if (NOT Boost_FOUND)
    ExternalProject_Add(
            boost
            URL https://boostorg.jfrog.io/artifactory/main/release/1.76.0/source/boost_1_76_0.tar.bz2
            URL_HASH SHA256=f0397ba6e982c4450f27bf32a2a83292aba035b827a5623a14636ea583318c41
            BUILD_IN_SOURCE 1
            INSTALL_DIR ${DEVROOT}
            CONFIGURE_COMMAND ./bootstrap.sh --prefix=<INSTALL_DIR>
            BUILD_COMMAND ""
            INSTALL_COMMAND ./b2 install --prefix=<INSTALL_DIR>
    )
    set(extra_libs boost)
else()
    set(extra_libs "")
endif ()

ExternalProject_Add(
        azmq
        DEPENDS zmq catch2 ${extra_libs}
        CMAKE_ARGS ${CMAKE_ARGS} -DBUILD_TESTING=YES
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../..
        TEST_BEFORE_INSTALL YES
)
