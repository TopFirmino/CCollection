#include "unity.h"

/* Dichiarazioni dei test in altri file */
void test_Somma_di_positivi(void);
void test_Somma_con_negativi(void);

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Somma_di_positivi);
    RUN_TEST(test_Somma_con_negativi);
    return UNITY_END();
}
