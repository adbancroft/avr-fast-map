#include <Arduino.h>
#include <unity.h>
#include "avr-fast-map.h"
#include "test_utils.h"

template <typename T, typename U>
static void assert_fast_map(T in, T inMin, T inMax, U outMin, U outMax) {
    U expected = (U)map(in, inMin, inMax, outMin, outMax);
    U actual = fast_map(in, inMin, inMax, outMin, outMax);
    char szMsg[256];
    sprintf(szMsg, "In %" PRId32 ", InMin %" PRId32 ", InMax %" PRId32 ", OutMin %" PRId32 ", OutMax %" PRId32, 
    (int32_t)in, (int32_t)inMin, (int32_t)inMax, (int32_t)outMin, (int32_t)outMax);
    TEST_ASSERT_EQUAL_MESSAGE(expected, actual, szMsg);  
}

template <typename T, typename U>
struct fast_map_test_data
{
  T inMin;
  T inMax;
  T inStep;
  bool invertT;
  U outMin;
  U outMax;
  bool invertU;
};

template <typename T, typename U>
static void test_fast_map(const fast_map_test_data<T, U> &test)
{
  for (uint32_t i = test.inMin; i <= (uint32_t)test.inMax; i = (i + test.inStep))
  {
    T inMin = test.invertT ? test.inMax : test.inMin;
    T inMax = test.invertT ? test.inMin : test.inMax;
    U outMin = test.invertU ? test.outMax : test.outMin;
    U outMax = test.invertU ? test.outMin : test.outMax;
    assert_fast_map((T)i, inMin, inMax, outMin, outMax);
  }
}  

static void test_maths_fastMap_U16xU16_same_direction(void)
{
    const uint16_t inMin = 3;
    const uint16_t inMax = 233;
    const uint16_t outMin = 256;
    const uint16_t outMax = 512;

    {
      fast_map_test_data<uint16_t, uint16_t> test_params = {inMin, inMax, 1, false, outMin, outMax, false};
      test_fast_map(test_params);
    }
    {
      fast_map_test_data<uint16_t, uint16_t> test_params = {inMin, inMax, 1, true, outMin, outMax, true};
      test_fast_map(test_params);
    }
}

static void test_maths_fastMap_U16xU16_reverse_direction(void)
{
    const uint16_t inMin = 3;
    const uint16_t inMax = 233;
    const uint16_t outMin = 256;
    const uint16_t outMax = 512;

    {
        fast_map_test_data<uint16_t, uint16_t> test_params = {inMin, inMax, 1, false, outMin, outMax, true};
        test_fast_map(test_params);
    }

    {
        fast_map_test_data<uint16_t, uint16_t> test_params = {inMin, inMax, 1, true, outMin, outMax, false};
        test_fast_map(test_params);
    }
}

static void test_maths_fastMap_S8xS16_same_direction(void)
{
    const int8_t inMin = 3;
    const int8_t inMax = 123;
    const int16_t outMin = -23579;
    const int16_t outMax = -15973;

    {
      fast_map_test_data<int8_t, int16_t> test_params = {inMin, inMax, 1, false, outMin, outMax, false};
      test_fast_map(test_params);
    }
    {
      fast_map_test_data<int8_t, int16_t> test_params = {inMin, inMax, 1, true, outMin, outMax, true};
      test_fast_map(test_params);
    }
}

static void test_maths_fastMap_S8xS16_different_direction(void)
{
    const int8_t inMin = 3;
    const int8_t inMax = 123;
    const int16_t outMin = -23579;
    const int16_t outMax = -15973;

    {
        fast_map_test_data<int8_t, int16_t> test_params = {inMin, inMax, 1, false, outMin, outMax, true};
        test_fast_map(test_params);
    }

    {
        fast_map_test_data<int8_t, int16_t> test_params = {inMin, inMax, 1, true, outMin, outMax, false};
        test_fast_map(test_params);
    }
}

static void test_maths_fastMap_S16xS16_below_range(void)
{
    const int16_t inMin = -1500;
    const int16_t inMax = -11123;
    const int16_t outMin = 1200;
    const int16_t outMax = 5000;    

    {
      int16_t in = inMin - 5000;    
      assert_fast_map(in, inMin, inMax, outMin, outMax);
    }

    // Invert the range
    {
      int16_t in = inMax + 5000;    
      assert_fast_map(in, inMax, inMin, outMin, outMax);
    }
}

static void test_maths_fastMap_S16xS16_above_range(void)
{
    const int16_t inMin = -1500;
    const int16_t inMax = -11123;
    const int16_t outMin = 1200;
    const int16_t outMax = 5000;    
    const int16_t in = inMax + 1500;    

    assert_fast_map(in, inMin, inMax, outMin, outMax);
}

void test_fast_map(void) {
  SET_UNITY_FILENAME() {
    RUN_TEST(test_maths_fastMap_U16xU16_same_direction);
    RUN_TEST(test_maths_fastMap_U16xU16_reverse_direction);
    RUN_TEST(test_maths_fastMap_S8xS16_same_direction);
    RUN_TEST(test_maths_fastMap_S8xS16_different_direction);
    RUN_TEST(test_maths_fastMap_S16xS16_below_range);
    RUN_TEST(test_maths_fastMap_S16xS16_above_range);
  }
}