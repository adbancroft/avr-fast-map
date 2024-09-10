#include <Arduino.h>
#include <unity.h>
#include "avr-fast-map.h"
#include "test_utils.h"

static void test_safeMultiply_u8u8(void) {
  TEST_ASSERT_EQUAL_UINT16(0, fast_map_impl::safeMultiply((uint8_t)0, (uint8_t)0));  
  TEST_ASSERT_EQUAL_UINT16((uint16_t)UINT8_MAX * UINT8_MAX, fast_map_impl::safeMultiply((uint8_t)UINT8_MAX, (uint8_t)UINT8_MAX));  
}

static void test_safeMultiply_u16u8(void) {
  TEST_ASSERT_EQUAL_UINT32(0, fast_map_impl::safeMultiply((uint16_t)0, (uint8_t)0));  
  TEST_ASSERT_EQUAL_UINT32((uint32_t)UINT16_MAX * UINT8_MAX, fast_map_impl::safeMultiply((uint16_t)UINT16_MAX, (uint8_t)UINT8_MAX));  
  TEST_ASSERT_EQUAL_UINT32((uint32_t)UINT16_MAX * UINT8_MAX, fast_map_impl::safeMultiply((uint8_t)UINT8_MAX, (uint16_t)UINT16_MAX));  
}

static void test_safeMultiply_u16u16(void) {
  TEST_ASSERT_EQUAL_UINT32(0, fast_map_impl::safeMultiply((uint16_t)0, (uint16_t)0));  
  TEST_ASSERT_EQUAL_UINT32((uint32_t)UINT16_MAX * UINT16_MAX, fast_map_impl::safeMultiply((uint16_t)UINT16_MAX, (uint16_t)UINT16_MAX));  
}

static void test_safeMultiply_s8s8(void) {
  TEST_ASSERT_EQUAL_INT16(0, fast_map_impl::safeMultiply((int8_t)0, (int8_t)0));  
  TEST_ASSERT_EQUAL_INT16((int16_t)INT8_MAX * INT8_MAX, fast_map_impl::safeMultiply((int8_t)INT8_MAX, (int8_t)INT8_MAX));  
}

static void test_safeMultiply_s16s8(void) {
  TEST_ASSERT_EQUAL_INT32(0, fast_map_impl::safeMultiply((int16_t)0, (int8_t)0));  
  TEST_ASSERT_EQUAL_INT32((int32_t)INT16_MAX * INT8_MAX, fast_map_impl::safeMultiply((int16_t)INT16_MAX, (int8_t)INT8_MAX));  
  TEST_ASSERT_EQUAL_INT32((int32_t)INT16_MAX * INT8_MAX, fast_map_impl::safeMultiply((int8_t)INT8_MAX, (int16_t)INT16_MAX));  
}

static void test_safeMultiply_s16s16(void) {
  TEST_ASSERT_EQUAL_INT32(0, fast_map_impl::safeMultiply((int16_t)0, (int16_t)0));  
  TEST_ASSERT_EQUAL_INT32((int32_t)INT16_MAX * INT16_MAX, fast_map_impl::safeMultiply((int16_t)INT16_MAX, (int16_t)INT16_MAX));  
}

static void test_absDelta_u8(void) {
    TEST_ASSERT_EQUAL_UINT8(0, fast_map_impl::absDelta((uint8_t)0, (uint8_t)0));
    TEST_ASSERT_EQUAL_UINT8(0, fast_map_impl::absDelta((uint8_t)UINT8_MAX, (uint8_t)UINT8_MAX));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, fast_map_impl::absDelta((uint8_t)0, (uint8_t)UINT8_MAX));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, fast_map_impl::absDelta((uint8_t)UINT8_MAX, (uint8_t)0));
}

static void test_absDelta_u16(void) {
    TEST_ASSERT_EQUAL_UINT16(0, fast_map_impl::absDelta((uint16_t)0, (uint16_t)0));
    TEST_ASSERT_EQUAL_UINT16(0, fast_map_impl::absDelta((uint16_t)UINT16_MAX, (uint16_t)UINT16_MAX));
    TEST_ASSERT_EQUAL_UINT16(UINT16_MAX, fast_map_impl::absDelta((uint16_t)0, (uint16_t)UINT16_MAX));
    TEST_ASSERT_EQUAL_UINT16(UINT16_MAX, fast_map_impl::absDelta((uint16_t)UINT16_MAX, (uint16_t)0));
}

