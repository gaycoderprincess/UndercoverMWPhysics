cmake_minimum_required(VERSION 3.27)
project(NFSUCMWPhysics)
SET(PROJECT_SUFFIX "_gcp")

SET(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_CXX_STANDARD 20)

SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
SET(CMAKE_RANLIB i686-w64-mingw32-ranlib)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -O3 -D_NDEBUG -DNDEBUG")
    # keep disabled for now... causes issues
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto -fno-use-linker-plugin")
    if (NOT CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-exclude-libs,ALL -Wl,--gc-sections -Wl,--strip-all")
    endif ()
else ()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -D_DEBUG")
endif ()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof -Wl,--pdb=${PROJECT_NAME}${PROJECT_SUFFIX}.pdb -masm=intel -static")

add_compile_definitions(NYA_COMMON_NO_D3D SUSPENSIONRACER_ELISE_TEST)

file(GLOB_RECURSE PROJECT_FILES CONFIGURE_DEPENDS
    ./*.cpp
    ./*.h
    ./*.hpp
)

add_library(NFSUCMWPhysics SHARED ${PROJECT_FILES})

target_include_directories(NFSUCMWPhysics PUBLIC decomp)
target_include_directories(NFSUCMWPhysics PUBLIC ../nya-common)
target_include_directories(NFSUCMWPhysics PUBLIC ../nya-common/3rdparty)
target_include_directories(NFSUCMWPhysics PUBLIC ../nya-common-nfsuc)
target_include_directories(NFSUCMWPhysics PUBLIC ../CwoeeMenuLib)

set_target_properties(NFSUCMWPhysics PROPERTIES PREFIX "")
set_target_properties(NFSUCMWPhysics PROPERTIES SUFFIX "${PROJECT_SUFFIX}.asi")