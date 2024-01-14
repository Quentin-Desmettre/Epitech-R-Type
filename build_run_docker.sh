#! /bin/bash

isBuild=$(docker image ls | grep r-type_server)

if [[ -z "$isBuild" ]];
then
    echo "Docker image not found, building it..."
    docker build -t r-type_server .
    echo "Docker image built, running it..."
    docker run --rm -p 4242:4242 -p 4243:4243 r-type_server
    exit 0
else
    echo "Docker image found, running it..."
    docker run --rm -p 4242:4242 -p 4243:4243 r-type_server
    exit 0
fi
