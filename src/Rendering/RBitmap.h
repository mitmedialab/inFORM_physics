//
//  RRectangle.h
//  Relief2
//
//  Created by Daniel Leithinger on 3/18/13.
//
//

#ifndef __Relief2__RBitmap__
#define __Relief2__RBitmap__

#include "RenderableObject.h"
#include <iostream>

class RBitmap : public RenderableObject {
public:
    ofxAnimatableFloat imageBlendAnim;
    ofImage blendImage1;
    ofImage blendImage2;
    ofImage image;
    
    
    bool blendImage1loaded;
    bool blendImage2loaded;
    
    RBitmap();
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);
    void setImages(ofImage image1, ofImage image2, ofxAnimatableFloat imageBlend);
    void setImages(string imagePath1, string imagePath2, ofxAnimatableFloat imageBlend);
};

#endif /* defined(__Relief2__RBitmap__) */
