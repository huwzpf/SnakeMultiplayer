#!/bin/bash

# ./compile.sh -e (clients_amount) (ip without quoutes)

clear
echo "Starting Server ..."
if [[ "$1" == "-e"  ]]; then
    rm config.h
    touch config.h
    echo "#ifndef CONFIG_H" >> config.h
    echo "#define CONFIG_H" >> config.h
    echo "#define _CRT_SECURE_NO_WARNINGS" >> config.h   
    echo "#define PORT 2137" >> config.h
    echo "#define WIDTH 1280" >> config.h
    echo "#define HEIGHT 720" >> config.h
    echo "#define ANGLE 7" >> config.h
    echo "#define DEFAULT_VELOCITY 2" >> config.h
    echo "#define MAX_GLOBAL_COUNTER 1000" >> config.h
    echo "#define MAX_PLAYERS 7" >> config.h
    echo "#define RGB 3" >> config.h
    echo "#define BUFFER_SIZE 1024" >> config.h
    echo "#define BONUS_CHANCE 0.01" >> config.h
    echo "#define BONUS_TIME 1200" >> config.h
    echo "#define PLAYER_BONUS_TIME 100" >> config.h
    echo "#define DEFAULT_SIZE 1.0" >> config.h
    echo "#define BONUS_SIZE 20" >> config.h
    echo "#define GAP_FREQ 200" >> config.h
    echo "#define BUFFER_SIZE_SMALL 64" >> config.h
    echo "#define CLIENTS_AMOUNT ${2}" >> config.h
    echo "const char* IP = \"${3}\";" >> config.h
    echo "#endif" >> config.h
fi    
    gcc -c queue.c -o queue.o
    gcc -c server.c -lpthread -lm -o server.o
    gcc server.o queue.o -lpthread -lm -o server 
    ./server 

