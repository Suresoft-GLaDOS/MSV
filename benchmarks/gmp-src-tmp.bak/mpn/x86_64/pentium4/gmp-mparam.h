/* Pentium 4-64 gmp-mparam.h -- Compiler/machine parameter header file.

Copyright 1991, 1993, 1994, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,
2008, 2009, 2010 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */

#define GMP_LIMB_BITS 64
#define BYTES_PER_MP_LIMB 8

/* These routines exists for all x86_64 chips, but they are slower on Pentium4
   than separate add/sub and shift.  Make sure they are not really used.  */
#undef HAVE_NATIVE_mpn_rsblsh1_n
#undef HAVE_NATIVE_mpn_rsblsh2_n
#undef HAVE_NATIVE_mpn_addlsh_n
#undef HAVE_NATIVE_mpn_rsblsh_n

/* 3400 MHz Pentium / 1024 Kibyte cache */

#define MOD_1_NORM_THRESHOLD                 0  /* always */
#define MOD_1_UNNORM_THRESHOLD               0  /* always */
#define MOD_1N_TO_MOD_1_1_THRESHOLD          4
#define MOD_1U_TO_MOD_1_1_THRESHOLD          3
#define MOD_1_1_TO_MOD_1_2_THRESHOLD        14
#define MOD_1_2_TO_MOD_1_4_THRESHOLD        32
#define PREINV_MOD_1_TO_MOD_1_THRESHOLD      7
#define USE_PREINV_DIVREM_1                  1  /* native */
#define DIVEXACT_1_THRESHOLD                 0  /* always (native) */
#define BMOD_1_TO_MOD_1_THRESHOLD           20

#define MUL_TOOM22_THRESHOLD                12
#define MUL_TOOM33_THRESHOLD                66
#define MUL_TOOM44_THRESHOLD               118
#define MUL_TOOM6H_THRESHOLD               157
#define MUL_TOOM8H_THRESHOLD               242

#define MUL_TOOM32_TO_TOOM43_THRESHOLD      81
#define MUL_TOOM32_TO_TOOM53_THRESHOLD      91
#define MUL_TOOM42_TO_TOOM53_THRESHOLD      81
#define MUL_TOOM42_TO_TOOM63_THRESHOLD      80

#define SQR_BASECASE_THRESHOLD               0  /* always (native) */
#define SQR_TOOM2_THRESHOLD                 20
#define SQR_TOOM3_THRESHOLD                 77
#define SQR_TOOM4_THRESHOLD                214
#define SQR_TOOM6_THRESHOLD                254
#define SQR_TOOM8_THRESHOLD                454

#define MULMOD_BNM1_THRESHOLD               10
#define SQRMOD_BNM1_THRESHOLD               11

#define MUL_FFT_MODF_THRESHOLD             236  /* k = 5 */
#define MUL_FFT_TABLE3                                      \
  { {    236, 5}, {      9, 4}, {     19, 5}, {     13, 6}, \
    {      9, 5}, {     19, 6}, {     13, 7}, {      7, 6}, \
    {     15, 7}, {      8, 6}, {     17, 7}, {      9, 6}, \
    {     19, 7}, {     11, 6}, {     23, 7}, {     13, 8}, \
    {      7, 7}, {     17, 8}, {      9, 7}, {     21, 8}, \
    {     11, 7}, {     23, 8}, {     13, 9}, {      7, 8}, \
    {     21, 9}, {     11, 8}, {     25,10}, {      7, 9}, \
    {     15, 8}, {     33, 9}, {     19, 8}, {     39, 9}, \
    {     23, 8}, {     47, 9}, {     27,10}, {     15, 9}, \
    {     39,10}, {     23, 9}, {     51,11}, {     15,10}, \
    {     31, 9}, {     67,10}, {     39, 9}, {     79,10}, \
    {     47, 9}, {     95,10}, {     55,11}, {     31,10}, \
    {     79,11}, {     47, 9}, {    191,12}, {     31,11}, \
    {     63,10}, {    127, 9}, {    255,10}, {    143, 9}, \
    {    287,11}, {     79,10}, {    159, 9}, {    319,10}, \
    {    175, 9}, {    351,11}, {     95,10}, {    191, 9}, \
    {    383,10}, {    223,12}, {     63,11}, {    127,10}, \
    {    255,11}, {    143,10}, {    287, 9}, {    575,10}, \
    {    303,11}, {    159,10}, {    319,11}, {    175,12}, \
    {     95,11}, {    191,10}, {    383,11}, {    223,13}, \
    {   8192,14}, {  16384,15}, {  32768,16}, {  65536,17}, \
    { 131072,18}, { 262144,19}, { 524288,20}, {1048576,21}, \
    {2097152,22}, {4194304,23}, {8388608,24} }
