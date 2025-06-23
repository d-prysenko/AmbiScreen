#!/bin/bash

name=$(ls /dev/serial/by-id -la | grep sb-Raspberry_Pi_Pico | grep -o 'tty.*')
sudo ~/projects/AmbiScreen/build/src/AmbiScreen /dev/$name
