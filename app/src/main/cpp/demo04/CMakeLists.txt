# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.


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

add_library(swscale
        SHARED
        IMPORTED)
set_target_properties(
        swscale
        PROPERTIES IMPORTED_LOCATION
        ${FF}/libswscale.so
)
include_directories(../include)


add_library( # Sets the name of the library.
        demo04-lib
        SHARED
        demo04.cpp)

find_library( # Sets the name of the path variable.
        log-lib
        log)

target_link_libraries( # Specifies the target library.
        demo04-lib
        avcodec
        avformat
        avutil
        swscale
        ${log-lib}
        )

