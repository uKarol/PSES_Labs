gcc UT_Lib_Calc.c -o bins/UT_Lib_Calc.exe
cd bins
UT_Lib_Calc.exe
cd ..
gcc -fprofile-arcs -ftest-coverage -g UT_Lib_Calc.c -o bins/UT_Lib_Calc.exe
cd bins 
UT_Lib_Calc.exe
cd ..
gcov UT_Lib_Calc.c
pause