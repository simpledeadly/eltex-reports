#include "calc.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <math.h>

int setup_suite(void) { return 0; }
int teardown_suite(void) { return 0; }

/*
 * Тест: Сложение через указатель на функцию
 *
 * Проверяет, что operations[0] ссылается на функцию add и возвращает
 * корректную сумму. Вызов происходит через указатель, не напрямую.
 *
 * Входные данные: 5.0, 3.0
 * Ожидаемый результат: 8.0 (допуск 0.001)
 */
void test_add(void) {
  CU_ASSERT_DOUBLE_EQUAL(operations[0](5.0, 3.0), 8.0, 0.001);
}

/*
 * Тест: Вычитание через указатель на функцию
 *
 * Проверяет, что operations[1] ссылается на функцию subtract.
 *
 * Входные данные: 5.0, 3.0
 * Ожидаемый результат: 2.0 (допуск 0.001)
 */
void test_sub(void) {
  CU_ASSERT_DOUBLE_EQUAL(operations[1](5.0, 3.0), 2.0, 0.001);
}

/*
 * Тест: Умножение через указатель на функцию
 *
 * Проверяет, что operations[2] ссылается на функцию multiply.
 *
 * Входные данные: 5.0, 3.0
 * Ожидаемый результат: 15.0 (допуск 0.001)
 */
void test_mul(void) {
  CU_ASSERT_DOUBLE_EQUAL(operations[2](5.0, 3.0), 15.0, 0.001);
}

/*
 * Тест: Нормальное деление через указатель на функцию
 *
 * Проверяет корректность деления при ненулевом делителе.
 *
 * Входные данные: 15.0, 3.0
 * Ожидаемый результат: 5.0 (допуск 0.001)
 */
void test_div(void) {
  CU_ASSERT_DOUBLE_EQUAL(operations[3](15.0, 3.0), 5.0, 0.001);
}

/*
 * Тест: Деление на ноль (граничный случай)
 *
 * В C деление double на 0.0 не вызывает краш — стандарт IEEE 754
 * определяет результат как +Infinity или -Infinity.
 * Тест проверяет, что функция divide(15.0, 0.0) возвращает бесконечность,
 * а не аварийно завершается. Логика защиты находится в main.c.
 *
 * Входные данные: 15.0, 0.0
 * Ожидаемый результат: isinf(result) == 1
 */
void test_div_zero(void) {
  double result = operations[3](15.0, 0.0);
  CU_ASSERT_TRUE(isinf(result));
}

int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite suite =
      CU_add_suite("Calc_Pointers_Suite", setup_suite, teardown_suite);
  if (NULL == suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(suite, "test_add", test_add) ||
      NULL == CU_add_test(suite, "test_sub", test_sub) ||
      NULL == CU_add_test(suite, "test_mul", test_mul) ||
      NULL == CU_add_test(suite, "test_div", test_div) ||
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
