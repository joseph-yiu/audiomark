/*
 * Copyright (C) EEMBC(R). All Rights Reserved
 *
 * All EEMBC Benchmark Software are products of EEMBC and are provided under the
 * terms of the EEMBC Benchmark License Agreements. The EEMBC Benchmark Software
 * are proprietary intellectual properties of EEMBC and its Members and is
 * protected under all applicable laws, including all applicable copyright laws.
 *
 * If you received this EEMBC Benchmark Software without having a currently
 * effective EEMBC Benchmark License Agreement, you must discontinue use.
 */
/*
 * Copyright (C) 2022 Arm Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __EE_ABF_H
#define __EE_ABF_H

#include "ee_types.h"
#include "ee_api.h"
#include "public.h"

#define NFFT    128
#define NFFTD2  (NFFT / 2)
#define LAGSTEP 16
/* fixed direction beamformer from [0.. LAGSTEP-1] <=> [-180deg .. +180deg] */
#define FIXED_DIRECTION 0
#define REAL            1
#define COMPLEX         2
#define LEN_BM_ADF      6

typedef struct
{
    ee_f32_t alpha_BM_NLMS;
    ee_f32_t DS_DET_TH;
    ee_f32_t ep_GSC;
} ee_abf_f32_params_t;

typedef struct
{
    ee_f32_t states_BM_ADF[NFFT / 2 + 1][LEN_BM_ADF * 2];
    ee_f32_t coefs_BM_ADF[NFFT / 2 + 1][LEN_BM_ADF * 2];
    ee_f32_t Norm_out_BM[NFFT / 2 + 1];
    ee_f32_t lookBF_out[NFFT / 2 + 1];
    ee_f32_t GSC_det_avg;
    uint8_t  adptBF_coefs_update_enable;
} ee_abf_f32_mem_t;

/* Fast coefficient structure */
typedef struct
{
    float         old_left[NFFTD2 * COMPLEX];
    float         old_right[NFFTD2 * COMPLEX];
    float         ola_new[NFFTD2];
    ee_rfft_f32_t rS;
    ee_cfft_f32_t cS;
} ee_abf_f32_static_t;

/* "working" / scratch area */
typedef struct
{
    float    mic[NFFT * COMPLEX + 2];
    float    BM[NFFT * COMPLEX + 2];
    float    BF[NFFT * COMPLEX + 2];
    float    X0[NFFT * COMPLEX + 2];
    float    Y0[NFFT * COMPLEX + 2];
    float    CY0[NFFTD2 * COMPLEX + 2];
    float    XY[NFFTD2 * COMPLEX];
    float    PHATNORM[NFFTD2 * COMPLEX];
    float    allDerot[LAGSTEP];
    float    corr;
    uint32_t icorr;
} ee_abf_f32_working_t;

typedef struct
{
    float                *wrot;
    float                *window;
    ee_abf_f32_static_t  *st; /* fast coefficient memory area */
    ee_abf_f32_working_t *w;  /* fast working memory area */
} beamformer_f32_instance;

int32_t ee_abf_f32(int32_t command,
                   void  **instance,
                   void   *data,
                   void   *parameters);

#endif
