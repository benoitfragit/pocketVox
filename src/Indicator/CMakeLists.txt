include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Indicator pocketvox_indicator.c)
target_include_directories(Indicator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
