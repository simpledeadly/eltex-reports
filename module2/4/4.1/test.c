#include "phonebook.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <string.h>

int setup_suite(void) { return 0; }
int teardown_suite(void) { return 0; }

/*
 * Тест: Добавление первого элемента в пустой список
 *
 * После вставки единственного элемента:
 * - head должен указывать на него (список не пуст)
 * - prev == NULL (нет предыдущего)
 * - next == NULL (нет следующего)
 *
 * Входные данные: name = "A", phone = "123"
 * Ожидаемый результат: head != NULL, head->name == "A",
 *                       head->prev == NULL, head->next == NULL
 */
void test_add_first(void) {
  clear_phonebook_records();
  add_contact_record("A", "123");
  Contact *head = get_head();

  CU_ASSERT_PTR_NOT_NULL(head);
  CU_ASSERT_STRING_EQUAL(head->name, "A");
  CU_ASSERT_PTR_NULL(head->prev);
  CU_ASSERT_PTR_NULL(head->next);
}

/*
 * Тест: Вставка с сохранением лексикографического порядка
 *
 * Элементы добавляются в произвольном порядке ("C", "A", "B").
 * Список должен автоматически поддерживать сортировку при каждой вставке.
 * Проверяем полную цепочку: head -> next -> next -> NULL.
 *
 * Входные данные: "C", "A", "B" (порядок вставки)
 * Ожидаемый результат: список в порядке "A" -> "B" -> "C",
 *                       последний next == NULL
 */
void test_sorted_insert(void) {
  clear_phonebook_records();
  add_contact_record("C", "3");
  add_contact_record("A", "1");
  add_contact_record("B", "2");

  Contact *head = get_head();

  CU_ASSERT_PTR_NOT_NULL(head);
  CU_ASSERT_STRING_EQUAL(head->name, "A");
  CU_ASSERT_PTR_NOT_NULL(head->next);
  CU_ASSERT_STRING_EQUAL(head->next->name, "B");
  CU_ASSERT_PTR_NOT_NULL(head->next->next);
  CU_ASSERT_STRING_EQUAL(head->next->next->name, "C");
  CU_ASSERT_PTR_NULL(head->next->next->next);
}

/*
 * Тест: Изменение имени контакта и пересортировка списка
 *
 * При редактировании имени элемент удаляется со старой позиции
 * и вставляется заново на место, соответствующее новому имени.
 * Тест проверяет, что порядковый номер в списке меняется корректно.
 *
 * Входные данные: список "A"(1), "B"(2), "C"(3). Меняем "A" (индекс 1) на "Z".
 * Ожидаемый результат: список "B" -> "C" -> "Z"
 */
void test_edit_resort(void) {
  clear_phonebook_records();
  add_contact_record("C", "3");
  add_contact_record("A", "1");
  add_contact_record("B", "2");

  edit_contact_record(1, "Z", "9"); // "A" -> "Z"

  Contact *head = get_head();
  CU_ASSERT_STRING_EQUAL(head->name, "B");
  CU_ASSERT_STRING_EQUAL(head->next->name, "C");
  CU_ASSERT_STRING_EQUAL(head->next->next->name, "Z");
}

/*
 * Тест: Удаление элементов из начала и конца списка
 *
 * Проверяет корректность переопределения указателей при удалении
 * крайних элементов. После удаления head должен переключиться
 * на следующий элемент, хвост — освободить память.
 *
 * Входные данные: список "A"(1), "B"(2), "C"(3).
 *   Удаляем индекс 1 ("A") -> список: "B", "C"
 *   Удаляем индекс 2 ("C") -> список: только "B", next == NULL
 * Ожидаемый результат: после обоих удалений остаётся один элемент "B"
 */
void test_delete_edges(void) {
  clear_phonebook_records();
  add_contact_record("A", "1");
  add_contact_record("B", "2");
  add_contact_record("C", "3");

  delete_contact_record(1); // delete A
  Contact *head = get_head();
  CU_ASSERT_STRING_EQUAL(head->name, "B");

  delete_contact_record(2); // delete C
  CU_ASSERT_STRING_EQUAL(head->name, "B");
  CU_ASSERT_PTR_NULL(head->next);
}

int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite suite =
      CU_add_suite("Phonebook_DList_Suite", setup_suite, teardown_suite);
  if (NULL == suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(suite, "test_add_first", test_add_first) ||
      NULL == CU_add_test(suite, "test_sorted_insert", test_sorted_insert) ||
      NULL == CU_add_test(suite, "test_edit_resort", test_edit_resort) ||
      NULL == CU_add_test(suite, "test_delete_edges", test_delete_edges)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  int failures = CU_get_number_of_failures();
  CU_cleanup_registry();
  return failures;
}
