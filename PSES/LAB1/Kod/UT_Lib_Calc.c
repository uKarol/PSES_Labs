/** ==================================================================================================================*\
  @file UT_Lib_Calc.c

  @brief Testy jednostkowe do Lib_Calc
\*====================================================================================================================*/

#include "acutest.h"
#include "Std_Types.h"

#include "Lib_Calc.c"   /* To nie pomyłka - taki include eksponuje zmienne statyczne dla testów */


/**
  @brief Test dodawania

  Funkcja testująca dodawanie w bibliotece. Funkcje testowe acutest nie mogą zwracać ani przyjmować danych!
*/
void Test_Of_Lib_Calc_Add(void)
{
    sint32 result;
    Std_ReturnType retv;

    retv = Lib_Calc_Add(0L, 0L, &result);

    TEST_CHECK(result == 0);       /* Test przejdzie jeżeli wyrażenie w nawiasach jest prawdziwe */
    TEST_CHECK(retv == E_OK);

    retv = Lib_Calc_Add(1L, -1L, &result);

    TEST_CHECK(result == 0);
    TEST_CHECK(retv == E_OK);

    /* TUTAJ DOPISZ WIĘCEJ TESTÓW */

    //random positive numbers
    retv = Lib_Calc_Add(200L, 100L, &result);

    TEST_CHECK(result == 300L);
    TEST_CHECK(retv == E_OK);

    //random megative numbers
    retv = Lib_Calc_Add(-201L, -666L, &result);

    TEST_CHECK(result == -867L);
    TEST_CHECK(retv == E_OK);

    // near overflow
    retv = Lib_Calc_Add(2147483646L, 1L, &result);

    TEST_CHECK(result == 2147483647L);
    TEST_CHECK(retv == E_OK);

    // negative tests 

    // max positive + 1, expected overflow 
    retv = Lib_Calc_Add(2147483647L, 1L, &result);

    TEST_CHECK(result == 2147483648UL);
    TEST_CHECK(retv == E_NOT_OK);

    // max positive + max positive, expected overflow 
    retv = Lib_Calc_Add(2147483647L, 2147483647L, &result);

    TEST_CHECK(result == 0xFFFFFFFE);
    TEST_CHECK(retv == E_NOT_OK);

    // max negative - 1, expected overflow 

    retv = Lib_Calc_Add(-2147483648L, -1L, &result);

    TEST_CHECK(result == 0x7FFFFFFF);
    TEST_CHECK(retv == E_NOT_OK);

    // max negative + max negative, expected overflow

    retv = Lib_Calc_Add(-2147483648L, -2147483648L, &result);

    TEST_CHECK(result == 0);
    TEST_CHECK(retv == E_NOT_OK);

}


/**
  @brief Test odejmowania

  Funkcja testująca odejmowanie w bibliotece.
*/
void Test_Of_Lib_Calc_Sub(void)
{
    sint32 result;
    Std_ReturnType retv;
    //zero value
    retv = Lib_Calc_Sub(0L, 0L, &result);

    TEST_CHECK(result == 0);
    TEST_CHECK(retv == E_OK);
    //posotive and negative
    retv = Lib_Calc_Sub(1L, -1L, &result);

    TEST_CHECK(result == 2L);
    TEST_CHECK(retv == E_OK);

    /* TUTAJ DOPISZ WIĘCEJ TESTÓW */

    // negative tests 

    // max positive + 1, expected overflow 
    retv = Lib_Calc_Sub(2147483647L, -1L, &result);

    TEST_CHECK(result == 0x80000000);
    TEST_CHECK(retv == E_NOT_OK);

    // max positive + max positive, expected overflow 
    retv = Lib_Calc_Sub(2147483647L, -2147483647L, &result);

    TEST_CHECK(result == 0xFFFFFFFEL);
    TEST_CHECK(retv == E_NOT_OK);

    // max negative - 1, expected overflow 

    retv = Lib_Calc_Sub(-2147483648L, 1L, &result);

    TEST_CHECK(result == 0x7FFFFFFF);
    TEST_CHECK(retv == E_NOT_OK);

    // max negative + max negative, expected overflow

    retv = Lib_Calc_Sub(-2147483648L, -2147483648L, &result);

    TEST_CHECK(result == 0);
    TEST_CHECK(retv == E_OK);
}

