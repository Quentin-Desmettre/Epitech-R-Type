# Epitech - R-Type

R-Type is a 2D horizontal scrolling shoot 'em up arcade game developed by Irem and published in 1987.

The main goal of this project is to create a clone of this game using modern C++ and the SFML library.

But, in addition,
to build this game, we must create a game engine, and the game must provide a multiplayer mode
(using an authoritative server and a client), through the use of the UDP protocol.

# Summary

- [How to run the game](#how-to-run-the-game)
    - [Build from source](#build-from-source)
        - [Requirements](#requirements)
        - [Build](#build)
    - [Download the latest release](#download-the-latest-release)
    - [Run the game](#run-the-game)
- [How to play](#how-to-play)
    - [Controls](#controls)
    - [Gameplay](#gameplay)
- [Contribution guidelines](#contribution-guidelines)
- [Developer documentation](#developer-documentation)
- [Authors](#authors)

# How to run the game

To run the game, can either

- [Build it from source](#build-from-source) (longer, for developers or curious people)
- [Download the latest release](#download-the-latest-release) (faster, for players)

## Build from source

### Requirements

- [CMake](https://cmake.org/) (>= 3.22)

If running on a Linux system, you will also need these packages to use SFML:

- freetype
- x11
- xrandr
- udev
- opengl
- flac
- ogg
- vorbis
- vorbisenc
- vorbisfile
- openal
- pthread

> The exact name of the packages may vary from distribution to distribution. Once those packages are installed, don't forget to install their development headers as well.

### Build

Once you have cloned the repository and opened a terminal in the project's root directory, you can run the following command:

```bash
./run.sh
```

This script will do everything for you, pulling the SFML from source and building it, and then building the game.
You will be left with 2 executables at the root of the repository: `r-type_client` and `r-type_server`,
which are the client and server executables respectively.

> Note that the first build may take a while, as it will build *everything* from source.
Subsequent builds will be much faster, as only the modified files will be recompiled.

## Download the latest release

You can download the latest release [here]().

Please select the appropriate archive for your system (Windows, Linux or macOS).


## Run the game

To run the game, you will need to run the server first, and then the client.

Run the server with the following command:

```bash
./r-type_server [port]
```

With `[port]` being the port you want the server to listen on.

Then, run the client with the following command:

```bash
./r-type_client [server ip] [server port]
```

With `[server ip]` being the IP address of the server, and `[server port]` being the port the server is listening on.

# How to play

## Controls

- **Move left**: `Q`
- **Move right**: `D`
- **Move up**: `Z`
- **Move down**: `S`
- **Shoot**: `Space`
- **Charge**: `Left Shift`
- **Quit**: `Escape`

## Gameplay

The goal of the game is to kill as many enemies as possible, without dying. Each enemy killed will give you 1 point.

Once you have died, you can rejoin the game by quitting and relaunching the client, restarting the game with 0 points.


# Contribution guidelines

If you want to contribute to this project, please read the [CONTRIBUTING.md](CONTRIBUTING.md) file.

# Developer documentation

You can find an extended documentation of the project [here](index.md).

You will find everything technical you need to know in order to start coding on this project.

# Authors

- [**Quentin Desmettre**](mailto:quentin.desmettre@epitech.eu)
- [**Paulin Leclercq**](mailto:paulin.leclercq@epitech.eu)
- [**Edgar Lecuyer**](mailto:edgar.lecuyer@epitech.eu)
- [**Noa Trachez**](mailto:noa.trachez@epitech.eu)
- [**Clement Vandeville**](mailto:clement.vandeville@epitech.eu)