#define MUL_FFT_TABLE3_SIZE 91
#define MUL_FFT_THRESHOLD                 2752

#define SQR_FFT_MODF_THRESHOLD             216  /* k = 5 */
#define SQR_FFT_TABLE3                                      \
  { {    216, 5}, {     13, 6}, {      9, 5}, {     19, 6}, \
    {     15, 7}, {      8, 6}, {     17, 7}, {      9, 6}, \
    {     19, 7}, {     10, 6}, {     21, 7}, {     17, 8}, \
    {      9, 7}, {     21, 8}, {     11, 7}, {     24, 8}, \
    {     13, 9}, {      7, 8}, {     15, 7}, {     31, 8}, \
    {     21, 9}, {     11, 8}, {     27,10}, {      7, 9}, \
    {     15, 8}, {     33, 9}, {     19, 8}, {     39, 9}, \
    {     23, 8}, {     47, 9}, {     27,10}, {     15, 9}, \
    {     39,10}, {     23, 9}, {     47,11}, {     15,10}, \
    {     31, 9}, {     63, 8}, {    127,10}, {     39, 9}, \
    {     79,10}, {     55,11}, {     31,10}, {     63, 9}, \
    {    127, 8}, {    255,10}, {     71, 9}, {    143, 8}, \
    {    287,10}, {     79,11}, {     47,10}, {     95, 9}, \
    {    191,12}, {     31,11}, {     63, 9}, {    255,10}, \
    {    143, 9}, {    287,11}, {     79,10}, {    159, 9}, \
    {    319,10}, {    175, 9}, {    351,11}, {     95,10}, \
    {    191, 9}, {    383,10}, {    207,11}, {    111,12}, \
    {     63,11}, {    127,10}, {    255,11}, {    143,10}, \
    {    287,11}, {    159,10}, {    319, 9}, {    639,11}, \
    {    175,10}, {    351,12}, {     95,11}, {    207,10}, \
    {    415,11}, {    223,13}, {   8192,14}, {  16384,15}, \
    {  32768,16}, {  65536,17}, { 131072,18}, { 262144,19}, \
    { 524288,20}, {1048576,21}, {2097152,22}, {4194304,23}, \
    {8388608,24} }
#define SQR_FFT_TABLE3_SIZE 93
#define SQR_FFT_THRESHOLD                 2112

#define MULLO_BASECASE_THRESHOLD             0  /* always */
#define MULLO_DC_THRESHOLD                  32
#define MULLO_MUL_N_THRESHOLD             5397

#define DC_DIV_QR_THRESHOLD                 28
#define DC_DIVAPPR_Q_THRESHOLD              67
#define DC_BDIV_QR_THRESHOLD                27
#define DC_BDIV_Q_THRESHOLD                 49

#define INV_MULMOD_BNM1_THRESHOLD           22
#define INV_NEWTON_THRESHOLD               202
#define INV_APPR_THRESHOLD                 101

#define BINV_NEWTON_THRESHOLD              199
#define REDC_1_TO_REDC_2_THRESHOLD          13
#define REDC_2_TO_REDC_N_THRESHOLD          44

#define MU_DIV_QR_THRESHOLD                979
#define MU_DIVAPPR_Q_THRESHOLD             979
#define MUPI_DIV_QR_THRESHOLD               98
#define MU_BDIV_QR_THRESHOLD               807
#define MU_BDIV_Q_THRESHOLD                979

#define MATRIX22_STRASSEN_THRESHOLD         17
#define HGCD_THRESHOLD                     101
#define GCD_DC_THRESHOLD                   222
#define GCDEXT_DC_THRESHOLD                222
#define JACOBI_BASE_METHOD                   4

#define GET_STR_DC_THRESHOLD                12
#define GET_STR_PRECOMPUTE_THRESHOLD        26
#define SET_STR_DC_THRESHOLD               248
#define SET_STR_PRECOMPUTE_THRESHOLD      1438
