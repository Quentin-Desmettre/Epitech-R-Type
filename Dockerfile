FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y \
    libxrandr-dev  \
    libxcursor-dev  \
    libudev-dev  \
    libopenal-dev  \
    libflac-dev  \
    libvorbis-dev  \
    libgl1-mesa-dev  \
    libegl1-mesa-dev  \
    libdrm-dev  \
    libgbm-dev \
    build-essential \
    libfreetype-dev \
    pkg-config \
    cmake \
    git

# Install Environment
WORKDIR /app
COPY CMakeLists.txt .
COPY src src
COPY assets assets
COPY shaders shaders
COPY Compilation Compilation
COPY include include

# Build
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build -j
RUN cp build/r-type_server .

# Run
CMD ["./r-type_server"]
