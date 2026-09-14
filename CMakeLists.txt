cmake_minimum_required(VERSION 3.20)

project(android_files_backup LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

add_library(android_files_backup_lib STATIC
    src/application/application_controller.cpp

    src/backup/backup_service.cpp

    src/ui/cli_application/cli_app_core.cpp

    src/ui/cli_application/operations/cli_app_backup.cpp  
    src/ui/cli_application/operations/cli_app_device.cpp  
    src/ui/cli_application/operations/cli_app_local_novigation.cpp  
    src/ui/cli_application/operations/cli_app_main_menu.cpp  
    src/ui/cli_application/operations/cli_app_remote_navigation.cpp
    
    src/ui/cli_application/helpers/cli_app_basic_input.cpp  
    src/ui/cli_application/helpers/cli_app_basic_output.cpp  
    src/ui/cli_application/helpers/cli_app_show_menu.cpp


    src/process/process_runner.cpp

    src/adb/adb_device.cpp
    src/adb/adb_file.cpp

    src/adb/adb_client/adb_devices.cpp
    src/adb/adb_client/adb_directories.cpp
    src/adb/adb_client/adb_files.cpp
    src/adb/adb_client/adb_runner.cpp

    src/adb/adb_parsers/adb_parse_device_line.cpp
    src/adb/adb_parsers/adb_parse_file_line.cpp

    src/utils/utils.cpp
)

target_include_directories(android_files_backup_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(android_files_backup_lib
    PUBLIC
        Qt6::Core
)

target_compile_options(android_files_backup_lib
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

qt_add_executable(android_files_backup
    src/main.cpp
)

target_link_libraries(android_files_backup
    PRIVATE
        android_files_backup_lib
)


add_executable(TEST_adb_file_parser
    tests/adb/parsers/file_parser/file_parser.cpp
)

target_link_libraries(TEST_adb_file_parser
    PRIVATE
        android_files_backup_lib
)