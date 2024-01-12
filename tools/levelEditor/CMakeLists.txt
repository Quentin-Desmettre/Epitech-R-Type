cmake_minimum_required(VERSION 3.5)

project(level-editor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (NOT WIN32)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif ()

include_directories(include)

set(SRC src/main.cpp
        src/Editor.cpp
        src/Window.cpp
        include/Editor.hpp
        include/Window.hpp
)

add_executable(level-editor ${SRC})

target_link_libraries(level-editor PRIVATE sfml-graphics sfml-window sfml-system)

if (WIN32 AND BUILD_SHARED_LIBS)
    target_link_libraries(level-editor PRIVATE ws2_32.lib)
    add_custom_command(TARGET level-editor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:level-editor> $<TARGET_FILE_DIR:level-editor> COMMAND_EXPAND_LISTS)
    set(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_SOURCE_DIR}/Compilation/windows/windows.cmake")
endif ()

if (UNIX)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation directory")
    set(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_SOURCE_DIR}/Compilation/linux/linux.cmake")
endif ()

if (APPLE AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(level-editor PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "@executable_path/../lib"
    )
    set(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_SOURCE_DIR}/Compilation/macos/macos.cmake")
endif()

install(TARGETS level-editor DESTINATION bin)
install(DIRECTORY assets DESTINATION .)

set(CPACK_PACKAGE_VERSION_MAJOR "${RTYPE_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${RTYPE_MINOR_VERSION}")
set(CPACK_BUNDLE_NAME "R-Type")
include(CPack)
