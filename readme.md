[![Build](https://github.com/adbancroft/avr-fast-map/actions/workflows/build.yml/badge.svg)](https://github.com/adbancroft/avr-fast-map/actions/workflows/build.yml)
[![Unit Tests](https://github.com/adbancroft/avr-fast-map/actions/workflows/unit-tests.yml/badge.svg)](https://github.com/adbancroft/avr-fast-map/actions/workflows/unit-tests.yml)

# avr-fast-map: a faster implementation of the Arduino map() function.

The standard Arduino `map` function is implemented using division of signed long integers. This is a very slow operation on AVR, since there is no hardware divider. The `fast_map` function provided by this library can be much faster. E.g. mapping a `uint8_t` input to a `uint8_t` output range is ~10x faster.

Exact speedup varies depending on data types & number ranges.

## Using the library

### Installation

The library is available in both the [Arduino Library](https://www.arduino.cc/reference/en/libraries/avr-fast-map/) and [PlatformIO Library](https://registry.platformio.org/libraries/adbancroft/avr-fast-map) registries. 

The library can also be cloned & included locally or included directly from GitHub (if your tooling supports it). 

### Code

1. `#include <avr-fast-map.h>`
2. Replace calls to `map` with calls to `fast-map`.

The code base is compatible with all platforms: non-AVR builds compile down to the `map` function.
