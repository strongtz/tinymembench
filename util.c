/*
 * Copyright © 2011 Siarhei Siamashka <siarhei.siamashka@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#include "util.h"

void aligned_block_copy(int64_t * __restrict dst_,
                        int64_t * __restrict src,
                        int                  size)
{
    volatile int64_t *dst = dst_;
    int64_t t1, t2, t3, t4;
    while ((size -= 64) >= 0)
    {
        t1 = *src++;
        t2 = *src++;
        t3 = *src++;
        t4 = *src++;
        *dst++ = t1;
        *dst++ = t2;
        *dst++ = t3;
        *dst++ = t4;
        t1 = *src++;
        t2 = *src++;
        t3 = *src++;
        t4 = *src++;
        *dst++ = t1;
        *dst++ = t2;
        *dst++ = t3;
        *dst++ = t4;
    }
}

void aligned_block_copy_backwards(int64_t * __restrict dst_,
                                  int64_t * __restrict src,
                                  int                  size)
{
    volatile int64_t *dst = dst_;
    int64_t t1, t2, t3, t4;
    src += size / 8 - 1;
    dst += size / 8 - 1;
    while ((size -= 64) >= 0)
    {
        t1 = *src--;
        t2 = *src--;
        t3 = *src--;
        t4 = *src--;
        *dst-- = t1;
        *dst-- = t2;
        *dst-- = t3;
        *dst-- = t4;
        t1 = *src--;
        t2 = *src--;
        t3 = *src--;
        t4 = *src--;
        *dst-- = t1;
        *dst-- = t2;
        *dst-- = t3;
        *dst-- = t4;
    }
}

/*
 * Walk memory addresses in the backwards direction, but still
 * copy each individual 32 byte block in the forward direction.
 */
void aligned_block_copy_backwards_bs32(int64_t * __restrict dst_,
                                       int64_t * __restrict src,
                                       int                  size)
{
    volatile int64_t *dst = dst_;
    int64_t t1, t2, t3, t4;
    src += size / 8 - 8;
    dst += size / 8 - 8;
    while ((size -= 64) >= 0)
    {
        t1 = src[4];
        t2 = src[5];
        t3 = src[6];
        t4 = src[7];
        dst[4] = t1;
        dst[5] = t2;
        dst[6] = t3;
        dst[7] = t4;
        t1 = src[0];
        t2 = src[1];
        t3 = src[2];
        t4 = src[3];
        dst[0] = t1;
        dst[1] = t2;
        dst[2] = t3;
        dst[3] = t4;
        src -= 8;
        dst -= 8;
    }
}

/*
 * Walk memory addresses in the backwards direction, but still
 * copy each individual 64 byte block in the forward direction.
 */
void aligned_block_copy_backwards_bs64(int64_t * __restrict dst_,
                                       int64_t * __restrict src,
                                       int                  size)
{
    volatile int64_t *dst = dst_;
    int64_t t1, t2, t3, t4;
    src += size / 8 - 8;
    dst += size / 8 - 8;
    while ((size -= 64) >= 0)
    {
        t1 = src[0];
        t2 = src[1];
        t3 = src[2];
        t4 = src[3];
        dst[0] = t1;
        dst[1] = t2;
        dst[2] = t3;
        dst[3] = t4;
        t1 = src[4];
        t2 = src[5];
        t3 = src[6];
        t4 = src[7];
        dst[4] = t1;
        dst[5] = t2;
        dst[6] = t3;
        dst[7] = t4;
        src -= 8;
        dst -= 8;
    }
}

void aligned_block_copy_pf32(int64_t * __restrict dst_,
                             int64_t * __restrict src,
                             int                  size)
{
    volatile int64_t *dst = dst_;
    int64_t t1, t2, t3, t4;
    while ((size -= 64) >= 0)
    {
        __builtin_prefetch(src + 32, 0, 0);
        t1 = *src++;
        t2 = *src++;
        t3 = *src++;
        t4 = *src++;
        *dst++ = t1;
        *dst++ = t2;
        *dst++ = t3;
        *dst++ = t4;
        __builtin_prefetch(src + 32, 0, 0);
        t1 = *src++;
        t2 = *src++;
        t3 = *src++;
        t4 = *src++;
        *dst++ = t1;
        *dst++ = t2;
        *dst++ = t3;
        *dst++ = t4;
    }
}

