cmake_minimum_required(VERSION 3.5)
project(drc-remote-desktop VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBDRC REQUIRED IMPORTED_TARGET libdrc)
pkg_check_modules(LIBSWSCALE REQUIRED IMPORTED_TARGET libswscale)
pkg_check_modules(X264 REQUIRED IMPORTED_TARGET x264)

add_subdirectory(third_party/abseil-cpp)
add_subdirectory(third_party/webrtc)

file(GLOB SRCS
    main.cc
    vector_desktop_frame.cc)
add_executable(drc-remote-desktop ${SRCS})
target_link_libraries(drc-remote-desktop
    webrtc
    PkgConfig::LIBDRC
    PkgConfig::LIBSWSCALE
    PkgConfig::X264)
