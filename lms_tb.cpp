// TESTBENCH for LMS filter
//Trying to show the adaptive noise cancellation

#include "lms_filter.h"
#include <iostream>
#include <cmath>
#include <iomanip>

#define TEST_LENGTH 2000

int main() {
    data_t y;
    error_t error;
    int enable_adapt;
    
    std::cout << "=== LMS Adaptive Filter Testbench ===" << std::endl;
    std::cout << "Filter Order: " << N << std::endl;
    std::cout << "Step Size (mu): 1/" << (1 << MU_SHIFT) << std::endl << std::endl;
    
    // Test scenario: Adaptive noise cancellation
    // Signal: sine wave + noise
    // Reference: correlated noise
    // Goal: LMS adapts to cancel the noise component
    
    std::cout << "Sample\tInput\tDesired\tOutput\t\tError\t\tMSE" << std::endl;
    std::cout << "------\t-----\t-------\t------\t\t-----\t\t---" << std::endl;
    
    double mse = 0.0;
    double mse_avg = 0.0;
    
    for (int n = 0; n < TEST_LENGTH; n++) {
        // Generate test signals
        double t = n * 0.1;
        
        // Clean signal (sine wave)
        double signal = sin(t);
        
        // Noise (another sine wave with diff frequency)
        double noise = 0.5 * sin(3.7 * t + 0.5);
        
        // Input: signal + noise
        data_t x = signal + noise;
        
        // Desired output: just the clean signal
        data_t d = signal;
        
        // Enable adaptation after initial samples
        enable_adapt = (n > 10) ? 1 : 0;
        
        // Call LMS filter
        lms_filter(&y, x, d, &error, enable_adapt);
        
        // Compute MSE (Mean Squared Error)
        double err_val = (double)error;
        mse = err_val * err_val;
        mse_avg = 0.95 * mse_avg + 0.05 * mse;  // Exponential moving average
        
        // Print results every 50 samples
        if (n % 50 == 0 || n < 10) {
            std::cout << std::setw(6) << n << "\t";
            std::cout << std::fixed << std::setprecision(4);
            std::cout << std::setw(6) << (double)x << "\t";
            std::cout << std::setw(7) << (double)d << "\t";
            std::cout << std::setw(6) << (double)y << "\t\t";
            std::cout << std::setw(7) << (double)error << "\t";
            std::cout << std::setw(8) << mse_avg << std::endl;
        }
    }
    
    std::cout << "\n=== Test Complete ===" << std::endl;
    std::cout << "Final MSE: " << std::fixed << std::setprecision(6) << mse_avg << std::endl;
    
    if (mse_avg < 0.1) {
        std::cout << "PASS: Filter successfully adapted!" << std::endl;
        return 0;
    } else {
        std::cout << "FAIL: Filter did not converge adequately." << std::endl;
        return 1;
    }
}