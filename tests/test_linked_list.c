#include "unity.h"

/* SUT d'esempio */
static int somma(int a, int b) { return a + b; }

void setUp(void)   {}
void tearDown(void){}

void test_Somma_di_positivi(void) {
    TEST_ASSERT_EQUAL_INT(7, somma(3, 4));
}

void test_Somma_con_negativi(void) {
    TEST_ASSERT_EQUAL_INT(1, somma(3, -2));
}