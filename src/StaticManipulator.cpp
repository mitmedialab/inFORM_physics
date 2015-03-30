//
//  StaticManipulator.cpp
//  Relief2
//
//  Created by Daniel Windham on 3/18/15.
//
//

#include "StaticManipulator.h"

StaticManipulator::StaticManipulator(KinectTracker *tracker) {
    kinectTracker = tracker;
    pinHeightMapImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, GL_RGBA);

    // by default, direction starts as up
    currentDirection = UP;
}

void StaticManipulator::drawHeightMap() {
    pinHeightMapImage.draw(0, 0);
}

void StaticManipulator::drawGraphics() {
}

void StaticManipulator::update(float dt) {
    pinHeightMapImage.begin();
    ofBackground(0);
    ofSetColor(255);
    drawDirectionMover(RELIEF_PROJECTOR_SIZE_X);
    drawFence(RELIEF_PROJECTOR_SIZE_X);
    pinHeightMapImage.end();
}

void StaticManipulator::drawFence(float lengthScale) {
    ofSetColor(255);
    int pinLength = lengthScale / 30;
    ofRect(0, 0, lengthScale, pinLength); // top bar
    ofRect(0, 0, pinLength, lengthScale); // left bar
    ofRect(lengthScale - pinLength, 0, pinLength, lengthScale); // right bar
    ofRect(0, lengthScale - pinLength, lengthScale, pinLength); // bottom bar
}

void StaticManipulator::drawDirectionMover(float lengthScale) {
    // simple description of cube dimensions
    float width = 4.0 / 30;
    float height = 4.0 / 30;

    if (kinectTracker->redCubes.size() != 1) {
        return;
    }

    Cube cube = kinectTracker->redCubes[0];
    
    int left, right, top, bottom;

    if (currentDirection == LEFT) {
        left = (cube.minX + 0.3 * width) * lengthScale;
        right = (cube.maxX + 0.3 * width) * lengthScale;
        top = (cube.minY - 0.3 * height) * lengthScale;
        bottom = (cube.maxY + 0.3 * height) * lengthScale;
        int sliceWidth = (right - left) / 10;
        for (int i = 0; i < 10; i++) {
            ofSetColor(30 + 10 * i);
            int sliceLeft = left + i * sliceWidth;
            ofRect(sliceLeft, top, sliceWidth, bottom - top);
        }
    }

    if (currentDirection == RIGHT) {
        left = (cube.minX - 0.3 * width) * lengthScale;
        right = (cube.maxX - 0.3 * width) * lengthScale;
        top = (cube.minY - 0.3 * height) * lengthScale;
        bottom = (cube.maxY + 0.3 * height) * lengthScale;
        int sliceWidth = (right - left) / 10;
        for (int i = 0; i < 10; i++) {
            ofSetColor(120 - 10 * i);
            int sliceLeft = left + i * sliceWidth;
            ofRect(sliceLeft, top, sliceWidth, bottom - top);
        }
    }

    if (currentDirection == UP) {
        left = (cube.minX - 0.3 * width) * lengthScale;
        right = (cube.maxX + 0.3 * width) * lengthScale;
        top = (cube.minY + 0.3 * height) * lengthScale;
        bottom = (cube.maxY + 0.3 * height) * lengthScale;
        int sliceHeight = (bottom - top) / 10;
        for (int i = 0; i < 10; i++) {
            ofSetColor(30 + 10 * i);
            int sliceTop = top + i * sliceHeight;
            ofRect(left, sliceTop, right - left, sliceHeight);
        }
    }

    if (currentDirection == DOWN) {
        left = (cube.minX - 0.3 * width) * lengthScale;
        right = (cube.maxX + 0.3 * width) * lengthScale;
        top = (cube.minY - 0.3 * height) * lengthScale;
        bottom = (cube.maxY - 0.3 * height) * lengthScale;
        int sliceHeight = (bottom - top) / 10;
        for (int i = 0; i < 10; i++) {
            ofSetColor(120 - 10 * i);
            int sliceTop = top + i * sliceHeight;
            ofRect(left, sliceTop, right - left, sliceHeight);
        }
    }
}

void StaticManipulator::keyPressed(int key) {
    if(key == KEY_UP) {
        currentDirection = UP;
    }

    if(key == KEY_DOWN) {
        currentDirection = DOWN;
    }

    if(key == KEY_LEFT) {
        currentDirection = LEFT;
    }

    if(key == KEY_RIGHT) {
        currentDirection = RIGHT;
    }
}