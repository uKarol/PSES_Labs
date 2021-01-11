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

/**
  @brief Test odczytu i zapisu akumulatora

  Funkcja testująca odczytu i zapisu akumulatora.
*/
void Test_Of_Calc_Get_A(void){
    sint32 testVal;
    Std_ReturnType RetVal;

    // zapisz wlasciwa wartosc
    Calc_Set_A(0);
    RetVal = Calc_Get_A( &testVal);
    TEST_CHECK(RetVal == E_OK);
    TEST_CHECK(testVal == 0);

    // zapisz blad
    Calc_Set_A(0xFFFFFFFF);
    RetVal = Calc_Get_A( &testVal);
    TEST_CHECK(RetVal == E_NOT_OK);
    TEST_CHECK(testVal == 0xFFFFFFFF);

    // sprawdz czy blad nie zostal nadpisany
    Calc_Set_A(100);
    RetVal = Calc_Get_A( &testVal);
    TEST_CHECK(RetVal == E_NOT_OK);
    TEST_CHECK(testVal == 0xFFFFFFFF);

    //  zresetuj pamiec
    Calc_Reset();
    // sprawdz czy zapis poprawnej wartosci po resecie sie powiedzie
    Calc_Set_A(100);
    RetVal = Calc_Get_A( &testVal);
    TEST_CHECK(RetVal == E_OK);
    TEST_CHECK(testVal == 100);

}

/**
  @brief Test zapisu akumulatora

  Funkcja testująca zapis akumulatora.
*/
void Test_Of_Calc_Oper(void){

    Std_ReturnType AddReturnVals[3] = { E_OK, E_NOT_OK, E_OK };
    SET_RETURN_SEQ(Lib_Calc_Add, AddReturnVals, 3);

    Std_ReturnType SubReturnVals[3] = { E_OK, E_NOT_OK, E_OK };
    SET_RETURN_SEQ(Lib_Calc_Sub, SubReturnVals, 3);

    Std_ReturnType MulReturnVals[3] = { E_OK, E_NOT_OK, E_OK };
    SET_RETURN_SEQ(Lib_Calc_Mul, MulReturnVals, 3);

    Std_ReturnType DivReturnVals[3] = { E_OK, E_NOT_OK, E_OK, };
    SET_RETURN_SEQ(Lib_Calc_Div, DivReturnVals, 3);

    // operacje zakonczone powodzeniem
    // powinny zostac wywolane funkcje z lib calc
    // akumulator powienien miec wartosc inna niz 0xFFFFFFFF
    Calc_Set_A(0);
    Calc_Oper( CALC_ADD, 1);
    TEST_CHECK(Lib_Calc_Add_fake.call_count == 1);
    TEST_CHECK(Lib_Calc_Add_fake.arg0_val == 0L);
    TEST_CHECK(Lib_Calc_Add_fake.arg1_val == 1L);
    TEST_CHECK(akumulator != 0xFFFFFFFF);

    Calc_Set_A(0);
    Calc_Oper( CALC_SUB, 1);
    TEST_CHECK(Lib_Calc_Sub_fake.call_count == 1);
    TEST_CHECK(Lib_Calc_Sub_fake.arg0_val == 0L);
    TEST_CHECK(Lib_Calc_Sub_fake.arg1_val == 1L);
    TEST_CHECK(akumulator != 0xFFFFFFFF);

    Calc_Set_A(0);
    Calc_Oper( CALC_MUL, 1);
    TEST_CHECK(Lib_Calc_Mul_fake.call_count == 1);
    TEST_CHECK(Lib_Calc_Mul_fake.arg0_val == 0L);
    TEST_CHECK(Lib_Calc_Mul_fake.arg1_val == 1L);
    TEST_CHECK(akumulator != 0xFFFFFFFF);

    Calc_Set_A(0);
    Calc_Oper( CALC_DIV, 1);
    TEST_CHECK(Lib_Calc_Div_fake.call_count == 1);
    TEST_CHECK(Lib_Calc_Div_fake.arg0_val == 0L);
    TEST_CHECK(Lib_Calc_Div_fake.arg1_val == 1L);
    TEST_CHECK(akumulator != 0xFFFFFFFF);

    // operacje zakonczone bledami
    // wykonanie operacji z libcalc zakonczon e bledem
    // liczba wywaloan mockow powinna sie zwiekszyc
    // akumulator powienien miec wartosc 0xFFFFFFFF (blad)

    Calc_Oper( CALC_ADD, 0);
    TEST_CHECK(Lib_Calc_Add_fake.call_count == 2);
    TEST_CHECK(akumulator == 0xFFFFFFFF);
    Calc_Reset(); //reset akumulatora

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
    TEST_CHECK(akumulator == 0xFFFFFFFF); // tutaj reset akumulatora zostal celowo pominiety, aby mozna bylo przeprowadzic kolejne testy
    
    // proba wykonania poprawnej operacji, jezeli akumulator nie zostal zresetowany po wystapieniu bledu
    // liczba wywolan mockow powinna pozostac bez zmian
    // akumulator powienien miec wartosc 0xFFFFFFFF (blad)
    Calc_Oper( CALC_ADD, 1);
    TEST_CHECK(Lib_Calc_Add_fake.call_count == 2);
    TEST_CHECK(akumulator == 0xFFFFFFFF);

    Calc_Oper( CALC_SUB, 1);
    TEST_CHECK(Lib_Calc_Sub_fake.call_count == 2);
    TEST_CHECK(akumulator == 0xFFFFFFFF);

    Calc_Oper( CALC_MUL, 1);
    TEST_CHECK(Lib_Calc_Mul_fake.call_count == 2);
    TEST_CHECK(akumulator == 0xFFFFFFFF);

    Calc_Oper( CALC_DIV, 1);
    TEST_CHECK(Lib_Calc_Div_fake.call_count == 2);
    TEST_CHECK(akumulator == 0xFFFFFFFF);
    
    Calc_Reset();

    // proba wykonania operacji o blednym kodzie
    Calc_Oper( 666, 0);
    TEST_CHECK(akumulator == 0xFFFFFFFF);
    

}

