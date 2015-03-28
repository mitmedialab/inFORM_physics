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
    drawDirectionMover();
    drawFence();
    pinHeightMapImage.end();
}

void StaticManipulator::drawFence() {
    ofSetColor(255);
    int pinLength = RELIEF_PROJECTOR_SIZE_X / 30;
    ofRect(0, 0, RELIEF_PROJECTOR_SIZE_X, pinLength); // top bar
    ofRect(0, 0, pinLength, RELIEF_PROJECTOR_SIZE_X); // left bar
    ofRect(RELIEF_PROJECTOR_SIZE_X - pinLength, 0, pinLength, RELIEF_PROJECTOR_SIZE_X); // right bar
    ofRect(0, RELIEF_PROJECTOR_SIZE_X - pinLength, RELIEF_PROJECTOR_SIZE_X, pinLength); // bottom bar
}

void StaticManipulator::drawDirectionMover() {
    // simple description of cube dimensions
    float width = 4.0 / 30;
    float height = 4.0 / 30;

    Cube cube = kinectTracker->currentCube;

    ofPoint center = cube.center;
    
    int left, right, top, bottom;

    if (currentDirection == LEFT) {
        left = (cube.minX + 0.3 * width) * RELIEF_PROJECTOR_SIZE_X;
        right = (cube.maxX + 0.3 * width) * RELIEF_PROJECTOR_SIZE_X;
        top = (cube.minY - 0.3 * height) * RELIEF_PROJECTOR_SIZE_X;
        bottom = (cube.maxY + 0.3 * height) * RELIEF_PROJECTOR_SIZE_X;
        int sliceWidth = (right - left) / 10;
        for (int i = 0; i < 10; i++) {
            ofSetColor(30 + 10 * i);
            int sliceLeft = left + i * sliceWidth;
            ofRect(sliceLeft, top, sliceWidth, bottom - top);
        }
    }

    if (currentDirection == RIGHT) {
        left = (cube.minX - 0.3 * width) * RELIEF_PROJECTOR_SIZE_X;
        right = (cube.maxX - 0.3 * width) * RELIEF_PROJECTOR_SIZE_X;
        top = (cube.minY - 0.3 * height) * RELIEF_PROJECTOR_SIZE_X;
        bottom = (cube.maxY + 0.3 * height) * RELIEF_PROJECTOR_SIZE_X;
        int sliceWidth = (right - left) / 10;
        for (int i = 0; i < 10; i++) {
            ofSetColor(120 - 10 * i);
            int sliceLeft = left + i * sliceWidth;
            ofRect(sliceLeft, top, sliceWidth, bottom - top);
        }
    }

    if (currentDirection == UP) {
        left = (cube.minX - 0.3 * width) * RELIEF_PROJECTOR_SIZE_X;
        right = (cube.maxX + 0.3 * width) * RELIEF_PROJECTOR_SIZE_X;
        top = (cube.minY + 0.3 * height) * RELIEF_PROJECTOR_SIZE_X;
        bottom = (cube.maxY + 0.3 * height) * RELIEF_PROJECTOR_SIZE_X;
        int sliceHeight = (bottom - top) / 10;
        for (int i = 0; i < 10; i++) {
            ofSetColor(30 + 10 * i);
            int sliceTop = top + i * sliceHeight;
            ofRect(left, sliceTop, right - left, sliceHeight);
        }
    }

    if (currentDirection == DOWN) {
        left = (cube.minX - 0.3 * width) * RELIEF_PROJECTOR_SIZE_X;
        right = (cube.maxX + 0.3 * width) * RELIEF_PROJECTOR_SIZE_X;
        top = (cube.minY - 0.3 * height) * RELIEF_PROJECTOR_SIZE_X;
        bottom = (cube.maxY - 0.3 * height) * RELIEF_PROJECTOR_SIZE_X;
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