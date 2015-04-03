//
//  Cube.cpp
//  Relief2
//
//  Created by Daniel Windham on 3/25/15.
//
//

#include "Cube.h"

//           w                 w    .*1
//  0+---------------+1         . ' | \                        ^ -y
//   |               |      0*'     |  \   . '                 |
// h |       +       |        \     |  .\'   )theta     -x <---+---> +x
//   |       ^center |       --\----+'---\----                 |
//  3+---------------+2       h \   |     \                    v +y
//
// all cube distances are in fractions of a containing unit-square; the image
// frame the cube comes from is interpreted as having units width = height = 1


// Constructors

Cube::Cube() {
    blob = NULL;
    hasMarker = false;
}

Cube::Cube(Blob *_blob, bool _update) {
    hasMarker = false;
    setBlob(_blob, _update);
}

Cube::Cube(Blob *_blob, ofPoint _marker, bool _update) {
    setBlobAndMarker(_blob, _marker, _update);
}

bool Cube::isValid() {
    return blob != NULL;
}


// Setters

void Cube::setBlob(Blob *_blob, bool _update) {
    blob = _blob;
    if (_update) {
        update();
    }
}

void Cube::setMarker(ofPoint _marker, bool _update) {
    rawMarker = _marker;
    hasMarker = true;
    if (_update) {
        update();
    }
}

void Cube::setBlobAndMarker(Blob *_blob, ofPoint _marker, bool _update) {
    blob = _blob;
    rawMarker = _marker;
    hasMarker = true;
    if (_update) {
        update();
    }
}

void Cube::clearMarker(bool _update) {
    hasMarker = false;
    if (_update) {
        update();
    }
}


// Update

void Cube::update() {
    if (!isValid()) {
        return;
    }

    // a blob's angleBoundingRect height and width variables are flipped. furthermore, blob
    // units are scaled by the size of the image they were found in. fix these mistakes.
    normalizationVector.set(1.0 / blob->widthScale, 1.0 / blob->heightScale);
    width = blob->angleBoundingRect.height * normalizationVector.x;
    height = blob->angleBoundingRect.width * normalizationVector.y;
    center.set(blob->angleBoundingRect.x, blob->angleBoundingRect.y);
    center *= normalizationVector;

    // normalized marker position relative to center
    if (hasMarker) {
        marker = rawMarker * normalizationVector - center;
    }

    // range is 0 <= rawTheta < 90; raw theta does not take cube orientation into account
    rawTheta = -blob->angle;
    rawThetaRadians = rawTheta * pi / 180;

    // relative corner coordinates using raw theta value
    rawCorners[0].x = -width/2 * cos(rawThetaRadians) - height/2 * sin(rawThetaRadians);
    rawCorners[0].y = width/2 * sin(rawThetaRadians) - height/2 * cos(rawThetaRadians);
    rawCorners[1].x = width/2 * cos(rawThetaRadians) - height/2 * sin(rawThetaRadians);
    rawCorners[1].y = -width/2 * sin(rawThetaRadians) - height/2 * cos(rawThetaRadians);
    rawCorners[2].x = -rawCorners[0].x;
    rawCorners[2].y = -rawCorners[0].y;
    rawCorners[3].x = -rawCorners[1].x;
    rawCorners[3].y = -rawCorners[1].y;

    // if the cube is marked, use this to determine its orientation. cornerA becomes corners[0], cornerB
    // becomes corners[1], and the rest follow. if there is no marker, leave corners as they are.
    int cornerA = 0;
    int cornerB = 1;
    if (hasMarker) {
        // determine which two cube corners the marker is closest to
        vector<pair<float, int> > distances; // (distance, index) tuple vector
        for (int i = 0; i < 4; i++) {
            float distance = marker.squareDistance(rawCorners[i]);
            distances.push_back(pair<float, int>(distance, i));
        }

        // sort by distance value, then extract matching indices
        sort(distances.begin(), distances.end());
        cornerA = min(distances[0].second, distances[1].second);
        cornerB = max(distances[0].second, distances[1].second);
        if (cornerA == 0 && cornerB == 3) {
            cornerA = 3;
            cornerB = 0;
        }
    }

    // adjust the cube angle appropriately
    theta = rawTheta + (360 - 90 * cornerA) % 360;
    thetaRadians = theta * pi / 180;

    // relative corner coordinates, determined by cycling indices of raw corners
    for (int i = 0; i < 4; i++) {
        corners[i] = rawCorners[(i + cornerA) % 4];
    }

    // absolute corner coordinates
    absCorners[0] = center + corners[0];
    absCorners[1] = center + corners[1];
    absCorners[2] = center + corners[2];
    absCorners[3] = center + corners[3];

    // cube boundary descriptors
    minX = absCorners[0].x;
    maxX = absCorners[2].x;
    minY = absCorners[1].y;
    maxY = absCorners[3].y;
}