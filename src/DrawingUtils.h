//
//  DrawingUtils.h
//  Relief2
//
//  Created by Daniel Windham on 4/30/15.
//
//

#include "ofMain.h"
#include "Constants.h"
#include "Utils.h"


#ifndef __Relief2__DrawingUtils__
#define __Relief2__DrawingUtils__


void gradientQuadrilateral(ofPoint &p0, ofPoint &p1, ofPoint &p2, ofPoint &p3, ofColor &color0, ofColor &color1);
void gradientQuadrilateral(ofPoint &p0, ofPoint &p1, ofPoint &p2, ofPoint &p3, int color0, int color1);
void verticalLinearGradientRect(Rectangle &rect, ofColor &colorTop, ofColor &colorBottom);
void verticalLinearGradientRect(Rectangle &rect, int colorTop, int colorBottom);

#endif /* defined(__Relief2__DrawingUtils__) */