void Test_Of_Calc_Mem(void){

    RESET_FAKE(Lib_Calc_Add);
    RESET_FAKE(Lib_Calc_Sub);
    RESET_FAKE(Lib_Calc_MemS);

    Std_ReturnType AddReturnVals[3] = { E_OK, E_NOT_OK, E_OK };
    SET_RETURN_SEQ(Lib_Calc_Add, AddReturnVals, 3);

    Std_ReturnType SubReturnVals[3] = { E_OK, E_NOT_OK, E_OK };
    SET_RETURN_SEQ(Lib_Calc_Sub, SubReturnVals, 3);
    
    sint32 MemReturnVals[6] = { 0, 0, 0, 0, 0, 0};
    SET_RETURN_SEQ(Lib_Calc_MemR, MemReturnVals, 6);

    // normalne operacje na pamieci
    // za kazdym razem powinny byc wolane funkcje Lb_Calc_MemS i Lib_Calc_MemR
    // w przypadku CALC_MEM_ADD i CALC_MEM_SUB powinny byc jeszcze wywolane funkcje Lib_Calc_Add i Lib_Calc_Sub

    Calc_Set_A(0);
    Calc_Mem(CALC_MEM_ADD);
    TEST_CHECK(Lib_Calc_Add_fake.arg0_val == 0L);
    TEST_CHECK(Lib_Calc_Add_fake.arg1_val == 0L);
    TEST_CHECK(Lib_Calc_Add_fake.call_count == 1);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 1);
    TEST_CHECK(Lib_Calc_MemR_fake.call_count == 1);

    Calc_Set_A(0);
    Calc_Mem(CALC_MEM_SUB);
    TEST_CHECK(Lib_Calc_Sub_fake.arg0_val == 0L);
    TEST_CHECK(Lib_Calc_Sub_fake.arg1_val == 0L);
    TEST_CHECK(Lib_Calc_Sub_fake.call_count == 1);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 2);
    TEST_CHECK(Lib_Calc_MemR_fake.call_count == 2);
    
    Calc_Mem(CALC_MEM_STO);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 3);

    Calc_Mem(CALC_MEM_ZERO);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 4);

    // operacje funkcje Lib_Calc_Add i Lib_Calc_Sub zwrocily blad
    // liczba wywolac Lib_Calc_MemR powinna wzrosnac
    // liczba wywolac Lib_Calc_MemS powinna sie nie zmienic
    Calc_Set_A(0);
    Calc_Mem(CALC_MEM_ADD);
    TEST_CHECK(Lib_Calc_Add_fake.arg0_val == 0L);
    TEST_CHECK(Lib_Calc_Add_fake.arg1_val == 0L);
    TEST_CHECK(Lib_Calc_Add_fake.call_count == 2);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 4);
    TEST_CHECK(Lib_Calc_MemR_fake.call_count == 3);

    Calc_Set_A(0);
    Calc_Mem(CALC_MEM_SUB);
    TEST_CHECK(Lib_Calc_Sub_fake.arg0_val == 0L);
    TEST_CHECK(Lib_Calc_Sub_fake.arg1_val == 0L);
    TEST_CHECK(Lib_Calc_Sub_fake.call_count == 2);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 4);
    TEST_CHECK(Lib_Calc_MemR_fake.call_count == 4);
    

    Calc_Set_A(0xFFFFFFFF);
    TEST_CHECK(akumulator == 0xFFFFFFFF);

    // w akumulatorze jest blad i nie zostal on zresetowany
    // liczba wywolan funkcji z lib_calc powinna sie nie zmienic
    Calc_Mem(CALC_MEM_ADD);
    TEST_CHECK(Lib_Calc_Add_fake.call_count == 2);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 4);
    TEST_CHECK(Lib_Calc_MemR_fake.call_count == 4);

    Calc_Mem(CALC_MEM_SUB);
    TEST_CHECK(Lib_Calc_Sub_fake.call_count == 2);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 4);
    TEST_CHECK(Lib_Calc_MemR_fake.call_count == 4);

    Calc_Mem(CALC_MEM_STO);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 4);

    Calc_Mem(CALC_MEM_ZERO);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 4);
    Calc_Reset();


    // wywloanie funkcji o blednym kodzie
    // liczba wywloan powinna zostac bez zmian z wyjatkiem Lib_Calc_MemS, bo wczesniej wywolalismy reset
    Calc_Mem(10);
    TEST_CHECK(Lib_Calc_Add_fake.call_count == 2);
    TEST_CHECK(Lib_Calc_Sub_fake.call_count == 2);
    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 5);
    TEST_CHECK(Lib_Calc_MemR_fake.call_count == 4);


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
    
