#include "subnet_logic.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

int setup_suite(void) { return 0; }
int teardown_suite(void) { return 0; }

/*
 * Тест: Парсинг IP-адреса из строки в uint32_t
 *
 * IP-адрес хранится как 32-битное беззнаковое число (big-endian).
 * Каждый октет занимает 8 бит: 192.168.1.1 = 0xC0A80101.
 * Также проверяется обработка невалидного ввода — функция должна вернуть 0.
 *
 * Входные данные:
 *   "192.168.1.1"  -> 0xC0A80101
 *   "255.255.255.0" -> 0xFFFFFF00
 *   "invalid_ip"   -> 0 (ошибка парсинга)
 * Ожидаемый результат: соответствующие uint32_t значения
 */
void test_parse_ip(void) {
  CU_ASSERT_EQUAL(parse_ip("192.168.1.1"),
                  0xC0A80101); // 192=C0, 168=A8, 1=01, 1=01
  CU_ASSERT_EQUAL(parse_ip("255.255.255.0"), 0xFFFFFF00);
  CU_ASSERT_EQUAL(parse_ip("invalid_ip"), 0);
}

/*
 * Тест: Вычисление адреса сети
 *
 * Адрес сети = IP AND MASK. Все биты в хостовой части обнуляются.
 * Для 192.168.1.5 с маской /24 (255.255.255.0) адрес сети = 192.168.1.0.
 *
 * Входные данные: ip = 0xC0A80105 (192.168.1.5), mask = 0xFFFFFF00 (/24)
 * Ожидаемый результат: 0xC0A80100 (192.168.1.0)
 */
void test_get_network_addr(void) {
  uint32_t ip = 0xC0A80105;                                // 192.168.1.5
  uint32_t mask = 0xFFFFFF00;                              // 255.255.255.0
  CU_ASSERT_EQUAL(get_network_addr(ip, mask), 0xC0A80100); // 192.168.1.0
}

/*
 * Тест: Вычисление широковещательного адреса
 *
 * Broadcast = IP AND MASK, затем OR с инвертированной маской (~MASK).
 * Все биты хостовой части выставляются в 1.
 * Для /24: ~0xFFFFFF00 = 0x000000FF, broadcast = 192.168.1.255.
 *
 * Входные данные: ip = 0xC0A80105 (192.168.1.5), mask = 0xFFFFFF00 (/24)
 * Ожидаемый результат: 0xC0A801FF (192.168.1.255)
 */
void test_get_broadcast_addr(void) {
  uint32_t ip = 0xC0A80105;                                  // 192.168.1.5
  uint32_t mask = 0xFFFFFF00;                                // 255.255.255.0
  CU_ASSERT_EQUAL(get_broadcast_addr(ip, mask), 0xC0A801FF); // 192.168.1.255
}

/*
 * Тест: Подсчёт количества используемых хостов в подсети
 *
 * Формула: (~mask) - 1. Вычитаем 1, т.к. адрес сети и broadcast
 * не могут быть назначены хостам.
 * /24 -> ~0xFFFFFF00 = 0xFF = 255, 255 - 1 = 254 хоста.
 * /32 -> ~0xFFFFFFFF = 0, 0 - 1 = -1, приводится к 0 (нет хостов).
 * /31 -> ~0xFFFFFFFE = 1, 1 - 1 = 0 (point-to-point, нет обычных хостов).
 *
 * Входные данные: маски 0xFFFFFF00, 0xFFFFFFFF, 0xFFFFFFFE
 * Ожидаемый результат: 254, 0, 0
 */
void test_get_hosts_count(void) {
  CU_ASSERT_EQUAL(get_hosts_count(0xFFFFFF00), 254);
  CU_ASSERT_EQUAL(get_hosts_count(0xFFFFFFFF), 0);
  CU_ASSERT_EQUAL(get_hosts_count(0xFFFFFFFE), 0);
}

/*
 * Тест: Проверка принадлежности IP-адреса к подсети
 *
 * Алгоритм: (ip AND mask) == network_addr.
 * 192.168.1.5 принадлежит 192.168.1.0/24.
 * 192.168.2.5 не принадлежит — отличается третий октет.
 *
 * Входные данные: net = 0xC0A80100, mask = 0xFFFFFF00
 *   0xC0A80105 (192.168.1.5) -> 1 (принадлежит)
 *   0xC0A80205 (192.168.2.5) -> 0 (не принадлежит)
 * Ожидаемый результат: 1 и 0 соответственно
 */
void test_is_in_subnet(void) {
  uint32_t net = 0xC0A80100;                               // 192.168.1.0
  uint32_t mask = 0xFFFFFF00;                              // 255.255.255.0
  CU_ASSERT_EQUAL(is_in_subnet(0xC0A80105, net, mask), 1); // 192.168.1.5 в сети
  CU_ASSERT_EQUAL(is_in_subnet(0xC0A80205, net, mask), 0); // 192.168.2.5 не в сети
}

int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite suite = CU_add_suite("Subnet_Suite", setup_suite, teardown_suite);
  if (NULL == suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(suite, "test_parse_ip", test_parse_ip) ||
      NULL ==
          CU_add_test(suite, "test_get_network_addr", test_get_network_addr) ||
      NULL == CU_add_test(suite, "test_get_broadcast_addr",
                          test_get_broadcast_addr) ||
      NULL ==
          CU_add_test(suite, "test_get_hosts_count", test_get_hosts_count) ||
      NULL == CU_add_test(suite, "test_is_in_subnet", test_is_in_subnet)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  int failures = CU_get_number_of_failures();
  CU_cleanup_registry();
  return failures;
}
