#include "pqueue.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <string.h>

int setup_suite(void) { return 0; }
int teardown_suite(void) { return 0; }

/*
 * Тест: Извлечение элемента с наивысшим приоритетом
 *
 * В очередь добавляются элементы с разными приоритетами.
 * dequeue_first обязан вернуть элемент с максимальным приоритетом
 * независимо от порядка вставки.
 *
 * Входные данные: "Low"(1), "High"(10), "Medium"(5)
 * Ожидаемый результат: data == "High", prio == 10, res == 0
 */
void test_enqueue_dequeue_highest(void) {
  clear_queue();
  enqueue("Low", 1);
  enqueue("High", 10);
  enqueue("Medium", 5);

  char data[64];
  uint8_t prio;

  int res = dequeue_first(data, &prio);
  CU_ASSERT_EQUAL(res, 0);
  CU_ASSERT_STRING_EQUAL(data, "High");
  CU_ASSERT_EQUAL(prio, 10);
}

/*
 * Тест: Порядок FIFO при одинаковом приоритете
 *
 * Если приоритеты совпадают, очередь ведёт себя как FIFO:
 * первым должен извлекаться тот элемент, который был вставлен раньше.
 *
 * Входные данные: "First"(5), "Second"(5)
 * Ожидаемый результат: первый dequeue -> "First", второй -> "Second"
 */
void test_fifo_same_priority(void) {
  clear_queue();
  enqueue("First", 5);
  enqueue("Second", 5);

  char data[64];
  uint8_t prio;

  dequeue_first(data, &prio);
  CU_ASSERT_STRING_EQUAL(data, "First");

  dequeue_first(data, &prio);
  CU_ASSERT_STRING_EQUAL(data, "Second");
}

/*
 * Тест: Извлечение элемента по точному значению приоритета
 *
 * dequeue_exact_priority ищет и извлекает конкретный элемент по приоритету,
 * не затрагивая остальные. Если приоритет не найден — возвращает ошибку.
 *
 * Входные данные: "Target"(7), "Other"(3)
 *   dequeue_exact_priority(7) -> "Target", res == 0
 *   dequeue_exact_priority(99) -> не найден, res == 1
 * Ожидаемый результат: соответственно описанию выше
 */
void test_dequeue_exact_priority(void) {
  clear_queue();
  enqueue("Target", 7);
  enqueue("Other", 3);

  char data[64];
  uint8_t prio;

  int res = dequeue_exact_priority(7, data, &prio);
  CU_ASSERT_EQUAL(res, 0);
  CU_ASSERT_STRING_EQUAL(data, "Target");
  CU_ASSERT_EQUAL(prio, 7);

  res = dequeue_exact_priority(99, data, &prio);
  CU_ASSERT_EQUAL(res, 1);
}

/*
 * Тест: Извлечение из пустой очереди (граничный случай)
 *
 * При попытке извлечь элемент из пустой очереди функция
 * должна вернуть код ошибки (1), не вызывая краш.
 *
 * Входные данные: пустая очередь
 * Ожидаемый результат: res == 1
 */
void test_empty_queue(void) {
  clear_queue();
  char data[64];
  uint8_t prio;
  CU_ASSERT_EQUAL(dequeue_first(data, &prio), 1);
}

int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite suite =
      CU_add_suite("PriorityQueue_Suite", setup_suite, teardown_suite);
  if (NULL == suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(suite, "test_enqueue_dequeue_highest",
                          test_enqueue_dequeue_highest) ||
      NULL == CU_add_test(suite, "test_fifo_same_priority",
                          test_fifo_same_priority) ||
      NULL == CU_add_test(suite, "test_dequeue_exact_priority",
                          test_dequeue_exact_priority) ||
      NULL == CU_add_test(suite, "test_empty_queue", test_empty_queue)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  int failures = CU_get_number_of_failures();
  CU_cleanup_registry();
  return failures;
}
