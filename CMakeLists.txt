cmake_minimum_required(VERSION 3.0)

project(csg_testbed)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

#VTK 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
message( ${VTK_USE_FILE})

#JSON
include_directories(${PROJECT_SOURCE_DIR}/third_party/json/src) 

include_directories(${PROJECT_SOURCE_DIR}/include/) 

FILE(GLOB_RECURSE APP_HEADERS "include/*.h")
message("APP Headers: " ${APP_HEADERS})

FILE(GLOB_RECURSE APP_SOURCES "src/*.cpp")
message("APP Sources: " ${APP_SOURCES})

add_executable(${PROJECT_NAME} ${APP_HEADERS} ${APP_SOURCES})
target_link_libraries(${PROJECT_NAME}  ${VTK_LIBRARIES})


