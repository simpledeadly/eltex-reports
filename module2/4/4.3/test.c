#include "phonebook.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <string.h>

int setup_suite(void) { return 0; }
int teardown_suite(void) { return 0; }

/*
 * Тест: Вставка в бинарное дерево с сохранением свойства BST
 *
 * BST (Binary Search Tree): для каждого узла все элементы в левом
 * поддереве меньше него, в правом — больше (лексикографически).
 * Первый вставленный элемент становится корнем.
 *
 * Входные данные: вставка "M", "A", "Z"
 *   "M" — корень
 *   "A" < "M" -> левый потомок
 *   "Z" > "M" -> правый потомок
 * Ожидаемый результат: root->name == "M", root->left->name == "A",
 *                       root->right->name == "Z"
 */
void test_insert_sorted_tree(void) {
  clear_phonebook_records();
  add_contact_record("M", "1");
  add_contact_record("A", "2");
  add_contact_record("Z", "3");

  Contact *root = get_root();
  CU_ASSERT_PTR_NOT_NULL(root);
  CU_ASSERT_STRING_EQUAL(root->name, "M");

  CU_ASSERT_PTR_NOT_NULL(root->left);
  CU_ASSERT_STRING_EQUAL(root->left->name, "A");

  CU_ASSERT_PTR_NOT_NULL(root->right);
  CU_ASSERT_STRING_EQUAL(root->right->name, "Z");
}

/*
 * Тест: Удаление узла по in-order индексу с сохранением BST
 *
 * In-order обход даёт отсортированную последовательность.
 * Удаление узла перестраивает дерево так, чтобы свойство BST сохранилось.
 * После удаления корня его место занимает преемник (минимальный правый потомок).
 *
 * Входные данные: дерево из "C"(корень), "A"(left), "B"(left.right)
 *   In-order: A(1), B(2), C(3). Удаляем индекс 1 (A).
 *   Затем редактируем индекс 1 (B -> B_new) для верификации структуры.
 * Ожидаемый результат: root == "C", root->left == "B_new"
 */
void test_delete_by_index_tree(void) {
  clear_phonebook_records();
  add_contact_record("C", "3");
  add_contact_record("A", "1");
  add_contact_record("B", "2");

  int res = delete_contact_record(1); // удаляем A
  CU_ASSERT_EQUAL(res, 0);

  res = edit_contact_record(1, "B_new", "22"); // B теперь индекс 1
  CU_ASSERT_EQUAL(res, 0);

  Contact *root = get_root();
  CU_ASSERT_STRING_EQUAL(root->name, "C");
  CU_ASSERT_PTR_NOT_NULL(root->left);
  CU_ASSERT_STRING_EQUAL(root->left->name, "B_new");
}

/*
 * Тест: Изменение имени узла и его перемещение в дереве
 *
 * При изменении имени узел удаляется из текущей позиции и вставляется заново
 * на место, соответствующее новому имени. Это поддерживает инвариант BST.
 * Порядковый номер контакта в списке меняется вслед за именем.
 *
 * Входные данные: дерево "B"(корень), "A"(left), "C"(right).
 *   Меняем "B" (индекс 2 в in-order) на "Z".
 *   "B" удаляется, корнем становится "C" (минимальный правый преемник).
 *   "Z" вставляется как правый потомок "C".
 * Ожидаемый результат: root == "C", left == "A", right == "Z"
 */
void test_edit_resort_tree(void) {
  clear_phonebook_records();
  add_contact_record("B", "2");
  add_contact_record("A", "1");
  add_contact_record("C", "3");

  edit_contact_record(2, "Z", "9"); // B -> Z

  Contact *root = get_root();
  CU_ASSERT_STRING_EQUAL(root->name, "C");
  CU_ASSERT_PTR_NOT_NULL(root->left);
  CU_ASSERT_STRING_EQUAL(root->left->name, "A");
  CU_ASSERT_PTR_NOT_NULL(root->right);
  CU_ASSERT_STRING_EQUAL(root->right->name, "Z");
}

int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite suite =
      CU_add_suite("Phonebook_Tree_Suite", setup_suite, teardown_suite);
  if (NULL == suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(suite, "test_insert_sorted_tree",
                          test_insert_sorted_tree) ||
      NULL == CU_add_test(suite, "test_delete_by_index_tree",
                          test_delete_by_index_tree) ||
      NULL ==
          CU_add_test(suite, "test_edit_resort_tree", test_edit_resort_tree)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  int failures = CU_get_number_of_failures();
  CU_cleanup_registry();
  return failures;
}