static void test_absDelta_u32(void) {
    TEST_ASSERT_EQUAL_UINT32(0, fast_map_impl::absDelta((uint32_t)0, (uint32_t)0));
    TEST_ASSERT_EQUAL_UINT32(0, fast_map_impl::absDelta((uint32_t)UINT32_MAX, (uint32_t)UINT32_MAX));
    TEST_ASSERT_EQUAL_UINT32(UINT32_MAX, fast_map_impl::absDelta((uint32_t)0, (uint32_t)UINT32_MAX));
    TEST_ASSERT_EQUAL_UINT32(UINT32_MAX, fast_map_impl::absDelta((uint32_t)UINT32_MAX, (uint32_t)0));
}

static void test_absDelta_s8(void) {
    TEST_ASSERT_EQUAL_UINT8(0, fast_map_impl::absDelta((int8_t)0, (int8_t)0));
    TEST_ASSERT_EQUAL_UINT8(0, fast_map_impl::absDelta((int8_t)INT8_MAX, (int8_t)INT8_MAX));
    TEST_ASSERT_EQUAL_UINT8(0, fast_map_impl::absDelta((int8_t)INT8_MIN, (int8_t)INT8_MIN));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, fast_map_impl::absDelta((int8_t)INT8_MIN, (int8_t)INT8_MAX));
    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, fast_map_impl::absDelta((int8_t)INT8_MAX, (int8_t)INT8_MIN));
}

static void test_absDelta_s16(void) {
    TEST_ASSERT_EQUAL_UINT16(0, fast_map_impl::absDelta((int16_t)0, (int16_t)0));
    TEST_ASSERT_EQUAL_UINT16(0, fast_map_impl::absDelta((int16_t)INT16_MAX, (int16_t)INT16_MAX));
    TEST_ASSERT_EQUAL_UINT16(0, fast_map_impl::absDelta((int16_t)INT16_MIN, (int16_t)INT16_MIN));
    TEST_ASSERT_EQUAL_UINT16(UINT16_MAX, fast_map_impl::absDelta((int16_t)INT16_MIN, (int16_t)INT16_MAX));
    TEST_ASSERT_EQUAL_UINT16(UINT16_MAX, fast_map_impl::absDelta((int16_t)INT16_MAX, (int16_t)INT16_MIN));
}

static void test_absDelta_s32(void) {
    TEST_ASSERT_EQUAL_UINT32(0, fast_map_impl::absDelta((int32_t)0, (int32_t)0));
    TEST_ASSERT_EQUAL_UINT32(0, fast_map_impl::absDelta((int32_t)INT32_MAX, (int32_t)INT32_MAX));
    TEST_ASSERT_EQUAL_UINT32(0, fast_map_impl::absDelta((int32_t)INT32_MIN, (int32_t)INT32_MIN));
    TEST_ASSERT_EQUAL_UINT32(UINT32_MAX, fast_map_impl::absDelta((int32_t)INT32_MIN, (int32_t)INT32_MAX));
    TEST_ASSERT_EQUAL_UINT32(UINT32_MAX, fast_map_impl::absDelta((int32_t)INT32_MAX, (int32_t)INT32_MIN));
}

void test_fast_map_implementation(void) {
  SET_UNITY_FILENAME() {
    RUN_TEST(test_safeMultiply_u8u8);
    RUN_TEST(test_safeMultiply_u16u8);
    RUN_TEST(test_safeMultiply_u16u16);
    RUN_TEST(test_safeMultiply_s8s8);
    RUN_TEST(test_safeMultiply_s16s8);
    RUN_TEST(test_safeMultiply_s16s16);
    RUN_TEST(test_absDelta_u8);
    RUN_TEST(test_absDelta_u16);
    RUN_TEST(test_absDelta_u32);
    RUN_TEST(test_absDelta_s8);
    RUN_TEST(test_absDelta_s16);
    RUN_TEST(test_absDelta_s32);
  }
}