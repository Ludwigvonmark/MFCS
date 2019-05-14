#!/bin/bash 
g++ MFS.cpp -std=c++1z -lpthread -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLU -lglut -lassimp -lGLEW -o MFS -lstdc++fs && ./MFS