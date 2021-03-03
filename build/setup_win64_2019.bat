@echo off

mkdir ga3-win64
cd ga3-win64
cmake -G "Visual Studio 16 2019" ../../src/engine
cd ..
