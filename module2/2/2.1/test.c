#include "phonebook.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <string.h>

// Очистка глобального состояния перед каждым тестом
int setup_suite(void) {
  contact_count = 0;
  return 0;
}

int teardown_suite(void) { return 0; }

/*
 * Тест: Добавление контакта в пустую книгу
 *
 * Проверяет базовую работу функции add_contact_record:
 * - Функция принимает готовую структуру Contact
 * - Контакт должен сохраниться в phonebook[0]
 * - Счётчик contact_count должен увеличиться с 0 до 1
 * - Функция должна вернуть 0
 *
 * Входные данные: name = "Ivan", surname = "Ivanov"
 * Ожидаемый результат: res == 0, contact_count == 1,
 *                       phonebook[0].name == "Ivan",
 *                       phonebook[0].surname == "Ivanov"
 */
void test_add_contact(void) {
  contact_count = 0;
  struct Contact c;
  memset(&c, 0, sizeof(c));
  strcpy(c.name, "Ivan");
  strcpy(c.surname, "Ivanov");

  int res = add_contact_record(c);

  CU_ASSERT_EQUAL(res, 0);
  CU_ASSERT_EQUAL(contact_count, 1);
  CU_ASSERT_STRING_EQUAL(phonebook[0].name, "Ivan");
  CU_ASSERT_STRING_EQUAL(phonebook[0].surname, "Ivanov");
}

/*
 * Тест: Добавление при полностью заполненной книге
 *
 * Проверяет защиту от выхода за границы массива phonebook[100].
 * При достижении лимита функция обязана отказать, не записывая данные.
 * contact_count не должен измениться.
 *
 * Входные данные: contact_count = 100, пустая структура
 * Ожидаемый результат: res == -1, contact_count == 100
 */
void test_add_full(void) {
  contact_count = 100;
  struct Contact c;
  memset(&c, 0, sizeof(c));

  int res = add_contact_record(c);

  CU_ASSERT_EQUAL(res, -1);
  CU_ASSERT_EQUAL(contact_count, 100);
}

/*
 * Тест: Редактирование существующего контакта по индексу
 *
 * Проверяет, что edit_contact_record корректно заменяет запись по индексу.
 * Изменяется только поле name — подтверждение, что функция заменяет
 * структуру целиком и не затрагивает другие записи в phonebook.
 *
 * Входные данные: phonebook[0] = ("Ivan", "Ivanov"),
 *                 новые данные = ("Petr", "Ivanov"), индекс = 0
 * Ожидаемый результат: res == 0, phonebook[0].name == "Petr",
 *                       phonebook[0].surname == "Ivanov"
 */
void test_edit_contact(void) {
  contact_count = 1;
  strcpy(phonebook[0].name, "Ivan");
  strcpy(phonebook[0].surname, "Ivanov");

  struct Contact edited;
  memset(&edited, 0, sizeof(edited));
  strcpy(edited.name, "Petr");
  strcpy(edited.surname, "Ivanov");

  int res = edit_contact_record(0, edited);

  CU_ASSERT_EQUAL(res, 0);
  CU_ASSERT_STRING_EQUAL(phonebook[0].name, "Petr");
  CU_ASSERT_STRING_EQUAL(phonebook[0].surname, "Ivanov");
}

/*
 * Тест: Удаление контакта из начала с последующим сдвигом элементов
 *
 * Проверяет, что delete_contact_record удаляет запись по индексу и
 * сдвигает все последующие элементы влево на одну позицию.
 * После удаления phonebook[1] должен стать phonebook[0].
 *
 * Входные данные: phonebook = [("Ivan"), ("Petr")], contact_count = 2,
 *                 удаляем индекс 0
 * Ожидаемый результат: res == 0, contact_count == 1,
 *                       phonebook[0].name == "Petr"
 */
void test_delete_contact(void) {
  contact_count = 2;
  memset(&phonebook[0], 0, sizeof(struct Contact));
  memset(&phonebook[1], 0, sizeof(struct Contact));
  strcpy(phonebook[0].name, "Ivan");
  strcpy(phonebook[1].name, "Petr");

  int res = delete_contact_record(0);

  CU_ASSERT_EQUAL(res, 0);
  CU_ASSERT_EQUAL(contact_count, 1);
  CU_ASSERT_STRING_EQUAL(phonebook[0].name, "Petr");
}

int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite suite =
      CU_add_suite("Phonebook_Array_Suite", setup_suite, teardown_suite);
  if (NULL == suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(suite, "test_add_contact", test_add_contact) ||
      NULL == CU_add_test(suite, "test_add_full", test_add_full) ||
      NULL == CU_add_test(suite, "test_edit_contact", test_edit_contact) ||
      NULL == CU_add_test(suite, "test_delete_contact", test_delete_contact)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  int failures = CU_get_number_of_failures();
  CU_cleanup_registry();

  return failures;
}
