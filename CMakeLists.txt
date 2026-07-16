cmake_minimum_required(VERSION 3.20)

project(android_files_backup LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(android_files_backup
    src/main.cpp
    src/application/application_controller.cpp
    src/process/process_runner.cpp
    src/adb/adb_device.cpp
    src/adb/adb_client.cpp
    src/utils/utils.cpp
)

target_include_directories(android_files_backup
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(android_files_backup
    PRIVATE
        Qt6::Core
)

target_compile_options(android_files_backup
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)