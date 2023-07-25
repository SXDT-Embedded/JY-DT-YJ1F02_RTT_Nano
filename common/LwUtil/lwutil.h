/*
 * @Author       : yzy
 * @Date         : 2023-02-08 13:29:28
 * @LastEditors  : yzy
 * @LastEditTime : 2023-02-13 10:57:39
 * @FilePath     : \CH32V203C8T6_LetterShell\CommonLibrary\LwUtil\lwutil.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
/**
 * \file            lwutil.h
 * \brief           Lightweight utility library
 */

/*
 * Copyright (c) 2023 Tilen MAJERLE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of LwUTIL - Lightweight utility library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         v1.0.0
 */
#ifndef LWUTIL_HDR_H
#define LWUTIL_HDR_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        LWUTIL Lightweight utility
 * \brief           Lightweight utility
 * \{
 */

/**
 * \brief           Get size of statically allocated array
 * Array must be declared in a form of `type var_name[element_count]`
 *
 * \param[in]       x: Object to get array size of
 * \return          Number of elements in array (`element_count`)
 */
#define LWUTIL_ARRAYSIZE(x)      (sizeof(x) / sizeof((x)[0]))

/**
 * \brief           Get larger value out of 2 different inputs
 * \param[in]       x: First input
 * \param[in]       y: Second input
 * \return          Larger of both inputs
 */
#define LWUTIL_MAX(x, y)         ((x) > (y) ? (x) : (y))

/**
 * \brief           Get smaller value out of 2 different inputs
 * \param[in]       x: First input
 * \param[in]       y: Second input
 * \return          Smaller of both inputs
 */
#define LWUTIL_MIN(x, y)         ((x) < (y) ? (x) : (y))

/**
 * \brief           Get absolute value of the input
 * Returns always-positive value of the input.
 * \note            Special care must be taken when input variable holds
 * minimum value available for given signed integer type (char, int, long, ...).
 * Making absolute value of such input means longer output type requirement.
 * Consider writing result of this function to unsigned type of same integer width.
 * For example, minimum `signed char` value is `-128` while its absolute value (`128`)
 * requires variable type of either `unsigned char` or minimum signed 16-bit (or more).
 * \param[in]       x: Input value
 * \return          Absolute value of the input value
 */
#define LWUTIL_ABS(x)            ((x) < 0 ? -(x) : (x))

/**
 * \brief           Unused variable to avoid compilation warning if declared but not used
 * \param[in]       x: Input variable to declare unused
 */
#define LWUTIL_UNUSED(x)         (void)(x)

#define LWUTIL_CONCAT_BASE(x, y) x##y
#define LWUTIL_CONCAT(s0, s1)    LWUTIL_CONCAT_BASE(s0, s1)

/**
 * \brief           Compile time assert to validate specific expression
 * \note            Can only be used with the integer types
 * \param[in]       exp: Expression to test. It must be compile-time evaluation
 * \param[in]       random_variable_name: Meaningful variable name to be used.
 *                      Can be whatever until it is valid variable name
 */
#define LWUTIL_COMPILE_TIME_ASSERT(exp, random_variable_name)                                                          \
    typedef char LWUTIL_CONCAT2(random_variable_name, __LINE__)[!(exp) ? -1 : 1];

/**
 * \brief           Check if all bits in the `bit_mask` are set in the input value
 * \note            Can only be used with the integer types
 * \param[in]       val: Value to check for bits in
 * \param[in]       bit_mask: Bit mask to check in value
 * \return          `1` if all bits are set, `0` otherwise
 */
#define lwutil_bits_is_set_all(val, bit_mask) (((val) & (bit_mask)) == (bit_mask))

/**
 * \brief           Check if any of the `bit_mask` bits is set in the input value
 * \note            Can only be used with the integer types
 * \param[in]       val: Value to check for bits in
 * \param[in]       bit_mask: Bit mask to check in value
 * \return          `1` if any bit is set, `0` otherwise
 */
#define lwutil_bits_is_set_any(val, bit_mask) (((val) & (bit_mask)) != 0)

/**
 * \brief           Set bit mask in the input value
 * \note            Can only be used with the integer types
 * \param[in]       val: Value to set bits in.
 *                      Original input is not modified. It is pass-by-value.
 * \param[in]       bit_mask: Bit mask indicating which bits to set
 * \return          New value with bitwise OR between input value and bit mask
 */
#define lwutil_bits_set(val, bit_mask)        ((val) | (bit_mask))

/**
 * \brief           Clear bit mask in the input value
 * \note            Can only be used with the integer types
 * \param[in]       val: Value to clear bits in.
 *                      Original input is not modified. It is pass-by-value.
 * \param[in]       bit_mask: Bit mask indicating which bits to clear
 * \return          New value with bitwise AND and negated bit_mask between input value and bit mask
 *                  Value has bits cleared in the bit_mask set
 */
#define lwutil_bits_clear(val, bit_mask)      ((val) & ~(bit_mask))

/**
 * \brief           Toggle bit mask in the input value
 * Used only for integer types
 * \param[in]       val: Value to toggle bits in.
 *                      Original input is not modified. It is pass-by-value.
 * \param[in]       bit_mask: Bit mask indicating which bits to toggle
 * \return          New value with bitwise AND and negated bit_mask between input value and bit mask
 *                  Value has bits cleared in the bit_mask set
 */
#define lwutil_bits_toggle(val, bit_mask)     ((val) ^ (bit_mask))

/**
 * \brief           Store `16-bit` value to bytes array in little-endian format
 * \param[in]       val: Value to write to output array
 * \param[out]      ptr: Minimum `2-bytes` long output array to write value to
 */
static inline void
lwutil_st_u16_le(uint16_t val, void* ptr) {
    uint8_t* p = ptr;

    p[0] = (uint8_t)((val >> 0) & 0xFF);
    p[1] = (uint8_t)((val >> 8) & 0xFF);
}

/**
 * \brief           Store `32-bit` value to bytes array in little-endian format
 * \param[in]       val: Value to write to output array
 * \param[out]      ptr: Minimum `4-bytes` long output array to write value to
 */
static inline void
lwutil_st_u32_le(uint32_t val, void* ptr) {
    uint8_t* p = ptr;

    p[0] = (uint8_t)((val >> 0) & 0xFF);
    p[1] = (uint8_t)((val >> 8) & 0xFF);
    p[2] = (uint8_t)((val >> 16) & 0xFF);
    p[3] = (uint8_t)((val >> 24) & 0xFF);
}

/**
 * \brief           Load `16-bit` value from bytes array in little-endian format
 * \param[in]       ptr: Minimum `2-bytes` long input array to extract bytes from
 * \return          `16-bit` value extracted from input array
 */
static inline uint16_t
lwutil_ld_u16_le(const void* ptr) {
    const uint8_t* p = ptr;
    return p[1] << 8 | p[0];
}

/**
 * \brief           Load `32-bit` value from bytes array in little-endian format
 * \param[in]       ptr: Minimum `2-bytes` long input array to extract bytes from
 * \return          `32-bit` value extracted from input array
 */
static inline uint32_t
lwutil_ld_u32_le(const void* ptr) {
    const uint8_t* p = ptr;
    return p[3] << 24 | p[2] << 16 | p[1] << 8 | p[0];
}

/**
 * \brief           Store `16-bit` value to bytes array in big-endian format
 * \param[in]       val: Value to write to output array
 * \param[out]      ptr: Minimum `2-bytes` long output array to write value to
 */
static inline void
lwutil_st_u16_be(uint16_t val, void* ptr) {
    uint8_t* p = ptr;

    p[0] = (uint8_t)((val >> 8) & 0xFF);
    p[1] = (uint8_t)((val >> 0) & 0xFF);
}

/**
 * \brief           Store `32-bit` value to bytes array in big-endian format
 * \param[in]       val: Value to write to output array
 * \param[out]      ptr: Minimum `4-bytes` long output array to write value to
 */
static inline void
lwutil_st_u32_be(uint32_t val, void* ptr) {
    uint8_t* p = ptr;

    p[0] = (uint8_t)((val >> 24) & 0xFF);
    p[1] = (uint8_t)((val >> 16) & 0xFF);
    p[2] = (uint8_t)((val >> 8) & 0xFF);
    p[3] = (uint8_t)((val >> 0) & 0xFF);
}

/**
 * \brief           Load `16-bit` value from bytes array in big-endian format
 * \param[in]       ptr: Minimum `2-bytes` long input array to extract bytes from
 * \return          `16-bit` value extracted from input array
 */
static inline uint16_t
lwutil_ld_u16_be(const void* ptr) {
    const uint8_t* p = ptr;
    return p[0] << 8 | p[1];
}

/**
 * \brief           Load `32-bit` value from bytes array in big-endian format
 * \param[in]       ptr: Minimum `4-bytes` long input array to extract bytes from
 * \return          `32-bit` value extracted from input array
 */
static inline uint32_t
lwutil_ld_u32_be(const void* ptr) {
    const uint8_t* p = ptr;
    return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
}

void lwutil_uint8_t_to_2asciis(uint8_t hex, char* ascii);
void lwutil_u16_to_4asciis(uint16_t hex, char* ascii);
void lwutil_u32_to_8asciis(uint32_t hex, char* ascii);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LWUTIL_HDR_H */
