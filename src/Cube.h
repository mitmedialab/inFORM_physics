//
//  Cube.h
//  Relief2
//
//  Created by Daniel Windham on 3/25/15.
//
//

#include "ofxKCore.h"
#include <vector>


#ifndef __Relief2__Cube__
#define __Relief2__Cube__

//           w                 w    .*1
//  0+---------------+1         . ' | \                        ^ -y
//   |               |      0*'     |  \   . '                 |
// h |       +       |        \     |  .\'   )theta     -x <---+---> +x
//   |       ^center |       --\----+'---\----                 |
//  3+---------------+2       h \   |     \                    v +y
//
// all cube distances are in fractions of a containing unit-square; the image
// frame the cube comes from is interpreted as having units width = height = 1

class Cube {
public:
    Cube();
    Cube(Blob *_blob);
    bool isValid();         // test if cube is set up; cube only has meaning when it owns a blob
    void updateBlob(Blob *_blob);
    void update();

    Blob *blob;
    float theta;            // measured counterclockwise
    float thetaRadians;     // theta in radians
    float width;
    float height;
    ofPoint center;
    ofPoint corners[4];     // coordinates relative to center
    ofPoint absCorners[4];  // corners in absolute coordinates
    float minX, maxX, minY, maxY; // cube boundary descriptors (absolute coordinates)
};
#endif /* defined(__Relief2__Cube__) */
