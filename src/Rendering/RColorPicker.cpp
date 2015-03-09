//
//  RColorPicker.cpp
//  Relief2
//
//  Created by TMG on 10/1/13.
//
//

#include <math.h>

#include "RColorPicker.h"

RColorPicker::RColorPicker() {
    setup();
}

void RColorPicker::setup()
{
    loadShader();
    
    regButton = ofRectangle(3 * 30,24 * 30,5 * 30,5 * 30);
    regPicker = ofRectangle(3 * 30,4 * 30,24 * 30,22 * 30);
    
    // Control points for button
    {
        const float low = 30;
        const float high = 90;
        for(int i = -5;i <= 5; i++) {
            for(int j = -5;j <= 5; j++) {
                int index = (i+5) * 11 + (j+5);
                if (i*i + j*j > 25)
                    geomButton[i+5][j+5] = ofVec3f(i,j,0);
                else
                {
                    float z = sqrt(25.0f - (i*i+j*j));
                    float height = (z / 5.0f) * (high - low) + low;
                    geomButton[i+5][j+5] = ofVec3f(i,j,height);
                }
            }
        }
    }

    // Control points for picker
    {
        const float low = 50;
        const float high = 255;
        for(int i = -5;i <= 5; i++) {
            for(int j = -5;j <= 5; j++) {
                float height = ((5.0f - j) / 10.0f) * (high - low) + low;
                geomPicker[i+5][j+5] = ofVec3f(i,j,height);
            }
        }
    }
    
    imgButton1.loadImage("picker_before.png");
    imgButton2.loadImage("picker_after.png");
    imgPicker.loadImage("picker.png");
    
    anim.pause();
    currColor = false;
    nextDirection = true;
}

void RColorPicker::update(float dt) {
    anim.update(dt);
}

void RColorPicker::Go() {
    const float duration = 0.5f;
    
    float t = anim.getCurrentValue();
    if(nextDirection) {
        anim.animateFromTo(t,1.0f);
        anim.setDuration(duration * (1.0f - t));
    } else {
        anim.animateFromTo(t,0.0f);
        anim.setDuration(duration * t);
        currColor = !currColor;
    }
    anim.setCurve(BOUNCY);
    nextDirection = !nextDirection;
}

void Vertex(ofVec3f controlPoint, ofRectangle rect)
{	
    float x = (controlPoint.x + 5.0f) / 10.0f * rect.getWidth()  + rect.getX();
    float y = (controlPoint.y + 5.0f) / 10.0f * rect.getHeight() + rect.getY();
    float z = controlPoint.z;
    glVertex3f(x,y,z);
}

void RColorPicker::drawHeightMap() {
    float t = anim.getCurrentValue();
    
    ofRectangle regLERP(regButton.position * (1.0f - t) + regPicker.position * t,
                        regButton.width    * (1.0f - t) + regPicker.width    * t,
                        regButton.height   * (1.0f - t) + regPicker.height   * t);
    
    for(int i = 0;i < 11;i++)
        for(int j = 0;j < 11;j++)
            geomBlended[i][j] = geomButton[i][j] * (1.0f - t) +
                                geomPicker[i][j] * t;

    glPushMatrix();
    glTranslatef(0,0,-256);
    heightMapShader.begin();
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            Vertex(geomBlended[i]  [j], regLERP);
            Vertex(geomBlended[i+1][j], regLERP);
            Vertex(geomBlended[i][j+1], regLERP);

            Vertex(geomBlended[i+1][j]  , regLERP);
            Vertex(geomBlended[i][j+1]  , regLERP);
            Vertex(geomBlended[i+1][j+1], regLERP);
        }
    }
    glEnd();
    heightMapShader.end();
    glPopMatrix();
}

void RColorPicker::drawGraphics() {
    float t = anim.getCurrentValue();
    
    ofRectangle regLERP(regButton.position * (1.0f - t) + regPicker.position * t,
                        regButton.width    * (1.0f - t) + regPicker.width    * t,
                        regButton.height   * (1.0f - t) + regPicker.height   * t);

    // Draw button
    ofSetColor(255,255,255);
    if(!currColor)
        imgButton1.draw(regLERP);
    else
        imgButton2.draw(regLERP);

    // Draw blended picker
    if(t > 0)
    {
        ofEnableAlphaBlending();
        ofSetColor(255,255,255,(int)(t*255));
        imgPicker.draw(regLERP);
        ofDisableAlphaBlending();
    }
}