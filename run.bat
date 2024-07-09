make

@echo off

cd output

main.exe ..\obj\african_head.obj

gprof main.exe gmon.out > analysis.txt

output.tga

cd ..
