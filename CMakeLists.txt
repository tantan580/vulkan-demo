cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
project(vulkan-demo)
SET(CMAKE_BUILD_TYPE Debug)
#add_definitions(-std=c++11)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF) # -std=c++11 instead of -std=gnu++11

if(WIN32)
    MESSAGE(STATUS “This is windows.”)
    target_link_libraries(${PROJECT_NAME} glfw3 opengl32)
else(WIN32)
    include_directories(./)
    include_directories(./common)
    #find_package(glfw3 REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(DEPS REQUIRED glfw3 gdk-pixbuf-2.0 vulkan)
    include_directories(${DEPS_INCLUDE_DIRS})
    #add_definitions(-DNDEBUG)
    #file(GLOB project_file main.cpp vulkanApp.cpp)
    #file(GLOB project_file main2.cpp)

    #add_executable(${PROJECT_NAME} ${project_file})

    set(3rdparty
        ${DEPS_LIBRARIES}
        #glfw3  #libglfw3.a
        -lGL
        -lpthread
        -ldl
        )


    add_subdirectory(example)
    add_subdirectory(sample)
endif()
