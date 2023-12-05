cmake_minimum_required(VERSION 3.5)

project(r-type LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(SHARED_SRC src/shared/main.cpp
        include/aecs/World.hpp
        include/aecs/AbstractComponent.hpp
        src/aecs/AbstractComponent.cpp
        src/aecs/Entity.cpp
        include/aecs/Entity.hpp
        src/aecs/World.cpp
        include/aecs/SystemBase.hpp)
set(SERVER_SRC src/server/main.cpp)
set(CLIENT_SRC src/client/main.cpp
)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(r-type_client ${CLIENT_SRC} ${SHARED_SRC})
add_executable(r-type_server ${SERVER_SRC} ${SHARED_SRC})

target_link_libraries(r-type_client PRIVATE sfml-graphics sfml-network)
target_link_libraries(r-type_server
        PRIVATE
        sfml-graphics
        sfml-network
)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET r-type_client POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:r-type_client> $<TARGET_FILE_DIR:r-type_client> COMMAND_EXPAND_LISTS)
endif ()
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET r-type_server POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:r-type_server> $<TARGET_FILE_DIR:r-type_server> COMMAND_EXPAND_LISTS)
endif ()

install(TARGETS r-type_client r-type_server)

