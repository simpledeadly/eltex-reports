#include "calc.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

int setup_suite(void) { return 0; }
int teardown_suite(void) { return 0; }

/*
 * Тест: Сложение
 * Входные данные: 5.0, 3.0
 * Ожидаемый результат: 8.0
 */
void test_add(void) { CU_ASSERT_DOUBLE_EQUAL(calc_add(5.0, 3.0), 8.0, 0.001); }

/*
 * Тест: Вычитание
 * Входные данные: 5.0, 3.0
 * Ожидаемый результат: 2.0
 */
void test_sub(void) { CU_ASSERT_DOUBLE_EQUAL(calc_sub(5.0, 3.0), 2.0, 0.001); }

/*
 * Тест: Умножение
 * Входные данные: 5.0, 3.0
 * Ожидаемый результат: 15.0
 */
void test_mul(void) { CU_ASSERT_DOUBLE_EQUAL(calc_mul(5.0, 3.0), 15.0, 0.001); }

/*
 * Тест: Нормальное деление
 * Входные данные: 15.0, 3.0
 * Ожидаемый результат: 5.0, error = 0
 */
void test_div_normal(void) {
  int err;
  CU_ASSERT_DOUBLE_EQUAL(calc_div(15.0, 3.0, &err), 5.0, 0.001);
  CU_ASSERT_EQUAL(err, 0);
}

/*
 * Тест: Деление на ноль
 * Входные данные: 15.0, 0.0
 * Ожидаемый результат: error = 1
 */
void test_div_zero(void) {
  int err;
  calc_div(15.0, 0.0, &err);
  CU_ASSERT_EQUAL(err, 1);
}

int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite suite =
      CU_add_suite("Calc_Basic_Suite", setup_suite, teardown_suite);
  if (NULL == suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(suite, "test_add", test_add) ||
      NULL == CU_add_test(suite, "test_sub", test_sub) ||
      NULL == CU_add_test(suite, "test_mul", test_mul) ||
      NULL == CU_add_test(suite, "test_div_normal", test_div_normal) ||
      NULL == CU_add_test(suite, "test_div_zero", test_div_zero)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  int failures = CU_get_number_of_failures();
  CU_cleanup_registry();
  return failures;
}
