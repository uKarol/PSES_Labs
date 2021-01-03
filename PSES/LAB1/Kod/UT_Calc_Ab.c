/** ==================================================================================================================*\
  @file UT_Calc_Ab.c

  @brief Testy jednostkowe do Calc_Ab
\*====================================================================================================================*/
#include "Std_Types.h"

#include "acutest.h"
#include "fff.h"

#include "Calc_Ab.c"

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(Lib_Calc_MemS, sint32);
FAKE_VALUE_FUNC(sint32, Lib_Calc_MemR);
FAKE_VALUE_FUNC(Std_ReturnType, Lib_Calc_Add, sint32, sint32 , sint32* );
FAKE_VALUE_FUNC(Std_ReturnType, Lib_Calc_Sub, sint32, sint32 , sint32* );
FAKE_VALUE_FUNC(Std_ReturnType, Lib_Calc_Mul, sint32, sint32 , sint32* );
FAKE_VALUE_FUNC(Std_ReturnType, Lib_Calc_Div, sint32, sint32 , sint32* );

/**
  @brief Test zerowania modułu

  Funkcja testująca reset modułu.
*/
void Test_Of_Calc_Reset(void)
{
    akumulator = 0xFFFFFFFFL;

    Calc_Reset();

    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 1);
    TEST_CHECK(Lib_Calc_MemS_fake.arg0_val == 0L);
    TEST_CHECK(akumulator == 0L);
}


void Test_Of_Calc_Get_A(void){
    sint32 testVal;
    Std_ReturnType RetVal;

    // some positive value
    Calc_Set_A(0);
    RetVal = Calc_Get_A( &testVal);
    TEST_CHECK(RetVal == E_OK);
    TEST_CHECK(testVal == 0);

    // some negative value 
    Calc_Set_A(0xFFFFFFFF);
    RetVal = Calc_Get_A( &testVal);
    TEST_CHECK(RetVal == E_NOT_OK);
    TEST_CHECK(testVal == 0xFFFFFFFF);

    // some positive value after error
    Calc_Set_A(100);
    RetVal = Calc_Get_A( &testVal);
    TEST_CHECK(RetVal == E_NOT_OK);
    TEST_CHECK(testVal == 0xFFFFFFFF);

    //  bevior after reset
    Calc_Reset();
    Calc_Set_A(100);
    RetVal = Calc_Get_A( &testVal);
    TEST_CHECK(RetVal == E_OK);
    TEST_CHECK(testVal == 100);

}

void Test_Of_Calc_Oper(void){

    Std_ReturnType AddReturnVals[2] = { E_OK, E_NOT_OK };
    SET_RETURN_SEQ(Lib_Calc_Add, AddReturnVals, 2);

    Std_ReturnType SubReturnVals[2] = { E_OK, E_NOT_OK };
    SET_RETURN_SEQ(Lib_Calc_Sub, SubReturnVals, 2);

    Std_ReturnType MulReturnVals[2] = { E_OK, E_NOT_OK };
    SET_RETURN_SEQ(Lib_Calc_Mul, MulReturnVals, 2);

    Std_ReturnType DivReturnVals[2] = { E_OK, E_NOT_OK };
    SET_RETURN_SEQ(Lib_Calc_Div, DivReturnVals, 2);

    // valid operations
    Calc_Oper( CALC_ADD, 1);
    TEST_CHECK(Lib_Calc_Add_fake.call_count == 1);
    TEST_CHECK(akumulator != 0xFFFFFFFF);

    Calc_Oper( CALC_SUB, 1);
    TEST_CHECK(Lib_Calc_Sub_fake.call_count == 1);
    TEST_CHECK(akumulator != 0xFFFFFFFF);

    Calc_Oper( CALC_MUL, 1);
    TEST_CHECK(Lib_Calc_Mul_fake.call_count == 1);
    TEST_CHECK(akumulator != 0xFFFFFFFF);

    Calc_Oper( CALC_DIV, 1);
    TEST_CHECK(Lib_Calc_Div_fake.call_count == 1);
    TEST_CHECK(akumulator != 0xFFFFFFFF);

    // operation finished with errors
    Calc_Oper( CALC_ADD, 0);
    TEST_CHECK(Lib_Calc_Add_fake.call_count == 2);
    TEST_CHECK(akumulator == 0xFFFFFFFF);
    Calc_Reset();

    Calc_Oper( CALC_SUB, 0);
    TEST_CHECK(Lib_Calc_Sub_fake.call_count == 2);
    TEST_CHECK(akumulator == 0xFFFFFFFF);
    Calc_Reset();

    Calc_Oper( CALC_MUL, 0);
    TEST_CHECK(Lib_Calc_Mul_fake.call_count == 2);
    TEST_CHECK(akumulator == 0xFFFFFFFF);
    Calc_Reset();

    Calc_Oper( CALC_DIV, 0);
    TEST_CHECK(Lib_Calc_Div_fake.call_count == 2);
    TEST_CHECK(akumulator == 0xFFFFFFFF);
    Calc_Reset();

    // wrong operation
    Calc_Oper( 666, 0);
    TEST_CHECK(akumulator == 0xFFFFFFFF);
    

}

void Test_Of_Calc_Mem(void){

    RESET_FAKE(Lib_Calc_Add);
    RESET_FAKE(Lib_Calc_Sub);
    RESET_FAKE(Lib_Calc_MemS);

    Std_ReturnType AddReturnVals[2] = { E_OK, E_OK };
    SET_RETURN_SEQ(Lib_Calc_Add, AddReturnVals, 2);

    Std_ReturnType SubReturnVals[2] = { E_OK, E_OK };
    SET_RETURN_SEQ(Lib_Calc_Sub, SubReturnVals, 2);
    
    sint32 MemReturnVals[4] = { 0, 0, 0, 0};
    SET_RETURN_SEQ(Lib_Calc_MemR, MemReturnVals, 4);

    Calc_Set_A(0xFFFFFFFF);
    TEST_CHECK(akumulator == 0xFFFFFFFF);

    Calc_Mem(CALC_MEM_SUB);
    TEST_CHECK(Lib_Calc_Sub_fake.call_count == 0);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 0);
    TEST_CHECK(Lib_Calc_MemR_fake.call_count == 0);

    Calc_Mem(CALC_MEM_STO);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 0);

    Calc_Mem(CALC_MEM_ZERO);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 0);
    Calc_Reset();

    Calc_Mem(CALC_MEM_ADD);
    TEST_CHECK(Lib_Calc_Add_fake.call_count == 1);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 2);
    TEST_CHECK(Lib_Calc_MemR_fake.call_count == 1);

    Calc_Mem(CALC_MEM_SUB);
    TEST_CHECK(Lib_Calc_Sub_fake.call_count == 1);
     TEST_CHECK(Lib_Calc_MemS_fake.call_count == 3);
     TEST_CHECK(Lib_Calc_MemR_fake.call_count == 2);
    
    Calc_Mem(CALC_MEM_STO);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 4);

    Calc_Mem(CALC_MEM_ZERO);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 5);

}



/*
  Lista testów - wpisz tutaj wszystkie funkcje które mają być wykonane jako testy.
*/
TEST_LIST = {
    { "Test of Calc_Reset", Test_Of_Calc_Reset },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test_Of_Calc_Get_A", Test_Of_Calc_Get_A},
    { "Test_Of_Calc_Oper", Test_Of_Calc_Oper},
    { "Test_Of_Calc_Mem", Test_Of_Calc_Mem },
    { NULL, NULL }                                        /* To musi być na końcu */
};
    
