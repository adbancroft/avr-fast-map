#include <Arduino.h>
#include <unity.h>
#include "avr-fast-map.h"
#include "lambda_timer.hpp"
#include "test_utils.h"
#include "unity_print_timers.hpp"


static void test_fastmap_perf_8x8_map(void)
{
  const uint16_t iters = 50;
  const uint8_t inMin = 3;
  const uint8_t inMax = 233;
  const uint8_t step = 1;
  const uint8_t outMin = 0;
  const uint8_t outMax = 255;

  auto nativeTest = [] (uint8_t index, uint32_t &checkSum) { checkSum += map(index, inMin, inMax, outMin, outMax); };
  auto optimizedTest = [] (uint8_t index, uint32_t &checkSum) { checkSum += fast_map(index, inMin, inMax, outMin, outMax); };
  auto comparison = compare_executiontime<uint8_t, uint32_t>(iters, inMin, inMax, step, nativeTest, optimizedTest);
  
  MESSAGE_TIMERS(comparison.timeA.timer, comparison.timeB.timer);
  TEST_ASSERT_EQUAL(comparison.timeA.result, comparison.timeB.result);

#if defined(__AVR__) // We only expect a speed improvement on AVR
  TEST_ASSERT_LESS_THAN(comparison.timeA.timer.duration_micros(), comparison.timeB.timer.duration_micros());
#endif
}

static void test_fastmap_perf_8x16_map(void)
{
  const uint16_t iters = 50;
  const uint8_t inMin = 3;
  const uint8_t inMax = 233;
  const uint8_t step = 1;
  const uint16_t outMin = (UINT16_MAX/10)*2;
  const uint16_t outMax = (UINT16_MAX/10)*3;

  auto nativeTest = [] (uint8_t index, uint32_t &checkSum) {    checkSum +=      map(index, inMin, inMax, outMin, outMax); };
  auto optimizedTest = [] (uint8_t index, uint32_t &checkSum) { checkSum += fast_map(index, inMin, inMax, outMin, outMax); };
  auto comparison = compare_executiontime<uint8_t, uint32_t>(iters, inMin, inMax, step, nativeTest, optimizedTest);
  
  MESSAGE_TIMERS(comparison.timeA.timer, comparison.timeB.timer);
  TEST_ASSERT_EQUAL(comparison.timeA.result, comparison.timeB.result);

#if defined(__AVR__) // We only expect a speed improvement on AVR
  TEST_ASSERT_LESS_THAN(comparison.timeA.timer.duration_micros(), comparison.timeB.timer.duration_micros());
#endif
}

static void test_fastmap_perf_8x8_16x16(void)
{
  const uint16_t iters = 50;
  const uint8_t inMin = 3;
  const uint8_t inMax = 233;
  const uint8_t step = 1;
  const uint8_t outMin = 0;
  const uint8_t outMax = 255;

  auto nativeTest = [] (uint8_t index, uint32_t &checkSum) { checkSum += fast_map((uint16_t)index, (uint16_t)inMin, (uint16_t)inMax, (uint16_t)outMin, (uint16_t)outMax); };
  auto optimizedTest = [] (uint8_t index, uint32_t &checkSum) { checkSum += fast_map(index, inMin, inMax, outMin, outMax); };
  auto comparison = compare_executiontime<uint8_t, uint32_t>(iters, inMin, inMax, step, nativeTest, optimizedTest);
  
  MESSAGE_TIMERS(comparison.timeA.timer, comparison.timeB.timer);
  TEST_ASSERT_EQUAL(comparison.timeA.result, comparison.timeB.result);

#if defined(__AVR__) // We only expect a speed improvement on AVR
  TEST_ASSERT_LESS_THAN(comparison.timeA.timer.duration_micros(), comparison.timeB.timer.duration_micros());
#endif
}

static void test_fastmap_perf_16x16_map(void)
{
  const uint16_t iters = 50;
  const uint16_t inMin = 1521;
  const uint16_t inMax = 53333;
  const uint16_t step = 331;
  const uint16_t outMin = (UINT16_MAX/10)*2;
  const uint16_t outMax = (UINT16_MAX/10)*3;

  auto nativeTest = [] (uint16_t index, uint32_t &checkSum) { checkSum += map(index, inMin, inMax, outMin, outMax); };
  auto optimizedTest = [] (uint16_t index, uint32_t &checkSum) { checkSum += fast_map(index, inMin, inMax, outMin, outMax); };
  auto comparison = compare_executiontime<uint16_t, uint32_t>(iters, inMin, inMax, step, nativeTest, optimizedTest);
  
  MESSAGE_TIMERS(comparison.timeA.timer, comparison.timeB.timer);
  TEST_ASSERT_EQUAL(comparison.timeA.result, comparison.timeB.result);

#if defined(__AVR__) // We only expect a speed improvement on AVR
  TEST_ASSERT_LESS_THAN(comparison.timeA.timer.duration_micros(), comparison.timeB.timer.duration_micros());
#endif
}

void test_fast_map_perf(void) {
  SET_UNITY_FILENAME() {
    RUN_TEST(test_fastmap_perf_8x8_map);
    RUN_TEST(test_fastmap_perf_16x16_map);
    RUN_TEST(test_fastmap_perf_8x16_map);
    RUN_TEST(test_fastmap_perf_8x8_16x16);
  }
}