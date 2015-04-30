//
//  DrawingUtils.cpp
//  Relief2
//
//  Created by Daniel Windham on 4/30/15.
//
//

#include "DrawingUtils.h"


// gradientQuadrilateral: draw a quadrilateral with a linear gradient stretching from the first two points to the last two points
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

// gradientQuadrilateral: alternative calling pattern
void gradientQuadrilateral(ofPoint &p0, ofPoint &p1, ofPoint &p2, ofPoint &p3, int color0, int color1) {
    ofColor _color0(color0);
    ofColor _color1(color1);
    gradientQuadrilateral(p0, p1, p2, p3, _color0, _color1);
}

// verticalGradientRect: draw a rectangle with a linear gradient stretching from top to bottom
void verticalGradientRect(Rectangle &rect, ofColor &colorTop, ofColor &colorBottom) {
    ofPoint p0(rect.left, rect.top);
    ofPoint p1(rect.left + rect.width, rect.top);
    ofPoint p2(rect.left + rect.width, rect.top + rect.height);
    ofPoint p3(rect.left, rect.top + rect.height);
    gradientQuadrilateral(p0, p1, p2, p3, colorTop, colorBottom);
}

// verticalGradientRect: alternative calling pattern
void verticalGradientRect(Rectangle &rect, int colorTop, int colorBottom) {
    ofColor _colorTop(colorTop);
    ofColor _colorBottom(colorBottom);
    verticalGradientRect(rect, _colorTop, _colorBottom);
}