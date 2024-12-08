cmake_minimum_required(VERSION 3.0)

project(multiboy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB_RECURSE SRC src/*.cpp)

if(WIN32)
    add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SRC})
else()
    add_executable(${CMAKE_PROJECT_NAME} ${SRC})
endif()

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
}

set(BUILD_STATIC_DEFAULT OFF)
if(WIN32)
    set(BUILD_STATIC_DEFAULT ON)
endif()
option(BUILD_STATIC "Build a static binary" ${BUILD_STATIC_DEFAULT})

if(BUILD_STATIC)
    # Build libraries
    set(BUILD_STATIC_LIBS ON)
    set(BUILD_SHARED_LIBS OFF)
    set(BUILD_OBJECT_LIBS OFF)

    set(LIBMGBA_ONLY ON)

    add_subdirectory(vendor/mgba)

    include_directories(
        vendor/mbga/include/mgba
    }

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SDL2_STATIC_LIBRARIES}
        ${SDL2_TTF_STATIC_LIBRARIES}
        mgba
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        mgba
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/roms ${CMAKE_CURRENT_BINARY_DIR}/roms
)
