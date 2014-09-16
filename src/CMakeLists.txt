cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(POCKET_VOX)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -O2 -g3")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(LIBNOTIFY REQUIRED libnotify)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(POCKETSPHINX REQUIRED pocketsphinx)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-0.10)
pkg_check_modules(APPINDICATOR REQUIRED appindicator3-0.1)

include_directories(${GTK3_INCLUDE_DIRS})
include_directories(${LIBNOTIFY_INCLUDE_DIRS})
include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${POCKETSPHINX_INCLUDE_DIRS})
include_directories(${GSTREAMER_INCLUDE_DIRS})
include_directories(${APPINDICATOR_INCLUDE_DIRS})

link_directories(${GTK3_LIBRARY_DIRS})
link_directories(${LIBNOTIFY_LIBRARY_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})
link_directories(${POCKETSPHINX_LIBRARY_DIRS})
link_directories(${GSTREAMER_LIBRARY_DIRS})
link_directories(${APPINDICATOR_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})
add_definitions(${LIBNOTIFY_CFLAGS_OTHER})
add_definitions(${GLIB_CFLAGS_OTHER})
add_definitions(${POCKETSPHINX_CFLAGS_OTHER})
add_definitions(${GSTREAMER_CFLAGS_OTHER})
add_definitions(${APPINDICATOR_CFLAGS_OTHER})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(Recognizer)
add_subdirectory(Notifier)
add_subdirectory(Dictionnary)
add_subdirectory(Indicator)

set(SOURCES main.c)

add_executable(pocketvox ${SOURCES})
target_link_libraries (pocketvox LINK_PUBLIC Indicator Dictionnary Notifier Recognizer ${GTK3_LIBRARIES} ${LIBNOTIFY_LIBRARIES} ${GLIB_LIBRARIES} ${POCKETSPHINX_LIBRARIES}  ${GSTREAMER_LIBRARIES} ${APPINDICATOR_LIBRARIES})
