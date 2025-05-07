#!/bin/bash

g++ main.cpp -Ofast -std=c++0x -pthread
./a.out > img2.ppm