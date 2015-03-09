//
//  RFunction.cpp
//  Relief2
//
//  Created by TMG on 11/9/13.
//
//

#include <math.h>

#include "RFunction.h"

RFunction::RFunction() {
    loadShader();
    for(unsigned int i = 0; i < RELIEF_SIZE_X; i++)
        for(unsigned int j = 0; j < RELIEF_SIZE_Y; j++) {
            prev[i][j] = 0;
            curr[i][j] = 0;
            next[i][j] = 0;
        }
    
    {ofImage i;i.loadImage("equations/1.png");images.push_back(i);}
    {ofImage i;i.loadImage("equations/2.png");images.push_back(i);}
    {ofImage i;i.loadImage("equations/3.png");images.push_back(i);}
    {ofImage i;i.loadImage("equations/4.png");images.push_back(i);}
    {ofImage i;i.loadImage("equations/5.png");images.push_back(i);}
    
    nextFunction();
    anim.setDuration(0.0f);
}

ofColor fcolor(float y) {
    
}

void RFunction::update(float dt) {
    anim.update(dt);
    //if(anim.isAnimating())
        for (int i = 0; i < RELIEF_SIZE_X; i++)
            for (int j = 0; j < RELIEF_SIZE_Y; j++)
                curr[i][j] = ofLerp(prev[i][j],next[i][j],anim.getCurrentValue());
    return;
}

void RFunction::nextFunction() {
    for (int i = 0; i < RELIEF_SIZE_X; i++) {
        for (int j = 0; j < RELIEF_SIZE_Y; j++) {
            // Copy current to previous
            prev[i][j] = curr[i][j];
            
            float x = ((float)i - (float)(RELIEF_SIZE_X-1) / 2.0f) / ((float)(RELIEF_SIZE_X-1) / 2.0f);
            float y = ((float)j - (float)(RELIEF_SIZE_Y-1) / 2.0f) / ((float)(RELIEF_SIZE_Y-1) / 2.0f);
            switch (currFunc) {
                case 0: next[i][j] = x * y; break;
                case 1: next[i][j] = std::sqrt(4.0f - 2.0f*x*x - 2.0f*y*y) - 1.0f; break;
                case 2: next[i][j] = x*x - y*y; break;
                case 3: next[i][j] = std::cos(8.0f * (x*x + y*y)) * std::exp(-x*x - y*y); break;
                case 4: next[i][j] = (x*y*y*y - y*x*x*x) / 0.2f;
                default:
                    break;
            }
        }
    }
    currFunc = (currFunc + 1) % 5;

    anim.setDuration(1.0f);
    anim.setCurve(EASE_IN_EASE_OUT);
    anim.animateFromTo(0.0f,1.0f);
}

void Vertex(int i, int j, float curr[RELIEF_SIZE_X][RELIEF_SIZE_Y]) {
    float border = 3;
    float x = RELIEF_PROJECTOR_SIZE_X * (border + (RELIEF_SIZE_X-2*border - 1) * float(i) / (RELIEF_SIZE_X-1)) / (RELIEF_SIZE_X-1);
    float y = RELIEF_PROJECTOR_SIZE_Y * (border + (RELIEF_SIZE_Y-2*border - 1) * float(j) / (RELIEF_SIZE_Y-1)) / (RELIEF_SIZE_Y-1);
    
    float height = (curr[i][j] + 1.0f) / 2.0f * 256.0f;
    float hue = std::min(std::max(height,0.0f),250.0f);

    ofSetColor(ofColor::fromHsb(hue,255.0f,255.0f));
    glVertex3f(x,y,height - 256);
}

void RFunction::drawHeightMap() {
    heightMapShader.begin();
    glPushMatrix();
    ofClear(0,0,0);
    glBegin(GL_TRIANGLES);
    for(int i = 0;i < RELIEF_SIZE_X-1;i++)
        for(int j = 0; j < RELIEF_SIZE_Y-1; j++)
        {
            Vertex(i,j,curr); Vertex(i+1,j,curr); Vertex(i,j+1,curr);
            Vertex(i+1,j,curr); Vertex(i,j+1,curr); Vertex(i+1,j+1,curr);
        }
    
    glEnd();
    glPopMatrix();
    heightMapShader.end();
}

void RFunction::drawGraphics() {
    ofSetColor(this->color);
    glPushMatrix();
    glBegin(GL_TRIANGLES);
    
    for(int i = 0;i < RELIEF_SIZE_X-1;i++)
        for(int j = 0; j < RELIEF_SIZE_Y-1; j++)
        {
            Vertex(i,j,curr); Vertex(i+1,j,curr); Vertex(i,j+1,curr);
            Vertex(i+1,j,curr); Vertex(i,j+1,curr); Vertex(i+1,j+1,curr);
        }
    
    glEnd();
    glPopMatrix();
}

void RFunction::drawEquation() {
    ofSetColor(ofColor::white);
    int pi =(currFunc + images.size() - 2) % images.size();
    int ci =(currFunc + images.size() - 1) % images.size();
    
    int fw = 900;
    int fh = 100 * RELIEF_PROJECTOR_SIZE_Y / RELIEF_PROJECTOR_SCALED_SIZE_Y;
    
    float h = 0.5f * fh;
    
    float wp = h * images[pi].width / images[pi].height;
    float wc = h * images[ci].width / images[ci].height;

    images[pi].draw(fw / 2 - wp / 2 + 900 * anim.getCurrentValue(),
                    fh / 2 - h  / 2,
                    wp, h);

    images[ci].draw(fw / 2 - wc / 2 + 900 * anim.getCurrentValue() - 900,
                    fh / 2 - h / 2,
                    wc, h);
}