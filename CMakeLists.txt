cmake_minimum_required(VERSION 3.16)
project(main LANGUAGES C CXX)   # <-- enable C because glad is C

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

add_executable(main main.cpp engine.cpp image_loader.cpp)

find_package(SDL3 REQUIRED CONFIG)
find_package(OpenGL REQUIRED)

# --- GLAD (vendored from your zip) ---
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/external/glad")
set(GLAD_INCLUDE_DIR "${GLAD_DIR}/include")

# glad2 layout: src/gl.c ; glad1 layout: src/glad.c
if (EXISTS "${GLAD_DIR}/src/gl.c")
    set(GLAD_SRC "${GLAD_DIR}/src/gl.c")          # glad2
elseif (EXISTS "${GLAD_DIR}/src/glad.c")
    set(GLAD_SRC "${GLAD_DIR}/src/glad.c")        # glad1
else()
    message(FATAL_ERROR
        "Couldn't find GLAD source.\n"
        "Expected: ${GLAD_DIR}/src/gl.c (glad2) or ${GLAD_DIR}/src/glad.c (glad1).")
endif()

add_library(glad STATIC "${GLAD_SRC}")
target_include_directories(glad PUBLIC "${GLAD_INCLUDE_DIR}")
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})  # libdl for dlsym()

# --- Link everything ---
target_link_libraries(main PRIVATE SDL3::SDL3 glad OpenGL::GL)
target_include_directories(main PRIVATE "${GLAD_INCLUDE_DIR}")
