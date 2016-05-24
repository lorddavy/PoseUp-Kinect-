#pragma once

#include "ofMain.h"
#include "ofxKinectCommonBridge.h"

struct pose {
	ofPoint headPositionPosePos;
	ofPoint lFootPositionPosePos;
	ofPoint rFootPositionPosePos;
	ofPoint lHandPositionPosePos;
	ofPoint rHandPositionPosePos;
	ofPoint lShoulderPositionPosePos;
	ofPoint rShoulderPositionPosePos;
	ofPoint lElbowPositionPosePos;
	ofPoint rElbowPositionPosePos;
};

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		// GAME METHODS
		void detectPose();
		void getLoadingPage();
		void fillFactMap();
		void drawMarkers();
		void drawSkeletonLines();
		ofVec3f scaleSkeletonJoint( ofVec3f joint );
		ofPoint scalePoint( ofPoint joint, float aspect = 1 );
		void currentPoseToString();
		float compareWithPose( int whatPose );
		void loadArt();		

		// MENU METHODS
		void drawSettings();
		void drawMenu();
		int isCursorInButton( ofPoint cursor );
		int drawCursor();
		void StartHovering( int button );
		void StopHovering();
		
		ofxKinectCommonBridge kinect;

		ofImage portada;
		ofImage loading1;
		ofImage picture1;

		ofImage perfect;
		ofImage almost_there;
		ofImage so_bad;

		ofImage play_icon;

		ofPoint headPosition;
		ofPoint lFootPosition;
		ofPoint rFootPosition;
		ofPoint lHandPosition;
		ofPoint rHandPosition;
		ofPoint lShoulderPosition;
		ofPoint rShoulderPosition;
		ofPoint lElbowPosition;
		ofPoint rElbowPosition;

		int menu;
		int whatFactToLoad;
		int whatPictureToLoad;
		bool start;
		double framesCounter;
		double atmomentframe;  //frames counter at that time when I have detected the pose correctly
		int numimg;
		int hover_timer;
		bool isHovering;
		bool easymode;
		int timer_points;
		int min_time;
		int last_score;

		std::vector<std::string> fact_map;
		std::vector<ofImage> picture_map;
		std::vector<pose> pose_map;
		std::vector<ofPoint> mainmenu_buttons;
		std::vector<ofImage> buttonicon_map;
		std::vector<int> naked_mode;
};