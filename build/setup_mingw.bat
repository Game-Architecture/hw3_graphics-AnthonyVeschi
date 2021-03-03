@echo off

mkdir ga3-win64
cd ga3-win64
cmake -G "MinGW Makefiles" -DVIDEO_OPENGLES=0 ../../src/engine
cd ..
