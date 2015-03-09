//
//  RRectangle.cpp
//  Relief2
//
//  Created by Daniel Leithinger on 3/18/13.
//
//

#include "RBitmap.h"
#include "Constants.h"

RBitmap::RBitmap() {
    blendImage1loaded = false;
    blendImage2loaded = false;
}

void RBitmap::drawHeightMap() {
    if(render) {
    ofFill();
    ofSetColor(255);
    if(blendImage1loaded)
        blendImage1.draw(position.getCurrentPosition(), size.getCurrentPosition().x, size.getCurrentPosition().y);
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,(int)(imageBlendAnim.getCurrentValue() * 255));
    if(blendImage2loaded)
        blendImage2.draw(position.getCurrentPosition(), size.getCurrentPosition().x, size.getCurrentPosition().y);
    ofDisableAlphaBlending();
    }
}

void RBitmap::drawGraphics() {
    if (render) {
    ofFill();
    ofSetColor(200);
    if(blendImage1loaded)
        blendImage1.draw(position.getCurrentPosition(), size.getCurrentPosition().x, size.getCurrentPosition().y);
    ofEnableAlphaBlending();
    ofSetColor(200,200,200,(int)(imageBlendAnim.getCurrentValue()*255));
    if(blendImage2loaded)
        blendImage2.draw(position.getCurrentPosition(), size.getCurrentPosition().x, size.getCurrentPosition().y);
    ofDisableAlphaBlending();
    }
}

void RBitmap::update(float dt) {
    position.update(dt);
    size.update(dt);
    imageBlendAnim.update(dt);
}

void RBitmap::setImages(ofImage image1, ofImage image2, ofxAnimatableFloat imageBlend) {
    blendImage1 = image1;
    blendImage2 = image2;
    this->imageBlendAnim = imageBlend;
}

void RBitmap::setImages(string imagePath1, string imagePath2, ofxAnimatableFloat imageBlend) {
    blendImage1loaded = blendImage1.loadImage(imagePath1);
    blendImage2loaded = blendImage2.loadImage(imagePath2);
    this->imageBlendAnim = imageBlend;
}