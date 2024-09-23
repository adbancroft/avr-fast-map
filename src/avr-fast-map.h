#pragma once

#include <avr-fast-div.h>

/**
 * @file
 * @brief A faster implementation of the Arduino map() function.
 * 
 * Takes advantage of our optimized unsigned division functions.
 *
 */

/// @cond

// Below namespaces are private details of the fast map implementation 

// A poor, but adequate, replacement for type traits templates in the
// C++ standard library.
//
// Note that these are all compile time operations.
namespace type_traits {
    // Limited replacement for std::conditional
    // Primary template for true
    template<bool _Cond, typename _Iftrue, typename _Iffalse>
        struct conditional
        { typedef _Iftrue type; };

    // Partial specialization for false.
    template<typename _Iftrue, typename _Iffalse>
        struct conditional<false, _Iftrue, _Iffalse>
        { typedef _Iffalse type; };    

  // Limited replacement for std::make_unsigned 
  template<typename _Tp>
    struct make_signed { typedef _Tp type; };
  
  template<> 
    struct make_signed<uint8_t> { typedef int8_t type; };

  template<>
    struct make_signed<uint16_t> { typedef int16_t type; };

  template<>
    struct make_signed<uint32_t> { typedef int32_t type; };

  template<>
    struct make_signed<uint64_t> { typedef int64_t type; };

  template<typename _Tp>
    using make_signed_t = typename make_signed<_Tp>::type;
}

// 
namespace fast_map_impl {

    // Widen an integral type to the next larger type.
    // This is used to avoid overflow during the calculation.
    template < typename T > struct widen_integral { } ;
    template< > struct widen_integral<uint8_t> {
        typedef uint16_t type;
    };
    template< > struct widen_integral<uint16_t> {
        typedef uint32_t type;
    };
    template< > struct widen_integral<uint32_t> {
        typedef uint64_t type;
    };
    template< > struct widen_integral<int8_t> {
        typedef int16_t type;
    };
    template< > struct widen_integral<int16_t> {
        typedef int32_t type;
    };
    template< > struct widen_integral<int32_t> {
        typedef int64_t type;
    };    
    template< typename T >
    using widen_integral_t = typename widen_integral<T>::type;
 
    // Get the absolute difference between two values.
    // This is used to handle negative ranges.
    // Equivalent of abs(min-max)
    template <typename T>
    static inline type_traits::make_unsigned_t<T> absDelta(const T &min, const T &max) {
        if (max<min) {
            return (T)(min - max);
        }
        return (T)(max - min);
    }

    template <typename T, typename U, 
              // Note that std::common_type will not do what we want here, as it will use integer
              // calculation promotion rules. So everything will be "int" at a minimum - our
              // goal is to use the narrowest type possible for performance reasons.
              typename TCommon = typename type_traits::conditional<(sizeof(T) >= sizeof(U)), T, U>::type,
              // Widen the common type - this is our intermediate calculation type
              // to avoid overflow.
              typename TResult = typename fast_map_impl::widen_integral_t<TCommon>>
    TResult safeMultiply(const T &a, const U &b) {
        static_assert(type_traits::is_signed<T>::value==type_traits::is_signed<U>::value, "Both types must be signed or unsigned");
        return (TResult)(static_cast<TResult>(a) * static_cast<TResult>(b));
    }

}

/// @endcond

/**
 * @brief Optimized version of Arduino's map() function.
 * 
 * * For mapping 8-bit to 8-bit, it's 10x faster (9%)
 * * For mapping 16-bit to 16-bit, it's up to 3x faster (30%)
 * * For mapping 8-bit to 16-bit values, it's up to 3x faster (30%)
 * 
 * (see unit tests)
 * 
 * @tparam TIn Input range type
 * @tparam TOut Output range type
 * @param in Input value
 * @param inMin Input range minimum
 * @param inMax Input range maximum
 * @param outMin Output range minimum
 * @param outMax Output range maximum
 * @return TOut
 */
template <typename TIn, typename TOut>
static inline TOut fast_map(TIn in, TIn inMin, TIn inMax, TOut outMin, TOut outMax) {
    /* Float version (if m, yMax, yMin and n were float's)
        int yVal = (m * (yMax - yMin)) / n;
    */
    // We use unsigned types for performance and to avoid integer overflow.
    typedef typename type_traits::make_unsigned_t<TIn> in_unsigned_t;
    typedef typename type_traits::make_unsigned_t<TOut> out_unsigned_t;

    const in_unsigned_t m = fast_map_impl::absDelta(inMin, in);
    const in_unsigned_t inRange = fast_map_impl::absDelta(inMin, inMax);
    const out_unsigned_t outRange = fast_map_impl::absDelta(outMin, outMax);
    // fast_div will do the heavy lifting of optimizing integral type widths,
    // so no impact even if safeMultiply returns a bigger than necessary type
    const out_unsigned_t scaled = (out_unsigned_t)fast_div(fast_map_impl::safeMultiply(m, outRange), inRange);

    // Since we use unsigned types to avoid under/overflow, we need to adjust for a few cases
    // where the result should be less than outMin
    const bool inLowerThanInMin = (in<inMin);
    const bool inRangeInverted = (inMax<inMin);
    const bool inOpposite = inLowerThanInMin!=inRangeInverted;
    const bool outRangeInverted = (outMax<outMin);
    if (inOpposite!=outRangeInverted) {
      return (TOut)(outMin - scaled);     
    }
    return (TOut)(outMin + scaled);    
}