void aligned_block_copy_pf64(int64_t * __restrict dst_,
                             int64_t * __restrict src,
                             int                  size)
{
    volatile int64_t *dst = dst_;
    int64_t t1, t2, t3, t4;
    while ((size -= 64) >= 0)
    {
        __builtin_prefetch(src + 32, 0, 0);
        t1 = *src++;
        t2 = *src++;
        t3 = *src++;
        t4 = *src++;
        *dst++ = t1;
        *dst++ = t2;
        *dst++ = t3;
        *dst++ = t4;
        t1 = *src++;
        t2 = *src++;
        t3 = *src++;
        t4 = *src++;
        *dst++ = t1;
        *dst++ = t2;
        *dst++ = t3;
        *dst++ = t4;
    }
}

void aligned_block_fill(int64_t * __restrict dst_,
                        int64_t * __restrict src,
                        int                  size)
{
    volatile int64_t *dst = dst_;
    int64_t data = *src;
    while ((size -= 64) >= 0)
    {
        *dst++ = data;
        *dst++ = data;
        *dst++ = data;
        *dst++ = data;
        *dst++ = data;
        *dst++ = data;
        *dst++ = data;
        *dst++ = data;
    }
}

/*
 * Simulate reshuffled memory write accesses to the destination
 * buffer (a kind of "drunken master style" access pattern).
 *
 * See: https://github.com/ssvb/tinymembench/issues/7
 */
void aligned_block_fill_shuffle16(int64_t * __restrict dst_,
                                  int64_t * __restrict src,
                                  int                  size)
{
    volatile int64_t *dst = dst_;
    int64_t data = *src;
    while ((size -= 64) >= 0)
    {
        dst[0 + 0] = data;
        dst[1 + 0] = data;
        dst[1 + 2] = data;
        dst[0 + 2] = data;
        dst[1 + 4] = data;
        dst[0 + 4] = data;
        dst[0 + 6] = data;
        dst[1 + 6] = data;
        dst += 8;
    }
}

void aligned_block_fill_shuffle32(int64_t * __restrict dst_,
                                  int64_t * __restrict src,
                                  int                  size)
{
    volatile int64_t *dst = dst_;
    int64_t data = *src;
    while ((size -= 64) >= 0)
    {
        dst[3 + 0] = data;
        dst[0 + 0] = data;
        dst[2 + 0] = data;
        dst[1 + 0] = data;
        dst[3 + 4] = data;
        dst[0 + 4] = data;
        dst[2 + 4] = data;
        dst[1 + 4] = data;
        dst += 8;
    }
}

void aligned_block_fill_shuffle64(int64_t * __restrict dst_,
                                  int64_t * __restrict src,
                                  int                  size)
{
    volatile int64_t *dst = dst_;
    int64_t data = *src;
    while ((size -= 64) >= 0)
    {
        dst[5] = data;
        dst[2] = data;
        dst[7] = data;
        dst[6] = data;
        dst[1] = data;
        dst[3] = data;
        dst[0] = data;
        dst[4] = data;
        dst += 8;
    }
}

void aligned_block_scan_8(int64_t * __restrict dst_,
                          int64_t * __restrict src,
                          int                  size)
{
    volatile uint8_t *dst = (uint8_t *)dst_;
    uint8_t *data = (uint8_t *)src;
    /* NOTE: Not exactly same semantics as memchr: we won't return NULL if not found. */
    while ((size -= 1) >= 0 && *data != SEARCH_8)
    {
        data += 1;
    }
    /* assignment is to prevent compiler optimizing this away */
    ((void**)dst)[0] = (void*)data;
}

