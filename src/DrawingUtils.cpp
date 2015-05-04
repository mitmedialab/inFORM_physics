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