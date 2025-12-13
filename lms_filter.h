// MIT License
// Copyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// SOFTWARE.

//////////////////////////////////////////////////////////////////////////////////
// Purpose:
//    This is the header for lms_filter.cpp
//////////////////////////////////////////////////////////////////////////////////
#ifndef LMS_FILTER_H
#define LMS_FILTER_H

#include "ap_int.h"
#include "ap_fixed.h"

#define N 10 // num of taps
#define MU_SHIFT 8 //shift for mu step size --> mu = 1/(2^MU_SHIFT)

//type defs
typedef ap_fixed<16,2> data_t;  //IO data:          16-bit fixed, 2-bit int
typedef ap_fixed<32,8> coef_t;  //Coefficients:     32-bit fixed, 8-bit int
typedef ap_fixed<48,16> acc_t;  //Accmulator:       48-bit fixed, 16-bit int
typedef ap_fixed<32,8> error_t; //Error signal:     32-bit fixed, 8-bit int


// LMS filter
void lms_filter(
    data_t *y,              //filter output
    data_t x,               //Current input sample
    data_t d,               //Desired output
    error_t *error_out,     //Error signal output
    int enable_adapt        //enable/disable coef. adaptation
);

#endif
