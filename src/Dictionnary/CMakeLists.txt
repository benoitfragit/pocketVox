include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Dictionnary pocketvox_dictionnary.c)
target_include_directories(Notifier PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
