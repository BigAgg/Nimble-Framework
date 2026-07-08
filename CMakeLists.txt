cmake_minimum_required(VERSION 3.20)
project(APP VERSION 1.0 LANGUAGES CXX)

option(APP_BUILD_SHARED "Build shared library" ON)

# ---- standard ----
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)

# ---- options ----
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# ---- sources ----
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

# ---- headers ----
file(GLOB_RECURSE APP_HEADERS CONFIGURE_DEPENDS
    include/**/*.h
    include/**/*.hpp
    src/**/*.h
    src/**/*.hpp
)

# ---- dependencies ----
include(cmake/dependencies.cmake)

# ---- library ----
add_library(${PROJECT_NAME}
    ${APP_SOURCES}
    ${APP_HEADERS}
)

# static / shared toggle
if(APP_BUILD_SHARED)
    set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# ---- visual studio filters ----
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
    FILES ${APP_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES ${APP_HEADERS}
)

# ---- include dirs ----
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ---- link deps ----
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        raylib
        imgui
        rlimgui
        nfd
        nlohmann_json::nlohmann_json
)

# ---- export for FetchContent ----
include(GNUInstallDirs)

install(DIRECTORY include/ DESTINATION include)