/**
  @brief Test mnozenia

  Funkcja testująca mnozenie w bibliotece. 
*/
void Test_Of_Lib_Calc_Mul(void)
{
    sint32 result;
    Std_ReturnType retv;

    retv = Lib_Calc_Mul(0L, 0L, &result);

    TEST_CHECK(result == 0);
    TEST_CHECK(retv == E_OK);

    retv = Lib_Calc_Mul(1L, -1L, &result);

    TEST_CHECK(result == -1L);
    TEST_CHECK(retv == E_OK);

    /* TUTAJ DOPISZ WIĘCEJ TESTÓW */

    retv = Lib_Calc_Mul(10L, 2L, &result);

    TEST_CHECK(result == 20L);
    TEST_CHECK(retv == E_OK);


    // NEGATIVE 

    retv = Lib_Calc_Mul(-2147483648L, 2L, &result);

    TEST_CHECK(result == 0L);
    TEST_CHECK(retv == E_NOT_OK);

    retv = Lib_Calc_Mul(0x7FFFFFFFL, 2L, &result);

    TEST_CHECK(result == 0xFFFFFFFEL);
    TEST_CHECK(retv == E_NOT_OK);


}
/**
  @brief Test dzielenia

  Funkcja testująca dzielenie w bibliotece. Funkcje testowe acutest nie mogą zwracać ani przyjmować danych!
*/
void Test_Of_Lib_Calc_Div(void)
{
    sint32 result;
    Std_ReturnType retv;

    // posotive tests



    retv = Lib_Calc_Div(-2147483648L, 2L, &result);

    TEST_CHECK(result == 0xC0000000L);
    TEST_CHECK(retv == E_OK);

    retv = Lib_Calc_Div(-2147483648L, -2147483648L, &result);

    TEST_CHECK(result == 0x1L);
    TEST_CHECK(retv == E_OK);

    retv = Lib_Calc_Div(2147483647L, 2L, &result);

    TEST_CHECK(result == 0x3FFFFFFFL);
    TEST_CHECK(retv == E_OK);

    retv = Lib_Calc_Div(2147483647L, 2147483647L, &result);

    TEST_CHECK(result == 1L);
    TEST_CHECK(retv == E_OK);

    retv = Lib_Calc_Div(2147483647L, 0L, &result);

    // negative tests

    TEST_CHECK(result == 0x7FFFFFFFL);
    TEST_CHECK(retv == E_NOT_OK);

    retv = Lib_Calc_Div(0L, 0L, &result);

    TEST_CHECK(result == 0L);
    TEST_CHECK(retv == E_NOT_OK);

    retv = Lib_Calc_Div(-2147483648L, 0L, &result);

    TEST_CHECK(result == 0x80000000L);
    TEST_CHECK(retv == E_NOT_OK);

    
}
/**
  @brief Test zapisu i odczytu pamieci

  Funkcja testująca zapis i odczyt pamieci kalkulatora 
*/
void Test_Of_Lib_Calc_MemS(void)
{
    sint32 result;

    Lib_Calc_MemS(0);
    result = Lib_Calc_MemR();
    TEST_CHECK(result == 0L);

    Lib_Calc_MemS(5);
    result = Lib_Calc_MemR();
    TEST_CHECK(result == 5L);


    Lib_Calc_MemS(-2147483648L);
    result = Lib_Calc_MemR();
    TEST_CHECK(result == -2147483648L);

    Lib_Calc_MemS(2147483647L);
    result = Lib_Calc_MemR();
    TEST_CHECK(result == 2147483647L);

}

/*
  Lista testów - wpisz tutaj wszystkie funkcje które mają być wykonane jako testy.
*/
TEST_LIST = {
    { "Test of Lib_Calc_Add", Test_Of_Lib_Calc_Add },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of Lib_Calc_Sub", Test_Of_Lib_Calc_Sub },
    { "Test of Lib_Calc_Mul", Test_Of_Lib_Calc_Mul },
    { "Test of Lib_Calc_Div", Test_Of_Lib_Calc_Div },
    { "Test of LibCalc_MemS", Test_Of_Lib_Calc_MemS},
   // { "Test of Lib_Calc_Sub", Test_Of_Lib_Calc_Sub },
    { NULL, NULL }                                      /* To musi być na końcu */
};

