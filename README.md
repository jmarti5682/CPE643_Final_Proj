# CPE643 Final Project 
### Group: Jose Martinez-Ponce, Malik Tragna

## Initial Submission

**Date: 12/12/2025**

This is currently we have with our final project. We are currently implementation the adaptive LMS filter. We have also created a test bench for verifying convergence and error performance.

More of the math can be found at this website:  
https://www.geeksforgeeks.org/deep-learning/least-mean-squares-algorithm-in-neural-networks/

We basically are trying to implement this and see if it actually works. We ran into a couple of issues with our MSE being just above 0.1, which results in a failure. However, after changing around some parameters, mainly the TEST_LENGTH and the MU_SHIFT we were able to achieve a pass on our simulation. 

Functionally, this algorithm is useful for applications where systems and devices may need to change a varying signal in real time. For example, noise cancellation requires that headphones are capable of cancelling signals that are not of interest while maintaining the signals of interest. The LMS algorithm continuously changes the coefficients of the corresponding filter to ensure that these changing signals are handled correctly. 

Additionally, using an FPGA implementation allows for us to optimize the hardware implementation to the algorithm to allow for low latency computation. This is especially applicable for real time applications since latency is of extreme relevance for noise cancellation. We still are looking to synthesize our specific implementation on the PYNQ board and will see if we can continue to optimize the hardware implementation.
