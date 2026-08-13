#include "plugin_logic.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

int setup_suite(void) { return 0; }
int teardown_suite(void) { return 0; }

/*
 * Тест: Загрузка .dylib плагинов через dlopen и вызов через dlsym
 *
 * Проверяет весь цикл работы с плагинами:
 * 1. load_plugins() сканирует папку ./plugins, открывает каждый .dylib файл
 *    через dlopen и получает указатель на функцию через dlsym.
 * 2. После загрузки plugin_count > 0 (есть хотя бы один плагин).
 * 3. Функция "add" из плагина libadd.dylib вызывается через указатель
 *    и возвращает корректный результат (5.0 + 3.0 = 8.0).
 * 4. unload_plugins() корректно вызывает dlclose для каждого handle
 *    и сбрасывает plugin_count до 0.
 *
 * Входные данные: плагины в ./plugins/ (add, sub, mul, div)
 * Ожидаемый результат:
 *   plugin_count > 0 после загрузки
 *   plugins[i].func(5.0, 3.0) == 8.0 для плагина "add"
 *   plugin_count == 0 после выгрузки
 */
void test_load_plugins(void) {
  load_plugins();
  CU_ASSERT_TRUE(plugin_count > 0);

  int add_found = 0;
  for (int i = 0; i < plugin_count; i++) {
    if (strcmp(plugins[i].name, "add") == 0) {
      add_found = 1;
      CU_ASSERT_DOUBLE_EQUAL(plugins[i].func(5.0, 3.0), 8.0, 0.001);
    }
  }
  CU_ASSERT_TRUE(add_found);
  unload_plugins();
  CU_ASSERT_EQUAL(plugin_count, 0);
}

int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite suite = CU_add_suite("Plugin_Suite", setup_suite, teardown_suite);
  if (NULL == suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(suite, "test_load_plugins", test_load_plugins)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  int failures = CU_get_number_of_failures();
  CU_cleanup_registry();
  return failures;
}
