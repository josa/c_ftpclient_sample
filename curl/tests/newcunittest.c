/*
 * File:   newcunittest.c
 * Author: jgalvao
 *
 * Created on Jun 19, 2012, 11:42:35 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h"

/*
 * CUnit Test Suite
 */

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

void test1() {
    CU_ASSERT(2 * 2 == 4);
}

void test2() {
    CU_ASSERT(2 * 2 == 5);
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    CU_pSuite pSuite = CU_add_suite("localfile_test", init_suite, clean_suite);;

    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "test1", test1)) ||
            (NULL == CU_add_test(pSuite, "test2", test2))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
