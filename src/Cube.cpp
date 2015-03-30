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

Cube::Cube() {
    blob = NULL;
}

Cube::Cube(Blob *_blob) {
    update(_blob);
}

bool Cube::isValid() {
    return blob != NULL;
}

void Cube::update(Blob *_blob) {
    blob = _blob;
    update();
}

void Cube::update() {
    if (!isValid()) {
        return;
    }

    theta = -blob->angle;
    thetaRadians = theta * 3.14 / 180;

    // a blob's angleBoundingRect height and width variables are flipped. furthermore, blob
    // units are scaled by the size of the image they were found in. fix these mistakes.
    normalizationVector.set(1.0 / blob->widthScale, 1.0 / blob->heightScale);
    width = blob->angleBoundingRect.height * normalizationVector.x;
    height = blob->angleBoundingRect.width * normalizationVector.y;
    center.set(blob->angleBoundingRect.x, blob->angleBoundingRect.y);
    center *= normalizationVector;

    // relative corner coordinates
    corners[0].x = -width/2 * cos(thetaRadians) - height/2 * sin(thetaRadians);
    corners[0].y = width/2 * sin(thetaRadians) - height/2 * cos(thetaRadians);
    corners[1].x = width/2 * cos(thetaRadians) - height/2 * sin(thetaRadians);
    corners[1].y = -width/2 * sin(thetaRadians) - height/2 * cos(thetaRadians);
    corners[2].x = -corners[0].x;
    corners[2].y = -corners[0].y;
    corners[3].x = -corners[1].x;
    corners[3].y = -corners[1].y;

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