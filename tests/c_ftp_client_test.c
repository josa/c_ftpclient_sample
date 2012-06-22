/*
 * File:   c_ftp_client_test.c
 * Author: jgalvao
 *
 * Created on Jun 21, 2012, 9:14:00 PM
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
    CU_ASSERT(2 * 2 == 4);
}

void test_parseFileName() {
    char *filePath = "/bla/foo/test.tar.gz";
    char *expected = "test.tar.gz";
    char *filename = malloc(1);
    parseFileName(filePath, filename);
    CU_ASSERT_STRING_EQUAL(filename, expected);
}

int main() {
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("c_ftp_client_test", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "test1", test1)) ||
            (NULL == CU_add_test(pSuite, "test2", test2)) ||
            (NULL == CU_add_test(pSuite, "test_parseFileName", test_parseFileName))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
