//
//  DrawingUtils.cpp
//  Relief2
//
//  Created by Daniel Windham on 4/30/15.
//
//

#include "DrawingUtils.h"


// gradientQuadrilateral
//
// draw a quadrilateral with a linear gradient stretching from the first two points to the last two points
void gradientQuadrilateral(ofPoint &p0, ofPoint &p1, ofPoint &p2, ofPoint &p3, ofColor &color0, ofColor &color1) {
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    mesh.enableColors();
    mesh.addVertex(p0);
    mesh.addColor(color0);
    mesh.addVertex(p1);
    mesh.addColor(color0);
    mesh.addVertex(p2);
    mesh.addColor(color1);
    mesh.addVertex(p3);
    mesh.addColor(color1);
    mesh.draw();
}

// gradientQuadrilateral
//
// ~ alternative calling pattern
void gradientQuadrilateral(ofPoint &p0, ofPoint &p1, ofPoint &p2, ofPoint &p3, int color0, int color1) {
    ofColor _color0(color0);
    ofColor _color1(color1);
    gradientQuadrilateral(p0, p1, p2, p3, _color0, _color1);
}

// verticalLinearGradientRect
//
// draw a rectangle with a linear gradient stretching from top to bottom
void verticalLinearGradientRect(Rectangle &rect, ofColor &colorTop, ofColor &colorBottom) {
    ofPoint p0(rect.left, rect.top);
    ofPoint p1(rect.left + rect.width, rect.top);
    ofPoint p2(rect.left + rect.width, rect.top + rect.height);
    ofPoint p3(rect.left, rect.top + rect.height);
    gradientQuadrilateral(p0, p1, p2, p3, colorTop, colorBottom);
}

// verticalLinearGradientRect
//
// ~ alternative calling pattern
void verticalLinearGradientRect(Rectangle &rect, int colorTop, int colorBottom) {
    ofColor _colorTop(colorTop);
    ofColor _colorBottom(colorBottom);
    verticalLinearGradientRect(rect, _colorTop, _colorBottom);
}

// interpolateBezierControlPoints
//
// Generate interpolated points based on bezier control points. Use with float
// and ofFloatColor types, or any other types that can be set to 0.0 and support
// addition and multiplication by floats.
//
// Note: only use with types that play well with floating point arithmetic;
// other types will yield nonsensical results.
template<typename FloatType>
void interpolateBezierControlPoints(vector<pair<float, FloatType> > &controlPoints, vector<pair<float, FloatType> > &interpolatedPoints, float stepSize) {
    interpolatedPoints.clear();
    int n = controlPoints.size();

    // for each increment in the range [0, 1), calculate the interpolated point
    for (float t = 0; t < 1; t += stepSize) {
        float tIndex = 0.0;
        FloatType tValue = 0.0;
        float tCurrentPower = pow(1 - t, n - 1);
        float tStepChange = t / (1 - t);
        for (int i = 0; i < n; i++) {
            // at each step i, add control point i scaled by t^i * (1-t)^(n-i) * (n-1 choose i)
            // for efficiency, calculate scalar i by iteratively multiplying by t / (1-t)
            tIndex += controlPoints[i].first * tCurrentPower * binomialCoefficient(n - 1, i);
            tValue += controlPoints[i].second * tCurrentPower * binomialCoefficient(n - 1, i);
            tCurrentPower *= tStepChange;
        }
        interpolatedPoints.push_back(pair<float, FloatType>(tIndex, tValue));
    }

    // last, push the interpolated point for t == 1
    interpolatedPoints.push_back(controlPoints[n - 1]);
}

// verticalBezierGradientRect
//
// draw a rectangle with a bezier curve gradient stretching from top to bottom
void verticalBezierGradientRect(Rectangle &rect, vector<pair<float, float> > &controlPoints, float stepSize) {
    vector<pair<float, float> > interpolatedPoints;
    interpolateBezierControlPoints(controlPoints, interpolatedPoints, stepSize);
    int n = interpolatedPoints.size();

    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    mesh.enableColors();

    for (int i = 0; i < n; i++) {
        ofColor color(min(255, max(0, (int) interpolatedPoints[i].second)));
        mesh.addVertex(ofPoint(rect.left, rect.top + rect.height * interpolatedPoints[i].first));
        mesh.addColor(ofColor(color));
        mesh.addVertex(ofPoint(rect.left + rect.width, rect.top + rect.height * interpolatedPoints[i].first));
        mesh.addColor(ofColor(color));
    }

    mesh.draw();
}

// verticalBezierInterpolatedGradientRect
//
// basic bezier curve interpolator through three given points. this does not guarantee
// preservation of interpolation point extrema in the generated bezier curve (unless
// the passed in bezier step parameter q guarantees preservation).
//
// See verticalBezierSmartInterpolatedGradientRect for stronger guarantees.
//
// Note: currently, only the case with three interpolation points has been implemented.
void verticalBezierInterpolatedGradientRect(Rectangle &rect, vector<pair<float, float> > &interpolationPoints, float stepSize, float q) {
    if (interpolationPoints.size() != 3) {
        cout << "ERROR: verticalBezierInterpolatedGradientRect expects interpolationPoints parameter to have length 3. More general behavior has not yet been implemented." << endl;
    }

    // controlPoint[0] and [2] are the same as interpolationPoint[0] and [2]. We just
    // need to find controlPoint[1] such that interpolationPoint[1] comes out correctly.

    // the bezier step at the middle interpolation point is t = q satisfying 0 < q < 1.
    // q can be optionally specified in the call arguments.

    // if the passed argument q is below 0 (the default case), use q = 0.5
    if (q <= 0) {
        q = 0.5;
    // if the passed argument q is above 1, use q = ipIndex[1]
    } else if (q >= 1) {
        q = interpolationPoints[1].first;
    }
    float _q = 1 - q;

    // Find cpIndex[1] such that
    //   ipIndex[1] = cpIndex[0] * _q * _q + 2 * cpIndex[1] * _q * q + cpIndex[2] * q * q
    //              = ipIndex[0] * _q * _q + 2 * cpIndex[1] * _q * q + ipIndex[2] * q * q
    float cpIndex1 = (interpolationPoints[1].first - interpolationPoints[0].first * _q * _q - interpolationPoints[2].first * q * q) / (2 * _q * q);

    // Find cpValue[1] such that
    //   ipValue[1] = cpValue[0] * _q * _q + 2 * cpValue[1] * _q * q + cpValue[2] * q * q
    //              = ipValue[0] * _q * _q + 2 * cpValue[1] * _q * q + ipValue[2] * q * q
    float cpValue1 = (interpolationPoints[1].second - interpolationPoints[0].second * _q * _q - interpolationPoints[2].second * q * q) / (2 * _q * q);

    // populate controlPoints vector
    vector<pair<float, float> > controlPoints;
    controlPoints.push_back(interpolationPoints[0]);
    controlPoints.push_back(pair<float, float>(cpIndex1, cpValue1));
    controlPoints.push_back(interpolationPoints[2]);

    // draw rectangle using control points
    verticalBezierGradientRect(rect, controlPoints, stepSize);
}