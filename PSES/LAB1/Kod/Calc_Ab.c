/** ==================================================================================================================*\
  @file Calc_Ab.c

  @brief Implementacja kalkulatora
  
  Implementacja kalkukatora 4-działaniowego z pamięcią.

  @see Calc_Ab.pdf
\*====================================================================================================================*/

/*====================================================================================================================*\
    Załączenie nagłówków
\*====================================================================================================================*/
#include "Lib_Calc.h"
#include "Calc_Ab.h"

/*====================================================================================================================*\
    Makra lokalne
\*====================================================================================================================*/

/*====================================================================================================================*\
    Typy lokalne
\*====================================================================================================================*/

/*====================================================================================================================*\
    Zmienne globalne
\*====================================================================================================================*/

/*====================================================================================================================*\
    Zmienne lokalne (statyczne)
\*====================================================================================================================*/
static sint32 akumulator;     /**< Akumulator kalkulatora [P2] */

/*====================================================================================================================*\
    Deklaracje funkcji lokalnych
\*====================================================================================================================*/

/*====================================================================================================================*\
    Kod globalnych funkcji inline i makr funkcyjnych
\*====================================================================================================================*/

/*====================================================================================================================*\
    Kod funkcji
\*====================================================================================================================*/

/**
  @brief Zerowanie modułu

  Funkcja zeruje wartość akumulatora i pamięci.
  Realizuje [P9]
*/
void Calc_Reset(void)
{
    akumulator = 0L;
    Lib_Calc_MemS(0L);
}


// W2
void Calc_Set_A(sint32 a)
{
    if( akumulator != 0xFFFFFFFF ) akumulator = a;
}

//W2 W6
Std_ReturnType Calc_Get_A( sint32 *a)
{
    sint32 retVal;
    *a = akumulator;
    if( akumulator == 0xFFFFFFFF) retVal = E_NOT_OK;
    else retVal = E_OK;

    return retVal;
}

//W3
void Calc_Oper(Calc_Operation_T op, sint32 arg2)
{
    sint32 temp_result;
    
    if( akumulator != 0xFFFFFFFF )
    {
        Std_ReturnType RetVal = E_OK;
        switch (op)
        {
            case CALC_ADD:
                RetVal = Lib_Calc_Add(akumulator, arg2, &temp_result);
                break;
            case CALC_SUB:
                RetVal = Lib_Calc_Sub(akumulator, arg2, &temp_result);
                break;
            case CALC_MUL:
                RetVal = Lib_Calc_Mul(akumulator, arg2, &temp_result);
                break;
            case CALC_DIV:
                RetVal = Lib_Calc_Div(akumulator, arg2, &temp_result);
            break;   
            default:
                RetVal = E_NOT_OK;
            break;
        }

        if( RetVal == E_NOT_OK)   akumulator = 0xFFFFFFFF;
        else                      akumulator = temp_result;
    }
}
//W5
void Calc_Mem(Calc_MemOp_T op){
    
    sint32 result;
   
    if(akumulator != 0xFFFFFFFF){
        Std_ReturnType RetVal;
        sint32 arg;
        switch (op)
        {
            case CALC_MEM_ADD:
                arg = Lib_Calc_MemR();
                RetVal = Lib_Calc_Add(arg, akumulator, &result);
                if(RetVal == E_OK) Lib_Calc_MemS(result);
            break;
    
            case CALC_MEM_STO:
                Lib_Calc_MemS(akumulator);
            break;
    
            case CALC_MEM_SUB:
                arg = Lib_Calc_MemR();
                RetVal = Lib_Calc_Sub(arg, akumulator, &result);
                if(RetVal == E_OK) Lib_Calc_MemS(result);
            break;
    
            case CALC_MEM_ZERO:
                Lib_Calc_MemS(0);              
            break;
    
        }
    }
}