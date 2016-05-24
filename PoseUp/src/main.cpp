#include "ofMain.h"
#include "ofApp.h"

#include "ofGLProgrammableRenderer.h"

//========================================================================
int main( ){

	ofPtr<ofBaseRenderer> renderer(new ofGLProgrammableRenderer(true));
	ofSetCurrentRenderer(renderer);

	//ofSetupOpenGL(850, 480, OF_FULLSCREEN);
	ofSetupOpenGL(800, 600, OF_WINDOW);
	//ofSetupOpenGL(1200, 750, OF_WINDOW);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
