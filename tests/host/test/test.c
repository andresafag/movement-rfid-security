#include <testing.h>
#include <stdbool.h>
#include <stdlib.h>

// 1. The function we want to test
bool is_sum_ten(int a, int b) {
    if (a + b == 10) {
        return true;
    }
    return false;
}

// 2. Write the test cases
START_TEST(test_equals_ten) {
    // Assert that 7 + 3 returns true (1)
    ck_assert_int_eq(is_sum_ten(7, 3), true); 
}
END_TEST

START_TEST(test_not_equals_ten) {
    // Assert that 5 + 2 returns false (0)
    ck_assert_int_eq(is_sum_ten(5, 2), false);
}
END_TEST

// 3. Group the tests into a suite
Suite *math_suite(void) {
    Suite *s = suite_create("SimpleTenSuite");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_equals_ten);
    tcase_add_test(tc_core, test_not_equals_ten);
    suite_add_tcase(s, tc_core);

    return s;
}

// 4. The main function that runs everything
int main(void) {
    int number_failed;
    Suite *s = math_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


