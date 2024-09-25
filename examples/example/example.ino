/*
 avr-fast-map example
*/
#include <Arduino.h>
#include <avr-fast-map.h>

void setup() {
  Serial.begin(9600);   //send and receive at 9600 baud  
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Beginning test...");

  // Randomness here is all about ensuring that the compiler doesn't optimize away the calls
  // to map()/fast_map(), which it can't do under real world conditions.
  uint32_t seedValue = rand();

  const uint8_t from_min = 2U;
  const uint8_t from_max = UINT8_MAX-7U;
  const uint8_t to_min = 11U;
  const uint8_t to_max = UINT8_MAX/4U*3U;

  constexpr uint32_t iterations = 5000;

  // Built in map() function 
  randomSeed(seedValue);
  uint32_t mapCheckSum = 0UL;
  uint32_t mapStartTime = micros();
  for (uint32_t i = 0; i < iterations; i++) {
      mapCheckSum += map(random(0, UINT8_MAX), from_min, from_max, to_min, to_max);
  }
  uint32_t mapEndTime = micros();
  uint32_t mapDuration = mapEndTime-mapStartTime;

  // fast_map()
  randomSeed(seedValue);
  uint32_t fastmapCheckSum = 0UL;
  uint32_t fastmapStartTime = micros();
  for (uint32_t i = 0U; i < iterations; i++) {
    fastmapCheckSum += fast_map((uint8_t)random(0, UINT8_MAX), from_min, from_max, to_min, to_max);    
  }
  uint32_t fastmapEndTime = micros();
  uint32_t fastmapDuration = fastmapEndTime-fastmapStartTime;

  char msg[128];
  sprintf(msg, "map() Checksum: %" PRIu32 ", fast_map() Checksum: %" PRIu32, mapCheckSum, fastmapCheckSum);
  Serial.println(msg);
  sprintf(msg, "map() Duration: %" PRIu32 ", fast_map() Duration: %" PRIu32, mapDuration, fastmapDuration);
  Serial.println(msg);
  uint16_t percentDelta = (uint16_t)(fastmapDuration * 100U / mapDuration);
  sprintf(msg, "fast_map() took %" PRIu16 "%% less time than the map() function", 100-percentDelta);
  Serial.println(msg);
}

void loop() {

    // Blink to indicate end of test
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
}

