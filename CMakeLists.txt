cmake_minimum_required(VERSION 3.29)
project(PlanetWalker)

set(CMAKE_C_STANDARD 23)

# Include the command that downloads libraries
include(FetchContent)

# define a function for adding git dependencies
function(include_dependency libName gitURL gitTag)
    # setup the declare
    FetchContent_Declare(${libName}
            GIT_REPOSITORY ${gitURL}
            GIT_TAG        ${gitTag}
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
    )

    FetchContent_MakeAvailable(${libName})
endfunction()

# add raylib support
set(LIB1 raylib)
find_package(${LIB1} QUIET)
if (NOT ${LIB1}_FOUND)
    message(STATUS "Getting ${LIB1} from Github")
    include_dependency(${LIB1} https://github.com/raysan5/raylib.git 5.5)
else()
    message(STATUS "Using local ${LIB1}")
endif()

add_executable(PlanetWalker src/Main.cpp
        src/Game.cpp
        src/Game.h
        src/entities/Moon.h
        src/entities/Moon.cpp
        src/entities/Sky.cpp
        src/entities/Sky.h
        src/entities/Player.cpp
        src/entities/Player.h
        src/entities/Planet.cpp
        src/entities/Planet.h
        src/Configuration.cpp
        src/Configuration.h
        src/Renderer.cpp
        src/Renderer.h
        src/physics/RigidBody.cpp
        src/physics/RigidBody.h
        src/physics/ForceGenerator.cpp
        src/physics/ForceGenerator.h
        src/physics/Gravity.cpp
        src/physics/Gravity.h
        src/ModelManager.cpp
        src/ModelManager.h
        src/input/InputManager.cpp
        src/input/InputManager.h
        src/input/Mouse.cpp
        src/input/Mouse.h
        src/input/Keyboard.cpp
        src/input/Keyboard.h
        src/PlayerCamera.cpp
        src/PlayerCamera.h
        src/physics/Physics.cpp
        src/physics/Physics.h
        src/Scene.cpp
        src/Scene.h
)

# set the include directory
target_include_directories(PlanetWalker PRIVATE ${raylib_INCLUDE_DIRS})

# link all libraries to the project
target_link_libraries(PlanetWalker PRIVATE ${LIB1})

