file(GLOB_RECURSE SRCS "src/*.cc")
add_library(webrtc ${SRCS})
target_link_libraries(webrtc
    absl::meta
    pthread
    X11
    Xcomposite
    Xdamage
    Xext
    Xfixes
    Xrender
    Xrandr
    Xtst)
target_compile_definitions(webrtc PRIVATE WEBRTC_POSIX=1 WEBRTC_LINUX=1)
target_include_directories(webrtc PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src)