void aligned_block_scan_16(int64_t * __restrict dst_,
                           int64_t * __restrict src,
                           int                  size)
{
    volatile uint16_t *dst = (uint16_t *)dst_;
    uint16_t *data = (uint16_t *)src;
    /* NOTE: Not exactly same semantics as memchr: we stop at first non-filler
             value and won't return NULL if not found. */
    while ((size -= 2) >= 0 && *data == FILL_16)
    {
        data += 1;
    }
    /* assignment is to prevent compiler optimizing this away */
    ((void**)dst)[0] = (void*)data;
}

void aligned_block_scan_32(int64_t * __restrict dst_,
                           int64_t * __restrict src,
                           int                  size)
{
    volatile uint32_t *dst = (uint32_t *)dst_;
    uint32_t *data = (uint32_t *)src;
    /* NOTE: Not exactly same semantics as memchr: we stop at first non-filler
             value and won't return NULL if not found. */
    while ((size -= 4) >= 0 && *data == FILL_32)
    {
        data += 1;
    }
    /* assignment is to prevent compiler optimizing this away */
    ((void**)dst)[0] = (void*)data;
}

void aligned_block_scan_64(int64_t * __restrict dst_,
                           int64_t * __restrict src,
                           int                  size)
{
    volatile uint64_t *dst = (uint64_t *)dst_;
    uint64_t *data = (uint64_t *)src;
    /* NOTE: Not exactly same semantics as memchr: we stop at first non-filler
             value and won't return NULL if not found. */
    while ((size -= 8) >= 0 && *data == FILL_64)
    {
        data += 1;
    }
    /* assignment is to prevent compiler optimizing this away */
    ((void**)dst)[0] = (void*)data;
}

double gettime(void)
{
    struct timespec tp;

    clock_gettime(CLOCK_MONOTONIC, &tp);
    return (double)((int64_t)tp.tv_sec * 1000000000 + tp.tv_nsec) / 1000000000.;
}

double fmin(double a, double b)
{
    return a < b ? a : b;
}

#define ALIGN_PADDING    0x100000
#define CACHE_LINE_SIZE  128

static char *align_up(char *ptr, int align)
{
    return (char *)(((uintptr_t)ptr + align - 1) & ~(uintptr_t)(align - 1));
}

void *alloc_four_nonaliased_buffers(void **buf1_, int size1,
                                    void **buf2_, int size2,
                                    void **buf3_, int size3,
                                    void **buf4_, int size4)
{
    char **buf1 = (char **)buf1_, **buf2 = (char **)buf2_;
    char **buf3 = (char **)buf3_, **buf4 = (char **)buf4_;
    int antialias_pattern_mask = (ALIGN_PADDING - 1) & ~(CACHE_LINE_SIZE - 1);
    char *buf, *ptr;

    if (!buf1 || size1 < 0)
        size1 = 0;
    if (!buf2 || size2 < 0)
        size2 = 0;
    if (!buf3 || size3 < 0)
        size3 = 0;
    if (!buf4 || size4 < 0)
        size4 = 0;

    ptr = buf = 
        (char *)malloc(size1 + size2 + size3 + size4 + 9 * ALIGN_PADDING);
    memset(buf, FILL_8, size1 + size2 + size3 + size4 + 9 * ALIGN_PADDING);

    ptr = align_up(ptr, ALIGN_PADDING);
    if (buf1)
    {
        *buf1 = ptr + (0xAAAAAAAA & antialias_pattern_mask);
        ptr = align_up(*buf1 + size1, ALIGN_PADDING);
    }
    if (buf2)
    {
        *buf2 = ptr + (0x55555555 & antialias_pattern_mask);
        ptr = align_up(*buf2 + size2, ALIGN_PADDING);
    }
    if (buf3)
    {
        *buf3 = ptr + (0xCCCCCCCC & antialias_pattern_mask);
        ptr = align_up(*buf3 + size3, ALIGN_PADDING);
    }
    if (buf4)
    {
        *buf4 = ptr + (0x33333333 & antialias_pattern_mask);
    }

    return buf;
}

