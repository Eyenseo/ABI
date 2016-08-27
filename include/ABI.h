// Copyright (c) 2016 Roland Jaeger
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
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef EYENSEO_ABI_H
#define EYENSEO_ABI_H
/**
 * The main macro defined by this header is `E_ABI(s)`
 *
 * The macro is a convenience macro to export or import symbols. The normal
 * solution is to create a header for each library, copy & paste the contents of
 * an existing library and then change the macros that are being defined by
 * search and replace.
 *
 * This header is the only header that you'll need from now on:
 *
 *      void E_ABI(mylib) foo();
 *
 * will evaluate to different statements depending on defined macros.
 *
 *      |      import     |      export     |         static         |
 *      |                 |  E_ABI_mylib    |  E_ABI_STATIC_mylib    |
 *      |  E_ABI_mylib 0  |  E_ABI_mylib 1  |  E_ABI_STATIC_mylib 1  |
 *      |  E_ABI_mylib 2  |  -------------  |  --------------------  |
 *      |  E_ABI_mylib a  |  -------------  |  --------------------  |
 *      |       ...       |  -------------  |  --------------------  |
 *
 * + Import means either `__declspec(dllimport)` or
 *   `__attribute__((visibility("default")))` depending on the platform /
 *   compiler.
 * + Export means either `__declspec(dllexport)` or
 *   `__attribute__((visibility("default")))` depending on the platform /
 *   compiler.
 * + Static means that the macro will be evaluated to nothing -- the linker will
 *   take care of everything.
 */


//////////////////////////////////////////
// Internal macros -- subjects to change
//////////////////////////////////////////
/**
 * Check which compiler is used
 */
#if defined(_MSC_VER)
#define EYENSEO_ABI_COMPILER_MSVC 1
#elif defined(__GNUC__)
#define EYENSEO_ABI_COMPILER_GCC 1
// Clang is compatible to gcc so we don't need a special case
#else
#error Unknown compiler, please implement shared library macros
#endif

/**
 * Magic ...
 *
 * https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms
 */
#define EYENSEO_ABI_PASS(...) __VA_ARGS__

#define EYENSEO_ABI_CAT(a, ...) a##__VA_ARGS__

#define EYENSEO_ABI_IF_ELSE_IMPL_0(t, f) f
#define EYENSEO_ABI_IF_ELSE_IMPL_1(t, f) t
#define EYENSEO_ABI_IF_ELSE_IMPL(c) \
  EYENSEO_ABI_CAT(EYENSEO_ABI_IF_ELSE_IMPL_, c)

#ifdef EYENSEO_ABI_COMPILER_MSVC
// You screwed up msvc
#define EYENSEO_ABI_SECOND_IMPL(a, b, ...) b
#define EYENSEO_ABI_SECOND(...) \
  EYENSEO_ABI_PASS(EYENSEO_ABI_SECOND_IMPL(__VA_ARGS__))
#else
#define EYENSEO_ABI_SECOND(a, b, ...) b
#endif

#define EYENSEO_ABI_CHECK(...) EYENSEO_ABI_SECOND(__VA_ARGS__, 0, ~)

#define EYENSEO_ABI_IS_1() ~, 1
#define EYENSEO_ABI_IS_() ~, 1
#define EYENSEO_ABI_IS(...) \
  EYENSEO_ABI_CHECK(EYENSEO_ABI_CAT(EYENSEO_ABI_IS_, __VA_ARGS__)())

#ifdef EYENSEO_ABI_COMPILER_MSVC
// You screwed up msvc ... again
#define EYENSEO_ABI_IF_ELSE(c) \
  EYENSEO_ABI_IF_ELSE_IMPL(EYENSEO_ABI_PASS(EYENSEO_ABI_IS(c)))
#else
#define EYENSEO_ABI_IF_ELSE(c) EYENSEO_ABI_IF_ELSE_IMPL(EYENSEO_ABI_IS(c))
#endif

//////////////////////////////////////////
// Main macro
//////////////////////////////////////////
/**
 * Compiler switch for the main macro `E_ABI(s)`
 */
#ifdef EYENSEO_ABI_COMPILER_MSVC
#define E_ABI(s)                                                     \
  EYENSEO_ABI_IF_ELSE(EYENSEO_ABI_CAT(E_ABI_STATIC_, s))             \
  (/* If static: Static libraries will be handled by the linker */,  \
   /* Else:      */ EYENSEO_ABI_IF_ELSE(EYENSEO_ABI_CAT(E_ABI_, s))( \
       /* If defined:   We export */ __declspec(dllexport),          \
       /* Else:         We import */ __declspec(dllimport)))
#elif defined(EYENSEO_ABI_COMPILER_GCC)
#define E_ABI(s)                                                             \
  EYENSEO_ABI_IF_ELSE(EYENSEO_ABI_CAT(E_ABI_STATIC_, s))                     \
  (/* If static: Static libraries will be handled by the linker */,          \
   /* Else:      */ EYENSEO_ABI_IF_ELSE(EYENSEO_ABI_CAT(E_ABI_, s))(         \
       /* If defined:   We export */ __attribute__((visibility("default"))), \
       /* Else:         We import */ __attribute__((visibility("default")))))
#endif
#endif
