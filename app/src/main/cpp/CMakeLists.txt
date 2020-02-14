# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


#set(FF ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI})
set(FF ../../../../libs/${CMAKE_ANDROID_ARCH_ABI})
message("FF=${FF}")

add_library(avcodec
        SHARED
        IMPORTED)
set_target_properties( # Specifies the target library.
        avcodec
        PROPERTIES IMPORTED_LOCATION
        ${FF}/libavcodec.so)

add_library(avformat
        SHARED
        IMPORTED)
set_target_properties(
        avformat
        PROPERTIES IMPORTED_LOCATION
        ${FF}/libavformat.so
)

add_library(avutil
        SHARED
        IMPORTED)
set_target_properties(
        avutil
        PROPERTIES IMPORTED_LOCATION
        ${FF}/libavutil.so
)
include_directories(./include)


add_library( # Sets the name of the library.
        native-lib
        SHARED
        native-lib.cpp)

find_library( # Sets the name of the path variable.
        log-lib
        log)

target_link_libraries( # Specifies the target library.
        native-lib
        avcodec
        avformat
        avutil
        ${log-lib}
        )
