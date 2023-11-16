# the minimum version of CMake.
cmake_minimum_required(VERSION 3.4.1)
project(gpu_transform)

set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${NATIVERENDER_ROOT_PATH}
                    ${NATIVERENDER_ROOT_PATH}/include
                    ${NATIVERENDER_ROOT_PATH}/util
                    ${NATIVERENDER_ROOT_PATH}/napi
                    ${NATIVERENDER_ROOT_PATH}/common
                    ${NATIVERENDER_ROOT_PATH}/render
                    ${NATIVERENDER_ROOT_PATH}/constant
                    )

add_library(nativeGpu SHARED
                    ${NATIVERENDER_ROOT_PATH}/napi/napi_init.cpp
                    ${NATIVERENDER_ROOT_PATH}/render/EGLRender.cpp
                    ${NATIVERENDER_ROOT_PATH}/util/GLUtils.cpp
                    ${NATIVERENDER_ROOT_PATH}/util/NapiUtil.cpp
                    )

find_library (
            hilog-lib
            hilog_ndk.z )
find_library (
            EGL-lib
            EGL )
find_library (
            GLES-lib
            GLESv3 )

target_link_libraries(nativeGpu PUBLIC ${hilog-lib} ${EGL-lib} ${GLES-lib} libace_napi.z.so libc++.a)