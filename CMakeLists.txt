cmake_minimum_required(VERSION 3.23)
project(out VERSION 1.0.0)

include(CTest)
## TODO: CDash integration: page 316

## Header-only library
add_library(out INTERFACE)
## Alias
add_library(out::out ALIAS out)

target_compile_features(out INTERFACE cxx_std_20)

target_include_directories(out INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

if (BUILD_TESTING) # option is created by CTest
    enable_testing()
    add_subdirectory(tests)
endif ()

option(ENABLE_FUZZING "Enable fuzzing test builds" OFF)
if(ENABLE_FUZZING)
    message("Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html")
    add_subdirectory(fuzzing)
endif()

## Installation + CMake config files
##
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS out
        EXPORT outExportTarget
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

install(EXPORT outExportTarget
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        NAMESPACE ${PROJECT_NAME}::
        FILE outConfig.cmake
        )

write_basic_package_version_file(
        outConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
        )

install(FILES
        outConfig.cmake
        ${PROJECT_BINARY_DIR}/outConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        )