void print_help_and_exit(int minrepeats, int maxrepeats, int mem_realloc,
                int latbench_repeats, int latbench_count)
{
    printf("Usage: tinymembench [-h] [-b n] [-B n] [-r | -R] [-l n] [-c n]\n");
    printf("\n");
    printf(" -h    Print help and exit\n");
    printf(" -b n  Bandwith tests' min repeats (default: %d)\n", minrepeats);
    printf(" -B n  Bandwith tests' max repeats (default: %d)\n", maxrepeats);
    printf(" -m    Reallocate memory in bandwidth tests%s\n", mem_realloc ? " (default)" : "");
    printf(" -M    Don't reallocate memory in bandwith tests%s\n", mem_realloc ? "" : " (default)");
    printf(" -l n  Latency tests' repeats (default: %d)\n", latbench_repeats);
    printf(" -c n  Latency tests' count (default: %d)\n", latbench_count);
    printf("\n");
    printf("Bandwith tests' min repeats is capped to max repeats.\n");
    printf("Max value for n in %d, but you probably don't want to\n", INT_MAX);
    printf("use it unless you have an unlimited supply of coffee.\n");
    exit(0);
}

static int parse_pos_int(char *str) {
    long v;
    errno = 0;
    v = strtol(str, NULL, 0);
    if (errno != 0 || v <= 0 || v > INT_MAX)
    {
        return -1;
    }
    return v;
}

void parse_args(int argc, char **argv,
                int *minrepeats, int *maxrepeats, int *mem_realloc,
                int *latbench_repeats, int *latbench_count)
{
    int minrepeats_def = *minrepeats;
    int maxrepeats_def = *maxrepeats;
    int mem_realloc_def = *mem_realloc;
    int latbench_repeats_def = *latbench_repeats;
    int latbench_count_def = *latbench_count;
    int c;
    while ((c = getopt(argc, argv, "hmMb:B:l:c:")) != -1) {
        switch (c) {
        case 'h':
            print_help_and_exit(minrepeats_def, maxrepeats_def, mem_realloc_def,
                                latbench_repeats_def, latbench_count_def);
            break;
        case 'm':
            *mem_realloc = 1;
            break;
        case 'M':
            *mem_realloc = 0;
            break;
        case 'b':
            c = parse_pos_int(optarg);
            if (c <= 0)
            {
                printf("Invalid bandwith tests min repeats arg: %s\n", optarg);
                print_help_and_exit(minrepeats_def, maxrepeats_def, mem_realloc_def,
                                    latbench_repeats_def, latbench_count_def);
            }
            *minrepeats = c;
            break;
        case 'B':
            c = parse_pos_int(optarg);
            if (c <= 0)
            {
                printf("Invalid bandwith tests max repeats arg: %s\n", optarg);
                print_help_and_exit(minrepeats_def, maxrepeats_def, mem_realloc_def,
                                    latbench_repeats_def, latbench_count_def);
            }
            *maxrepeats = c;
            break;
        case 'l':
            c = parse_pos_int(optarg);
            if (c <= 0)
            {
                printf("Invalid latency tests repeats arg: %s\n", optarg);
                print_help_and_exit(minrepeats_def, maxrepeats_def, mem_realloc_def,
                                    latbench_repeats_def, latbench_count_def);
            }
            *latbench_repeats = c;
            break;
        case 'c':
            c = parse_pos_int(optarg);
            if (c <= 0)
            {
                printf("Invalid latency tests count arg: %s\n", optarg);
                print_help_and_exit(minrepeats_def, maxrepeats_def, mem_realloc_def,
                                    latbench_repeats_def, latbench_count_def);
            }
            *latbench_count = c;
            break;
        default:
            print_help_and_exit(minrepeats_def, maxrepeats_def, mem_realloc_def,
                                latbench_repeats_def, latbench_count_def);
            break;
        }
    }
}
