//
//  MathUtils.h
//  Relief2
//
//  Created by Daniel Windham on 5/03/15.
//
//

#include "ofMain.h"


#ifndef __Relief2__MathUtils__
#define __Relief2__MathUtils__


#define MATH_EPSILON 1e-9


double factorial(int n, int k=1);
double binomialCoefficient(int n, int k);

// polynomial root finders (polynomials up to degree 4) by Jochen Scharze
// originally found at http://tog.acm.org/resources/GraphicsGems/gemsiv/vec_mat/ray/solver.h
int isZero(double x);
int solveQuadratic(double c[3], double s[2]);
int solveCubic(double c[4], double s[3]);
int solveQuartic(double c[5], double s[4]);

#endif /* defined(__Relief2__MathUtils__) */