cmake_minimum_required(VERSION 3.5)

project(r-type LANGUAGES CXX)
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

set(SHARED_SRC
        # ECS
        src/aecs/AbstractComponent.cpp
        src/aecs/Entity.cpp
        src/aecs/StaticPacketBuilder.cpp
        src/aecs/StaticPacketParser.cpp
        src/aecs/SystemBase.cpp
        src/aecs/World.cpp
        include/aecs/AbstractComponent.hpp
        include/aecs/Entity.hpp
        include/aecs/StaticPacketBuilder.hpp
        include/aecs/StaticPacketParser.hpp
        include/aecs/SystemBase.hpp
        include/aecs/World.hpp

        # Other
        include/rtype/EntityFactory.hpp
        include/rtype/NetworkGlobals.hpp
        include/shared/PacketBuilder.hpp
        src/shared/PacketBuilder.cpp
        src/rtype/EntityFactory.cpp

        # Components
        include/rtype/components/AnimComponent.hpp
        include/rtype/components/BulletComponent.hpp
        include/rtype/components/ClientAdressComponent.hpp
        include/rtype/components/ClientPingComponent.hpp
        include/rtype/components/DamageCollisionComponent.hpp
        include/rtype/components/HPComponent.hpp
        include/rtype/components/MonsterComponent.hpp
        include/rtype/components/ParallaxComponent.hpp
        include/rtype/components/PositionComponent.hpp
        include/rtype/components/SpriteComponent.hpp
        include/rtype/components/VelocityComponent.hpp
        include/rtype/components/PowerComponent.hpp
        src/rtype/components/BulletComponent.cpp
        src/rtype/components/DamageCollisionComponent.cpp
        src/rtype/components/HPComponent.cpp
        src/rtype/components/MonsterComponent.cpp
        src/rtype/components/PlayerComponent.cpp
        src/rtype/components/PositionComponent.cpp
        src/rtype/components/SpriteComponent.cpp
        src/rtype/components/VelocityComponent.cpp
        src/rtype/components/PowerComponent.cpp

        # Systems
        include/rtype/systems/shared/AnimPlayerSystem.hpp
        include/rtype/systems/shared/AnimSystem.hpp
        include/rtype/systems/shared/BulletSystem.hpp
        include/rtype/systems/shared/ControlPlayerSystem.hpp
        include/rtype/systems/shared/DamageCollisionSystem.hpp
        include/rtype/systems/shared/InvulSystem.hpp
        include/rtype/systems/shared/MonsterBullet.hpp
        include/rtype/systems/shared/MonsterGenSystem.hpp
        include/rtype/systems/shared/ParallaxSystem.hpp
        include/rtype/systems/shared/PhysicsSystem.hpp
        src/rtype/systems/shared/AnimPlayerSystem.cpp
        src/rtype/systems/shared/AnimSystem.cpp
        src/rtype/systems/shared/BulletSystem.cpp
        src/rtype/systems/shared/ControlPlayerSystem.cpp
        src/rtype/systems/shared/DamageCollisionSystem.cpp
        src/rtype/systems/shared/InvulSystem.cpp
        src/rtype/systems/shared/MonsterBullet.cpp
        src/rtype/systems/shared/MonsterGenSystem.cpp
        src/rtype/systems/shared/ParallaxSystem.cpp
        src/rtype/systems/shared/PhysicsSystem.cpp
        src/shared/ArgParser.cpp
        include/shared/ArgParser.hpp
        include/shared/Utils.hpp
        src/rtype/systems/server/MapSystem.cpp
        include/rtype/systems/server/MapSystem.hpp
        include/rtype/components/CollidableComponent.hpp
        src/rtype/components/CollidableComponent.cpp
        src/rtype/components/BlockComponent.cpp
        include/rtype/components/BlockComponent.hpp
)
set(SERVER_SRC src/server/main.cpp
        src/server/RTypeServer.cpp
        include/rtype/RTypeServer.hpp
        include/rtype/systems/server/NewConnectionSystem.hpp
        include/rtype/systems/server/ServerInputsSystem.hpp
        include/rtype/systems/server/ServerCorrectionsSystem.hpp
        include/rtype/systems/server/DeleteClientSystem.hpp
        include/rtype/systems/server/PlayerOnConnectionSystem.hpp
        include/rtype/systems/server/PowerSystem.hpp
        src/rtype/systems/server/DeleteClientSystem.cpp
        src/rtype/systems/server/NewConnectionSystem.cpp
        src/rtype/systems/server/PlayerOnConnectionSystem.cpp
        src/rtype/systems/server/ServerCorrectionsSystem.cpp
        src/rtype/systems/server/ServerInputsSystem.cpp
        src/rtype/systems/server/PowerSystem.cpp
)
set(CLIENT_SRC src/client/main.cpp
        src/client/RTypeClient.cpp
        include/rtype/RTypeClient.hpp
        src/rtype/systems/client/RenderSystem.cpp
        include/rtype/systems/client/RenderSystem.hpp
        include/rtype/systems/client/ServerConnectionSystem.hpp
        include/rtype/systems/client/ClientInputSenderSystem.hpp
        src/rtype/systems/client/ClientServerDataHandlerSystem.cpp
        include/rtype/systems/client/ClientPingSystem.hpp
        src/rtype/systems/client/ClientInputSenderSystem.cpp
        src/rtype/systems/client/ClientPingSystem.cpp
        src/rtype/systems/client/ServerConnectionSystem.cpp
        include/rtype/systems/client/DamageSoundSystem.hpp
        include/rtype/components/MusicComponent.hpp
        src/rtype/components/MusicComponent.cpp
        src/rtype/systems/client/DamageSoundSystem.cpp
)


include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

INCLUDE_DIRECTORIES(${SFML_SOURCE_DIR}/include)

add_library(r-type_static STATIC ${SHARED_SRC})

target_link_libraries(r-type_static PUBLIC sfml-graphics sfml-network sfml-audio)

add_executable(r-type_client ${CLIENT_SRC})
add_executable(r-type_server ${SERVER_SRC})

target_link_libraries(r-type_client PRIVATE r-type_static)
target_link_libraries(r-type_server PRIVATE r-type_static)

if (WIN32 AND BUILD_SHARED_LIBS)
    target_link_libraries(r-type_client PRIVATE ws2_32.lib)
    target_link_libraries(r-type_server PRIVATE ws2_32.lib)
    add_custom_command(TARGET r-type_client POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:r-type_client> $<TARGET_FILE_DIR:r-type_client> COMMAND_EXPAND_LISTS)
    add_custom_command(TARGET r-type_server POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:r-type_server> $<TARGET_FILE_DIR:r-type_server> COMMAND_EXPAND_LISTS)
    set(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_SOURCE_DIR}/Compilation/windows/windows.cmake")
endif ()

if (UNIX)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation directory")
    set(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_SOURCE_DIR}/Compilation/linux/linux.cmake")
endif ()

if (APPLE AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(r-type_client PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "@executable_path/../lib"
    )
    set_target_properties(r-type_server PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "@executable_path/../lib"
    )
    set(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_SOURCE_DIR}/Compilation/macos/macos.cmake")
endif()

install(TARGETS r-type_client r-type_server DESTINATION bin)
install(DIRECTORY assets DESTINATION .)
install(DIRECTORY shaders DESTINATION .)

set(CPACK_PACKAGE_VERSION_MAJOR "${RTYPE_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${RTYPE_MINOR_VERSION}")
set(CPACK_BUNDLE_NAME "R-Type")
include(CPack)
