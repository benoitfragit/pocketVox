include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(Recognizer pocketvox_recognizer.c)
target_include_directories(Recognizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
