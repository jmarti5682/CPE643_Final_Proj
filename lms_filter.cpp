/*
Author(s): Jose Martinez-Ponce, Malik Tragna
Date: 12/12/2025
Purpose: An Adaptive FIR filter using the Least-Mean-Squares (LMS) algorithm. Based off of Lab 3 source code. 
==============================
lms_filer.cpp

LMS Algorithm:
1. Filter Output: y[n] = sum(w[i] * x[n-i])
2. Error: e[n] = d[n] - y[n]
3. Weight update: w[i] = w[i] + mu * e[n] * x[n-i]

==============================
*/

#include "lms_filter.h"

void lms_filter(
    data_t *y,
    data_t x,
    data_t d,
    error_t *error_out,
    int enable_adapt)
{

#pragma HLS INTERFACE mode=s_axilite port=y bundle=lms_io
#pragma HLS INTERFACE mode=s_axilite port=x bundle=lms_io
#pragma HLS INTERFACE mode=s_axilite port=d bundle=lms_io
#pragma HLS INTERFACE mode=s_axilite port=error_out bundle=lms_io
#pragma HLS INTERFACE mode=s_axilite port=enable_adapt bundle=lms_io
#pragma HLS INTERFACE mode=s_axilite port=return bundle=lms_io

    //static vars b/w function calls
    static coef_t weights[N+1];
    static data_t shift_reg[N+1];
    static int initialized = 0;

    #pragma HLS ARRAY_PARTITION variable=weights complete dim=1
    #pragma HLS ARRAY_PARTITION variable=shift_reg complete dim=1

    acc_t acc;
    error_t error;
    int i;

    //set weights to zero on first run
    if(!initialized)
    {
    init_loop:
        for(i = 0; i <= N; i++)
        {
            #pragma HLS UNROLL
            weights[i] = 0;
            shift_reg[i] = 0;
        }
        initialized = 1;
    }

    //=== FILTERING STAGE ===
    //y[n] = sum(w[i] * x[n-i])
    acc = 0;

    filter_loop:
    for(i = 0; i <= N; i++)
    {
        #pragma HLS UNROLL factor=3
        #pragma HLS PIPELINE II=1 //pipeline the multiply-accumlate op
        acc += (acc_t)weights[i] * (acc_t)shift_reg[i];
    }

    *y = (data_t)acc;


    //=== ERROR CALC ===
    // e[n] = d[n] - y[n]

    error = (error_t)d - (error_t)acc;
    *error_out = error;

    // === ADAPTATION ===
    // w[i] = + mu * e[n] * x[n-i]
    // mu = 1/(2^MU_SHIFT)

    if(enable_adapt)
    {
        adapt_loop:
        for(i = 0; i <= N; i++)
        {
            #pragma HLS UNROLL factor=3
            #pragma HLS PIPELINE II=1

            acc_t weight_update = ((acc_t)error * (acc_t)shift_reg[i]) >> MU_SHIFT;

            //update weight
            weights[i] = weights[i] + (coef_t)weight_update;
        }
    }

    // === SHIFT REG UPDATE ===
    // x[n-i] = x[n-i-1]

    shift_loop:
    for(i = N; i > 0; i--)
    {
        #pragma HLS UNROLL
        shift_reg[i] = shift_reg[i-1];
    }
    shift_reg[0] = x;


}