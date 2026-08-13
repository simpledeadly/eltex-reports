#include "chmod_logic.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

int setup_suite(void) { return 0; }
int teardown_suite(void) { return 0; }

/*
 * Тест: Парсинг строкового представления прав в восьмеричную маску
 *
 * Функция parse_string_mode принимает 9-символьную строку вида "rwxr-xr-x"
 * и конвертирует её в числовую маску. Каждый символ — бит:
 * 'r'=4, 'w'=2, 'x'=1, '-'=0. Строка разбита на 3 тройки (owner, group, others).
 *
 * Входные данные:
 *   "rwxr-xr-x" -> owner=7(rwx), group=5(r-x), others=5(r-x) = 0755
 *   "rw-r--r--" -> owner=6(rw-), group=4(r--), others=4(r--) = 0644
 *   "rwx------" -> owner=7(rwx), group=0(---), others=0(---) = 0700
 * Ожидаемый результат: соответствующее восьмеричное число
 */
void test_parse_string_mode(void) {
  CU_ASSERT_EQUAL(parse_string_mode("rwxr-xr-x"), 0755);
  CU_ASSERT_EQUAL(parse_string_mode("rw-r--r--"), 0644);
  CU_ASSERT_EQUAL(parse_string_mode("rwx------"), 0700);
}

/*
 * Тест: Добавление прав через операцию '+'
 *
 * Проверяет, что apply_chmod корректно устанавливает биты через OR.
 * 'a' = all (owner+group+others), 'g' = group.
 *
 * Входные данные:
 *   apply_chmod(0644, "a+x") — добавить бит исполнения всем
 *   Ожидаемый результат: 0755 (0644 | 0111)
 *
 *   apply_chmod(0644, "g+w") — добавить запись для группы
 *   Ожидаемый результат: 0664 (0644 | 0020)
 */
void test_apply_chmod_add(void) {
  CU_ASSERT_EQUAL(apply_chmod(0644, "a+x"), 0755);
  CU_ASSERT_EQUAL(apply_chmod(0644, "g+w"), 0664);
}

/*
 * Тест: Снятие прав через операцию '-'
 *
 * Проверяет, что apply_chmod корректно сбрасывает биты через AND с инвертированной маской.
 * 'o' = others.
 *
 * Входные данные:
 *   apply_chmod(0777, "o-w") — снять запись у others
 *   Ожидаемый результат: 0775 (0777 & ~0002)
 *
 *   apply_chmod(0755, "a-x") — снять исполнение у всех
 *   Ожидаемый результат: 0644 (0755 & ~0111)
 */
void test_apply_chmod_remove(void) {
  CU_ASSERT_EQUAL(apply_chmod(0777, "o-w"), 0775);
  CU_ASSERT_EQUAL(apply_chmod(0755, "a-x"), 0644);
}

/*
 * Тест: Установка прав через операцию '='
 *
 * Операция '=' сначала обнуляет биты целевой группы, затем устанавливает новые.
 * В отличие от '+', она не сохраняет предыдущие биты группы.
 *
 * Входные данные:
 *   apply_chmod(0644, "g=rwx") — установить группе rwx
 *   Ожидаемый результат: 0674 (group=7, owner=6, others=4)
 *
 *   apply_chmod(0777, "o=r") — установить others только чтение
 *   Ожидаемый результат: 0774 (others=4)
 */
void test_apply_chmod_set(void) {
  CU_ASSERT_EQUAL(apply_chmod(0644, "g=rwx"), 0674);
  CU_ASSERT_EQUAL(apply_chmod(0777, "o=r"), 0774);
}

int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite suite = CU_add_suite("Chmod_Suite", setup_suite, teardown_suite);
  if (NULL == suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(suite, "test_parse_string_mode",
                          test_parse_string_mode) ||
      NULL ==
          CU_add_test(suite, "test_apply_chmod_add", test_apply_chmod_add) ||
      NULL == CU_add_test(suite, "test_apply_chmod_remove",
                          test_apply_chmod_remove) ||
      NULL ==
          CU_add_test(suite, "test_apply_chmod_set", test_apply_chmod_set)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  int failures = CU_get_number_of_failures();
  CU_cleanup_registry();
  return failures;
}
