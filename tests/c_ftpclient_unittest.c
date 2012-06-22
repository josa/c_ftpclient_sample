#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h" 
 
void testGetNameFile(){
   char *filePath = "/bla/foo/test.tar.gz";
   char *expected = "test.tar.gz";
   char *filename = getNameFile(filePath);
   //CU_ASSERT_STRING_EQUAL(expected, filename)
}

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

int main() {
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("c_ftpclient_unittest", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_pTest t1 = CU_add_test(pSuite, "testGetNameFile", testGetNameFile);
    
    if (t1 == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}