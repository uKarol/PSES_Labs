gcc UT_Calc_Ab.c -o bins/UT_Calc_Ab.exe
cd bins
UT_Calc_Ab.exe
cd ..
gcc -fprofile-arcs -ftest-coverage -g UT_Calc_Ab.c -o bins/UT_Calc_Ab.exe
cd bins 
UT_Calc_Ab.exe
cd ..
gcov UT_Calc_Ab.c
pause