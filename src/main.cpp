#include "ofMain.h"
#include "ReliefApplication.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
	
    ofAppGlutWindow window;
	// Open the application window accross two screens.
	// As the two might have a different screen size, this code makes sure it's set to the max height
	//int screenHeight = SCREEN1_HEIGHT;
	//if (SCREEN2_HEIGHT > SCREEN1_HEIGHT) screenHeight = SCREEN2_HEIGHT;
	//ofSetupOpenGL(&window, SCREEN1_WIDTH + SCREEN2_WIDTH, screenHeight, OF_WINDOW);

	//ofRunApp( new ReliefApplication());
    
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);

	ofRunApp(new ReliefApplication()); // start the app
}
