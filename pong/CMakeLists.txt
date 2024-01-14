cmake_minimum_required(VERSION 3.5)

project(pong LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (NOT WIN32)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif ()

include_directories(include)
include_directories(include/common)
include_directories(include/common/ecs)
include_directories(include/common/network)
include_directories(include/common/network/components)
include_directories(include/common/network/systems)
include_directories(include/server)
include_directories(include/game)
include_directories(include/game/components)
include_directories(include/game/systems)


set(SHARED_SRC
        src/common/ecs/AbstractComponent.cpp
        include/common/ecs/AbstractComponent.hpp
        src/common/ecs/Entity.cpp
        include/common/ecs/Entity.hpp
        src/common/ecs/StaticPacketBuilder.cpp
        include/common/ecs/StaticPacketBuilder.hpp
        src/common/ecs/StaticPacketParser.cpp
        include/common/ecs/StaticPacketParser.hpp
        src/common/ecs/SystemBase.cpp
        include/common/ecs/SystemBase.hpp
        src/common/ecs/World.cpp
        include/common/ecs/World.hpp
        src/game/EntityFactory.cpp
        include/game/EntityFactory.hpp
        include/common/network/components/ClientPingComponent.hpp
        src/common/network/components/ClientAddressComponent.cpp
        include/common/network/components/ClientAddressComponent.hpp
        src/shared/PacketBuilder.cpp
        include/shared/PacketBuilder.hpp
        src/shared/ArgParser.cpp
        include/shared/ArgParser.hpp
        include/common/network/NetworkGlobals.hpp
        include/common/PlayerComponent.hpp
        src/common/PlayerComponent.cpp
        include/game/components/SpriteComponent.hpp
        src/game/components/SpriteComponent.cpp
        include/game/components/PositionComponent.hpp
        src/game/components/PositionComponent.cpp
        src/game/components/TextComponent.cpp
        include/game/components/TextComponent.hpp
        src/game/components/VelocityComponent.cpp
        include/game/components/VelocityComponent.hpp
        include/game/systems/ControlPlayerSystem.hpp
        src/game/systems/ControlPlayerSystem.cpp
        include/game/systems/PhysicsSystem.hpp
        src/game/systems/PhysicsSystem.cpp
        include/shared/SFMLLoader.hpp
        src/shared/SFMLLoader.cpp
        src/shared/Menu.cpp
        src/game/systems/MenuInputSystem.cpp
        src/game/components/InputComponent.cpp
        include/game/components/InputComponent.hpp
        include/game/systems/ButtonSystem.hpp
        src/game/systems/ButtonSystem.cpp
        include/game/components/BallComponent.hpp
        src/game/components/BallComponent.cpp
        src/game/systems/BallSystem.cpp
        include/game/systems/BallSystem.hpp
        src/game/systems/AISystem.cpp
        src/game/components/TagComponent.cpp
        include/game/components/TagComponent.hpp
)

set(SERVER_SRC
        src/server/main.cpp
        src/server/PongServer.cpp
        include/server/PongServer.hpp
        src/common/network/systems/ServerInputsSystem.cpp
        include/common/network/systems/ServerInputsSystem.hpp
        include/common/network/systems/NewConnectionSystem.hpp
        src/common/network/systems/NewConnectionSystem.cpp
        include/common/network/systems/DeleteClientSystem.hpp
        src/common/network/systems/DeleteClientSystem.cpp
        include/common/network/systems/PlayerOnConnectionSystem.hpp
        src/common/network/systems/PlayerOnConnectionSystem.cpp
)

set(CLIENT_SRC
        src/game/main.cpp
        src/game/PongClient.cpp
        include/game/PongClient.hpp
        include/game/systems/RenderSystem.hpp
        src/game/systems/RenderSystem.cpp
        include/common/network/systems/ClientServerDataHandlerSystem.hpp
        src/common/network/systems/ClientServerDataHandlerSystem.cpp
        src/common/network/systems/ServerConnectionSystem.cpp
        include/common/network/systems/ServerConnectionSystem.hpp
        src/common/network/systems/ClientInputSenderSystem.cpp
        include/common/network/systems/ClientInputSenderSystem.hpp
        include/common/network/systems/ClientPingSystem.hpp
        src/common/network/systems/ClientPingSystem.cpp
        include/shared/Menu.hpp
        include/game/systems/MenuInputSystem.hpp
        include/game/systems/AISystem.hpp
        src/game/systems/ScoreSystem.cpp
        include/game/systems/ScoreSystem.hpp
)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

INCLUDE_DIRECTORIES(${SFML_SOURCE_DIR}/include)

add_library(pong_static STATIC ${SHARED_SRC})

target_link_libraries(pong_static PUBLIC sfml-graphics sfml-network sfml-audio m)

add_executable(pong_client ${CLIENT_SRC})
add_executable(pong_server ${SERVER_SRC})

target_link_libraries(pong_client PRIVATE pong_static)
target_link_libraries(pong_server PRIVATE pong_static)

if (WIN32 AND BUILD_SHARED_LIBS)
    target_link_libraries(pong_client PRIVATE ws2_32.lib)
    target_link_libraries(pong_server PRIVATE ws2_32.lib)
    add_custom_command(TARGET pong_client POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:pong_client> $<TARGET_FILE_DIR:pong_client> COMMAND_EXPAND_LISTS)
    add_custom_command(TARGET pong_server POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:pong_server> $<TARGET_FILE_DIR:pong_server> COMMAND_EXPAND_LISTS)
    set(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_SOURCE_DIR}/Compilation/windows/windows.cmake")
endif ()

if (UNIX)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation directory")
    set(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_SOURCE_DIR}/Compilation/linux/linux.cmake")
endif ()

if (APPLE AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(pong_client PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "@executable_path/../lib"
    )
    set_target_properties(pong_server PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "@executable_path/../lib"
    )
    set(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_SOURCE_DIR}/Compilation/macos/macos.cmake")
endif ()

install(TARGETS pong_client pong_server DESTINATION bin)
install(DIRECTORY assets DESTINATION .)

set(CPACK_PACKAGE_VERSION_MAJOR "${RTYPE_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${RTYPE_MINOR_VERSION}")
set(CPACK_BUNDLE_NAME "Pong")
include(CPack)
