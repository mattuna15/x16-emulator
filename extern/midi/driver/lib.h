/*
 * lib.h - Library functions.
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *  Marco van den Heuvel <blackystardust68@yahoo.com>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef DEBUG
/* memory leak pinpointing, don't forget to enable in lib.c */
#define LIB_DEBUG_PINPOINT
#endif

extern void lib_init_rand(void);
extern unsigned int lib_unsigned_rand(unsigned int min, unsigned int max);
extern float lib_float_rand(float min, float max);

extern char *lib_msprintf(const char *fmt, ...);
extern char *lib_mvsprintf(const char *fmt, va_list args);

extern void lib_debug_check(void);

#if defined(__CYGWIN32__) || defined(__CYGWIN__) || defined(WIN32_COMPILE)

extern size_t lib_tcstostr(char *str, const char *tcs, size_t len);
extern size_t lib_strtotcs(char *tcs, const char *str, size_t len);

extern int lib_snprintf(char *str, size_t len, const char *fmt, ...);
#define lib_sntprintf lib_snprintf

#endif /* CYGWIN or WIN32_COMPILE */

#ifdef LIB_DEBUG_PINPOINT
extern void *lib_malloc_pinpoint(size_t size, const char *name, unsigned int line);
extern void *lib_calloc_pinpoint(size_t nmemb, size_t size, const char *name, unsigned int line);
extern void *lib_realloc_pinpoint(void *p, size_t size, const char *name, unsigned int line);
extern void lib_free_pinpoint(void *p, const char *name, unsigned int line);

extern char *lib_strdup_pinpoint(const char *str, const char *name, unsigned int line);

#ifndef COMPILING_LIB_DOT_C

#define lib_malloc(x) lib_malloc_pinpoint(x, __FILE__, __LINE__)
#define lib_free(x) lib_free_pinpoint(x, __FILE__, __LINE__)
#define lib_calloc(x, y) lib_calloc_pinpoint(x, y, __FILE__, __LINE__)
#define lib_realloc(x, y) lib_realloc_pinpoint(x, y, __FILE__, __LINE__)
#define lib_strdup(x) lib_strdup_pinpoint(x, __FILE__, __LINE__)

#endif //not defined COMPILING_LIB_DOT_C
#endif
