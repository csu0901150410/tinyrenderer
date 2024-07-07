make

@echo off

cd output

main.exe

gprof main.exe gmon.out > analysis.txt

output.tga

cd ..
