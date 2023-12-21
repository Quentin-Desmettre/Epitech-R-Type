cmake_minimum_required(VERSION 3.5)

project(r-type LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(SHARED_SRC
        include/aecs/World.hpp
        include/aecs/AbstractComponent.hpp
        src/aecs/AbstractComponent.cpp
        src/aecs/Entity.cpp
        include/aecs/Entity.hpp
        src/aecs/World.cpp
        src/aecs/SystemBase.cpp
        include/aecs/SystemBase.hpp
        include/rtype/EntityFactory.hpp
        src/rtype/EntityFactory.cpp
        include/rtype/systems/InvulSystem.hpp
        include/rtype/systems/MonsterGenSystem.hpp
        src/rtype/systems/MonsterGenSystem.cpp
        include/rtype/components/MonsterComponent.hpp
        include/rtype/systems/MonsterBullet.hpp
        src/rtype/components/SpriteComponent.cpp
        include/rtype/components/SpriteComponent.hpp
        include/rtype/components/PositionComponent.hpp
        include/rtype/components/VelocityComponent.hpp
        include/rtype/components/ParallaxComponent.hpp
        include/rtype/systems/ParallaxSystem.hpp
        include/rtype/systems/PhysicsSystem.cpp
        include/rtype/systems/PhysicsSystem.hpp
        include/rtype/components/MyPlayerComponent.hpp
        include/rtype/systems/ControlPlayerSystem.hpp
        include/rtype/systems/AnimPlayerSystem.hpp
        include/rtype/components/BulletComponent.hpp
        include/rtype/systems/BulletSystem.hpp
        include/rtype/components/AnimComponent.hpp
        include/rtype/systems/AnimSystem.hpp
        include/rtype/components/DamageCollisionComponent.hpp
        include/rtype/systems/DamageCollisionSystem.hpp
        include/rtype/components/HPComponent.hpp)
set(SERVER_SRC src/server/main.cpp
        src/server/RTypeServer.cpp
        include/rtype/RTypeServer.hpp
        include/rtype/components/MyPlayerComponent.hpp
        include/rtype/systems/ControlPlayerSystem.hpp
        include/rtype/systems/AnimPlayerSystem.hpp
        include/rtype/systems/NewConnectionSystem.hpp
        include/rtype/components/ClientAdressComponent.hpp
        include/rtype/systems/ServerUdpSystem.hpp)
set(CLIENT_SRC src/client/main.cpp
        src/rtype/RTypeClient.cpp
        include/rtype/RTypeClient.hpp
        src/rtype/systems/RenderSystem.cpp
        include/rtype/systems/RenderSystem.hpp
        include/rtype/systems/ServerConnectionSystem.hpp
        include/rtype/systems/ClientInputSenderSystem.hpp
        include/rtype/systems/ClientServerDataHandlerSystem.cpp
        include/rtype/systems/ClientServerDataHandlerSystem.hpp
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

