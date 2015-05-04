//
//  MathUtils.cpp
//  Relief2
//
//  Created by Daniel Windham on 5/03/15.
//
//

#include "MathUtils.h"


// calculate the factorial n!. for efficiency, optionally pass k to calculate n! / k!
double factorial(int n, int k) {
    double result = 1;
    if (k < 1) {
        k = 1;
    }

    k++;
    for (; k <= n; k++) {
        result *= k;
    }
    return result;
}

// calculate the binomial coefficient n choose k
double binomialCoefficient(int n, int k) {
    if (k < 1 || k >= n) {
        return 1;
    }

    int _k = n - k;
    int kLow = min(k, _k);
    int kHigh = max(k, _k);

    // n! / (k! (n-k)!)
    // optimize by using n! / k! feature of factorial function
    return factorial(n, kHigh) / kLow;
}