cmake_minimum_required(VERSION 3.10)
project(rush_hour_solver)

# Mengatur standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Mengatur output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Mencari semua source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*/*.cpp"
)

# Menambahkan executable
add_executable(rush_hour ${SOURCES})

# Menambahkan include directories
target_include_directories(rush_hour PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
)

# Windows specific settings
if(WIN32)
    target_compile_definitions(rush_hour PRIVATE _WIN32)
endif()

