cmake_minimum_required(VERSION 3.8)
project(MyFS)

#set(CMAKE_VRBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(MKFS
        src/mkfs.myfs.cpp
        src/blockdevice.cpp)

set(MOUNT
        src/blockdevice.cpp
        src/myfs.cpp
        src/wrap.cpp
        src/mount.myfs.c)

set(UNITTESTS
        src/blockdevice.cpp
        src/myfs.cpp
        unittests/main.cpp
        unittests/test-blockdevice.cpp
        unittests/test-myfs.cpp
        unittests/helper.cpp)

include_directories(includes)

set(COMPILE_FLAGS "-Wall -DFUSE_USE_VERSION=26")

add_definitions(${COMPILE_FLAGS})

add_executable(mkfs.myfs ${MKFS})
add_executable(mount.myfs ${MOUNT})
add_executable(unittests ${UNITTESTS})

find_package(PkgConfig)
pkg_check_modules(FUSE fuse)

target_link_libraries(mkfs.myfs ${FUSE_LDFLAGS})
target_compile_options(mkfs.myfs PUBLIC ${FUSE_CFLAGS})
target_include_directories(mkfs.myfs PUBLIC ${FUSE_INCLUDE_DIRS})

target_link_libraries(mount.myfs ${FUSE_LDFLAGS})
target_compile_options(mount.myfs PUBLIC ${FUSE_CFLAGS})
target_include_directories(mount.myfs PUBLIC ${FUSE_INCLUDE_DIRS})

target_link_libraries(unittests ${FUSE_LDFLAGS})
target_compile_options(unittests PUBLIC ${FUSE_CFLAGS})
target_include_directories(unittests PUBLIC ${FUSE_INCLUDE_DIRS})
