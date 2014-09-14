include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Notifier pocketvox_notifier.c)
target_include_directories(Notifier PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
