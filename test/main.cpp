#include <Arduino.h>
#include <unity.h>
#include <avr/sleep.h>

void test_fast_map_implementation(void);
void test_fast_map(void) ;
void test_fast_map_perf(void);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    // delay(2000);

#if defined(CORE_TEENSY)
    // Without this, Teensy 3.5 produces a linker error:
    //  "undefined reference to `_write'""
    Serial.println("");
#endif

    UNITY_BEGIN(); 
    test_fast_map_implementation();
    test_fast_map();
    test_fast_map_perf();
    UNITY_END(); 
    
    // Tell SimAVR we are done
    cli();
    sleep_enable();
    sleep_cpu();
}

void loop()
{
    // Blink to indicate end of test
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
}