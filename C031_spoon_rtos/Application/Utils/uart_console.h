/*******************************************************************************
** @file:		uart_console.h
** @author:		HUY DAI
** @version:	V1.0.0
** @time:		11:00 AM Wednesday, 31 January, 2018
** @brief:		define debug header
** @revision:
**             	- version 1.0.1: <date> <name>
**             	<discribe the change>
**             	- version 1.0.1: <date> <name>
**             	<discribe the change>
*******************************************************************************/

#ifndef _UART_CONSOLE_H_
#define _UART_CONSOLE_H_

/******************************************************************************
**                               INCLUDES
**
*******************************************************************************/
#include "cmn.h"
#include <stdarg.h>
#include <stddef.h>


/******************************************************************************
**                               DEFINES
*******************************************************************************/								
#define USE_DEBUG     (1) // if use debug define =1 if do not use define =0
#define FULL_DEBUG    (0) // if use full debug then define =1,
#define DATA_DEBUG    (1) // if use data and debug then define =2,
#define NORMAL_DEBUG  (0) // if use data and debug then define =2,
#if USE_DEBUG    
#if FULL_DEBUG    
//use full debug
#define debug_printf(a,args...) UARTprintf("-Debug:[%s %d]"a,__FILE__,__LINE__,##args)

#elif DATA_DEBUG
// use debug normal
#define normal_printf(a,args...) {UARTprintf(a,##args);}
#define debug_printf(a,args...) {UARTprintf(a,##args);}
#define data_printf(a,args...) {UARTprintf("<DATA>");UARTprintf(a,##args);UARTprintf("</DATA>\r\n");}
#define dataGPS_printf(a,args...) {UARTprintf("\t");UARTprintf(a,##args);}
#define debug_off(a,args...) {}
#else
#define debug_printf(a,args...) UARTprintf(a,##args)
#define data_printf(a,args...) 
#endif

#else
// do not user debug
#define debug_printf(a,args...)     {}
#endif





/******************************************************************************
**                     EXTERNAL VARIABLE DECLARATIONS
*******************************************************************************/

/******************************************************************************
**                      EXTERNAL FUNCTION PROTOTYPES
*******************************************************************************/
//extern void UARTprintf( char *pcString, ...);
void UARTprintf(const char* format, ...);





///////////////////////////////////////////////////////////////////////////////
// \author (c) Marco Paland (info@paland.com)
//             2014-2018, PALANDesign Hannover, Germany
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// \brief Tiny printf, sprintf and snprintf implementation, optimized for speed on
//        embedded systems with a very limited resources.
//        Use this instead of bloated standard/newlib printf.
//        These routines are thread safe and reentrant.
//
///////////////////////////////////////////////////////////////////////////////







/**
 * Output a character to a custom device like UART, used by the printf() function
 * This function is declared here only. You have to write your custom implementation somewhere
 * \param character Character to output
 */
void _putchar(char character);


/**
 * Tiny printf implementation
 * You have to implement _putchar if you use printf()
 * To avoid conflicts with the regular printf() API it is overridden by macro defines
 * and internal underscore-appended functions like printf_() are used
 * \param format A string that specifies the format of the output
 * \return The number of characters that are written into the array, not counting the terminating null character
 */
#define printf printf_
int printf_(const char* format, ...);


/**
 * Tiny sprintf implementation
 * Due to security reasons (buffer overflow) YOU SHOULD CONSIDER USING (V)SNPRINTF INSTEAD!
 * \param buffer A pointer to the buffer where to store the formatted string. MUST be big enough to store the output!
 * \param format A string that specifies the format of the output
 * \return The number of characters that are WRITTEN into the buffer, not counting the terminating null character
 */
//#define sprintf sprintf_
int sprintf_(char* buffer, const char* format, ...);


/**
 * Tiny snprintf/vsnprintf implementation
 * \param buffer A pointer to the buffer where to store the formatted string
 * \param count The maximum number of characters to store in the buffer, including a terminating null character
 * \param format A string that specifies the format of the output
 * \return The number of characters that are WRITTEN into the buffer, not counting the terminating null character
 *         If the formatted string is truncated the buffer size (count) is returned
 */
//#define snprintf  snprintf_
#define vsnprintf vsnprintf_
int  snprintf_(char* buffer, size_t count, const char* format, ...);
int vsnprintf_(char* buffer, size_t count, const char* format, va_list va);


/**
 * printf with output function
 * You may use this as dynamic alternative to printf() with its fixed _putchar() output
 * \param out An output function which takes one character and an argument pointer
 * \param arg An argument pointer for user data passed to output function
 * \param format A string that specifies the format of the output
 * \return The number of characters that are sent to the output function, not counting the terminating null character
 */
int fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...);


#ifdef __cplusplus
}
#endif





#endif
