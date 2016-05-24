#include "ofApp.h"

#define DEBUG 1
#define HOVERTIMER 50
#define BUTTONRADIUS 70
#define JOINTRADIUS 50
#define MARKERSMODE 0 // 0 means showing the right position of each joint, 1 means showing if the current position is correct or not
#define TIMERSPEED 300
//ofSoundPlayer music;

// venus 0
// liberte 1
// liberty 1

//--------------------------------------------------------------
void ofApp::setup(){	
	ofSetFrameRate(60);

	// Kinect initialization code
	kinect.initSensor();

	// We first set up data streams for the Kinect (color, depth and skeleton tracking)
	kinect.initColorStream(640, 480, true);
	kinect.initDepthStream(640, 480, true);
	kinect.initSkeletonStream(false);	

	// simple start
	kinect.start();
	ofDisableAlphaBlending(); //Kinect alpha channel is default 0;

	ofBackground( ofColor::black );
	portada.loadImage("img/portada.png");

	// GRADATION IMAGES
	perfect.loadImage("img/perfect.png");
	almost_there.loadImage("img/almost_there.png");
	so_bad.loadImage("img/so_bad.png");

	loading1.loadImage("img/loading_page.png");

	// MENU IMAGES
	play_icon.loadImage("img/settings_icon.png");
	buttonicon_map.push_back(play_icon);
	
	play_icon.loadImage("img/play_icon.png");
	buttonicon_map.push_back(play_icon);
	
	play_icon.loadImage("img/help_icon.png");
	buttonicon_map.push_back(play_icon);

	play_icon.loadImage("img/go_back_icon.png");
	buttonicon_map.push_back(play_icon);

	play_icon.loadImage("img/easy_mode_icon.png");
	buttonicon_map.push_back(play_icon);

	play_icon.loadImage("img/naked_mode_icon.png");
	buttonicon_map.push_back(play_icon);

	loadArt();

	// BUTTON STUFF
	isHovering = false;
	hover_timer = HOVERTIMER;

	// ART
	loadArt();

	// GAME STUFF
	menu = -1;
	start = false;
	framesCounter = 0;
	atmomentframe = framesCounter;
	numimg = 1;
	timer_points = 0;
	min_time = 50000000;
	last_score = 50000000;

	// EASY MODE STUFF
	easymode = true;
	naked_mode.push_back(0); // venus
	naked_mode.push_back(3); // zeus

	// CONTENT
	fillFactMap();
	whatFactToLoad = fact_map.size() * rand()/RAND_MAX;
	whatPictureToLoad = 0;

	// MUSIC
	//music.loadSound("sound/beeth.mid");
	//music.setLoop(true); //Sound will loop
}

//--------------------------------------------------------------
void ofApp::update(){
	// Kinect's logic
	// First we update it
	kinect.update();

	// Check if there are skeletons
	if(kinect.isNewSkeleton()) 
	{
		// Go through skeletons
		for( int i = 0; i < kinect.getSkeletons().size(); i++) 
		{
			// Check if the skeleton is initialized
			if(kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_HEAD) != kinect.getSkeletons().at(i).end())
			{
				// Get joint information
				SkeletonBone headBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_HEAD)->second;
				SkeletonBone neckBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_SHOULDER_CENTER)->second;
				SkeletonBone lFootBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_FOOT_LEFT)->second;
				SkeletonBone rFootBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_FOOT_RIGHT)->second;
				SkeletonBone lHandBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_HAND_LEFT)->second;
				SkeletonBone rHandBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_HAND_RIGHT)->second;
				SkeletonBone lShoulderBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_SHOULDER_LEFT)->second;
				SkeletonBone rShoulderBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_SHOULDER_RIGHT)->second;
				SkeletonBone lElbowBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_ELBOW_LEFT)->second;
				SkeletonBone rElbowBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_ELBOW_RIGHT)->second;
				

				headPosition = scalePoint( ofPoint(headBone.getScreenPosition().x, headBone.getScreenPosition().y) );
				lFootPosition = scalePoint( ofPoint(lFootBone.getScreenPosition().x, lFootBone.getScreenPosition().y) );
				rFootPosition = scalePoint( ofPoint(rFootBone.getScreenPosition().x, rFootBone.getScreenPosition().y) );
				lHandPosition = scalePoint( ofPoint(lHandBone.getScreenPosition().x, lHandBone.getScreenPosition().y) );
				rHandPosition = scalePoint( ofPoint(rHandBone.getScreenPosition().x, rHandBone.getScreenPosition().y) );
				lElbowPosition = scalePoint( ofPoint(lElbowBone.getScreenPosition().x, lElbowBone.getScreenPosition().y) );
				rElbowPosition = scalePoint( ofPoint(rElbowBone.getScreenPosition().x, rElbowBone.getScreenPosition().y) );
				lShoulderPosition = scalePoint( ofPoint(lShoulderBone.getScreenPosition().x, lShoulderBone.getScreenPosition().y) );
				rShoulderPosition = scalePoint( ofPoint(rShoulderBone.getScreenPosition().x, rShoulderBone.getScreenPosition().y) );
			}
		}		
	}

	framesCounter++;
}

//--------------------------------------------------------------
void ofApp::draw()
{
	switch(menu)
	{
	//GAME OPTION
	case 0:
			//LOADING SCREEN
			if (!start)
			{
				getLoadingPage();
				if (framesCounter > (atmomentframe+100)) 
				{
					if ( lHandPosition.y > lFootPosition.y-10 && rHandPosition.y > lFootPosition.y-10  ) start = true;
				}
				
			//GAME SCREEN
			} else {
				// draw kinect's colour input
				kinect.draw(0, 0, ofGetHeight()*640/480, ofGetHeight());
				
				// art of reference
				ofSetRectMode(OF_RECTMODE_CORNER);
					ofPushMatrix();
						picture_map.at( whatPictureToLoad ).draw( ofGetWidth()/2, 0, std::max( ofGetHeight(),ofGetWidth()/2 ), ofGetHeight() );
					ofPopMatrix();
				ofPushStyle();

				//Skeleton generation
				drawSkeletonLines();
				drawMarkers();

				// detect pose
				detectPose();

				// points stuff
				timer_points++;
			}

			break;

	//CREDITS OPTION
	case 1:
			break;

	//HELP OPTION
	case 2:
			break;

	//MAIN MENU
	case -1:			 
			drawMenu();	
			break;
	case -2:
			drawSettings();
			break;
		
	}
}

void ofApp::drawMenu()
{
	// MAIN MENU DRAW
	for (int i = 0; i < 3; i++) 
	{
		ofSetRectMode(OF_RECTMODE_CORNER);
			ofPushMatrix();
			ofEnableAlphaBlending();
				buttonicon_map.at(i).draw( mainmenu_buttons.at(i).x-min(ofGetWidth()/4, 300)/2, mainmenu_buttons.at(i).y, min(ofGetWidth()/4, 300), min(ofGetWidth()/4, 300) );
			ofDisableAlphaBlending();
			ofPopMatrix();
		ofPopStyle();
	}
	
	int cursor = drawCursor();

	// PLAY MODE
	if ( cursor == 1 && hover_timer < 1)
	{
		menu = 0;
		//music.play();
		framesCounter = 0;
		atmomentframe = framesCounter;
		hover_timer = HOVERTIMER;
		StopHovering();
		timer_points = 0;
	}

	// SETTINGS MODE
	if ( cursor == 0 && hover_timer < 1)
	{
		menu = -2;
		//music.play();
		framesCounter = 0;
		atmomentframe = framesCounter;
		hover_timer = HOVERTIMER;		
		StopHovering();
	}

	std::string message = "Your min time is: " + std::to_string(min_time);
	std::string message2 = "Your last time was: " + std::to_string(last_score);

	ofDrawBitmapStringHighlight(message2, 0, ofGetHeight()*0.85, ofColor::white, ofColor::black);
	ofDrawBitmapStringHighlight(message, 0, ofGetHeight()*0.95, ofColor::white, ofColor::black);

	drawSkeletonLines();
}

void ofApp::drawSettings()
{
		// MAIN MENU DRAW
	for (int i = 3; i < 6; i++) 
	{
		ofSetRectMode(OF_RECTMODE_CORNER);
			ofPushMatrix();
			ofEnableAlphaBlending();
				buttonicon_map.at(i).draw( mainmenu_buttons.at(i-3).x-max(ofGetHeight()/5, 200)/2, mainmenu_buttons.at(i-3).y, min(ofGetWidth()/4, 300), min(ofGetWidth()/4, 300) );
			ofDisableAlphaBlending();
			ofPopMatrix();
		ofPopStyle();
	}
	
	int cursor = drawCursor();

	// MAIN MENU
	if ( cursor == 0 && hover_timer < 1)
	{
		menu = -1;
		//music.play();
		framesCounter = 0;
		atmomentframe = framesCounter;		
		hover_timer = HOVERTIMER;
		StopHovering();
	}

	if ( cursor == 1 && hover_timer < 1) 
	{
		easymode = true;
		menu = 0;
		framesCounter = 0;
		atmomentframe = framesCounter;
		hover_timer = HOVERTIMER;
		StopHovering();
		timer_points = 0;
	}
	if ( cursor == 2 && hover_timer < 1) 	
	{
		easymode = false;
		menu = 0;
		framesCounter = 0;
		atmomentframe = framesCounter;
		hover_timer = HOVERTIMER;
		StopHovering();
		timer_points = 0;
	}

	drawSkeletonLines();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (menu = -1)
	{
		//MENU OPTIONS
		switch(key)
		{

		//Play
		case OF_KEY_RETURN:
			menu = 0;
			//music.play();
			framesCounter = 0;
			break;

		//Credits
		case 'c':
		case 'C':
			menu = 1;
			break;

		//Help
		case 'h':
		case 'H':
			
			break;
		//Exit
		case OF_KEY_ESC:
			//music.unloadSound();
			exit();
			break;
		}
	}
}

//-------------------------------------------------------------
void ofApp::detectPose() {

	if ( easymode ) 
	{
		for( int index : naked_mode )
		{
			if ( index == whatPictureToLoad ) 
			{
				whatPictureToLoad = (whatPictureToLoad + 1) % picture_map.size();
				detectPose();
				return;
			}
		}
	}

	if ( whatPictureToLoad == 0 ) ofDrawBitmapStringHighlight("The Birth Of Venus - Botticelli (1486)", ofGetWidth()/2 + ofGetHeight()*0.05, ofGetHeight()*0.95, ofColor::white, ofColor::black);
	else if ( whatPictureToLoad == 1 ) ofDrawBitmapStringHighlight("La Liberté guidant le peuple - Delacroix (1830)", ofGetWidth()/2 + ofGetHeight()*0.05, ofGetHeight()*0.95, ofColor::white, ofColor::black);
	else if ( whatPictureToLoad == 2 ) ofDrawBitmapStringHighlight("Statue of Liberty - Bartholdi (1875)", ofGetWidth()/2 + ofGetHeight()*0.05, ofGetHeight()*0.95, ofColor::white, ofColor::black);
	float points = compareWithPose( whatPictureToLoad );

	int xposition = 20;
	float size = std::min( ofGetWidth()*0.2, 200.0 );
	
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofPushMatrix();
	ofEnableAlphaBlending();
	if ( points > 0.9 )
	{		
		perfect.draw( xposition, 0, size, size );

		//got the correct pose, so next image
		start = false;
		whatFactToLoad = fact_map.size() * rand()/RAND_MAX;
		
		whatPictureToLoad = (whatPictureToLoad + 1) % picture_map.size();

		//std::cout << "OLA" << std::endl;

		if ( whatPictureToLoad == 0 ) 
		{
			menu = -1;			
			//ofDrawBitmapStringHighlight("Your score is: " + timer_points, ofGetWidth()/2, ofGetHeight()/2, ofColor::white, ofColor::black);
			std::cout << "Your score is: " << timer_points << std::endl;
			if ( timer_points < min_time ) min_time = timer_points;
			last_score = timer_points;
		}
		
		numimg = 2;
		atmomentframe = framesCounter;			
	} 
	else if ( points > 0.3 )
	{
		//std::cout << "ALMOST TERE" << std::endl;
		almost_there.draw( xposition, 0, size, size ); // CLOSE POSITION
	}						
	else
	{	
		so_bad.draw( xposition, 0, size, size ); // BAD POSITION
	}

	ofDisableAlphaBlending();
	ofPopMatrix();
	ofPushStyle();


}

void ofApp::drawMarkers()
{
	float result = 1.0; // points
	pose newpose = pose_map.at( whatPictureToLoad ); // what pose do we have to compare the current pose with
	float joint_radius = JOINTRADIUS; // accepted distance from correct position
	
	ofPoint offset = newpose.headPositionPosePos - headPosition; // the position of each joint in relation to the head	

	if ( MARKERSMODE == 1) 
	{
		ofPushStyle();
			ofSetColor( ofColor::slateGrey );
			ofFill();		
			if (newpose.lFootPositionPosePos.distance(lFootPosition + offset) > joint_radius) ofCircle(lFootPosition, 10);
			if (newpose.rFootPositionPosePos.distance(rFootPosition + offset) > joint_radius) ofCircle(rFootPosition, 10);
			if (newpose.lHandPositionPosePos.distance(lHandPosition + offset) > joint_radius) ofCircle(lHandPosition, 10);
			if (newpose.rHandPositionPosePos.distance(rHandPosition + offset) > joint_radius) ofCircle(rHandPosition, 10);
			/*ofCircle(headPosition, 10);
			ofCircle(lElbowPosition, 10);
			ofCircle(rElbowPosition, 10);*/
		ofPopStyle();
	
		ofPushStyle();
			ofSetColor( ofColor::white );
			ofFill();		
			if (newpose.lFootPositionPosePos.distance(lFootPosition + offset) < joint_radius) ofCircle(lFootPosition, 10);
			if (newpose.rFootPositionPosePos.distance(rFootPosition + offset) < joint_radius) ofCircle(rFootPosition, 10);
			if (newpose.lHandPositionPosePos.distance(lHandPosition + offset) < joint_radius) ofCircle(lHandPosition, 10);
			if (newpose.rHandPositionPosePos.distance(rHandPosition + offset) < joint_radius) ofCircle(rHandPosition, 10);
			/*ofCircle(headPosition, 10);
			ofCircle(lElbowPosition, 10);
			ofCircle(rElbowPosition, 10);*/
		ofPopStyle();
	}
	else 
	{
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofPushMatrix();
		ofEnableAlphaBlending();
			if (newpose.lHandPositionPosePos.distance(lHandPosition + offset) > joint_radius) ofSetColor( ofColor::gray, 50 );
			else ofSetColor( ofColor::white, 60 );
			ofCircle(newpose.lHandPositionPosePos, JOINTRADIUS);

			if (newpose.rHandPositionPosePos.distance(rHandPosition + offset) > joint_radius) ofSetColor( ofColor::gray, 50 );
			else ofSetColor( ofColor::white, 60 );
			ofCircle(newpose.rHandPositionPosePos, JOINTRADIUS);

			if (newpose.lFootPositionPosePos.distance(lFootPosition + offset) > joint_radius) ofSetColor( ofColor::gray, 50 );
			else ofSetColor( ofColor::white, 60 );
			ofCircle(newpose.lFootPositionPosePos, JOINTRADIUS);

			if (newpose.rFootPositionPosePos.distance(rFootPosition + offset) > joint_radius) ofSetColor( ofColor::gray, 50 );
			else ofSetColor( ofColor::white, 60 );
			ofCircle(newpose.rFootPositionPosePos, JOINTRADIUS);
		ofDisableAlphaBlending();
		ofPopMatrix();
		ofPopStyle();
		ofSetColor( ofColor::white, 255 );
	}
}

void ofApp::getLoadingPage()
{
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofPushMatrix();
	ofEnableAlphaBlending();
		loading1.draw( ofGetWidth()/2, ofGetHeight()/2-ofGetWidth()/2*775/1061/2, ofGetWidth()/2, ofGetWidth()/2*775/1061 );
	ofDisableAlphaBlending();
	ofPopMatrix();
	ofPopStyle();

	std::string random_fact = "";	

	ofPushStyle();
		ofSetColor( ofColor::white );
		ofDrawBitmapString( "LOADING\n" + fact_map.at( whatFactToLoad ), ofGetHeight()*0.05, ofGetHeight()*0.85);
	ofPopStyle();
}

void ofApp::fillFactMap() {
	fact_map.push_back("The Statue of Liberty is of a robed female figure representing Libertas, the Roman goddess,\nwho bears a torch and a tabula ansata (a tablet evoking the law) upon which is inscribed\nthe date of the American Declaration of Independence, July 4, 1776.");
	fact_map.push_back("La Liberté guidant le peuple is a painting by Delacroix commemorating the July Revolution\nof 1830, which toppled King Charles X of France. A woman personifying the concept and the\ngoddess of Liberty leads the people forward over the bodies of the fallen.");
	fact_map.push_back("Did you know Artist Edgar Degas was so fascinated with ballet dancers that he became obsessed\nwith representing them in his art? It is estimated Degas made approximately 1500 paintings,\npastels, prints and drawings of dancers.");
	fact_map.push_back("In 1961, Le Bateau, a paper-cut from 1953 by Henri Matisse, hung upside-down for 2 months in\nthe Museum of Modern Art, New York – none of the 116,000 visitors had noticed. Genevieve Habert,\na stockbroker, noticed the mistake and notified a guard.");
	fact_map.push_back("Did you know that there is mysterious graffiti scribbled within a streak of the sky in Edvard\nMunch’s The Scream? No one knows whether it was Munch himself who wrote it, or if a disgruntled\nvisitor to one of his early exhibitions scribbled the pencil inscription onto the painting itself.");
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	currentPoseToString();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	// the points of each pose are in relation to the height of the screen !
	loadArt();
	
	// loading (god) picture is resized too !
	//int newsizex = ofGetWidth()/2;
	//loading1.resize(newsizex, newsizex*loading1.getHeight()/loading1.getWidth());
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::drawSkeletonLines()
{
	//Skeleton generation
	if(kinect.isNewSkeleton() && DEBUG)
		// for all skeletons
		for( int index = 0; index < kinect.getSkeletons().size(); index++) 
			// if they have a head
			if( kinect.getSkeletons().at(index).find( NUI_SKELETON_POSITION_HEAD ) != kinect.getSkeletons().at(index).end() ) 
			{
				// Iterate through joints
				for ( Skeleton::iterator it = kinect.getSkeletons().at(index).begin(); it != kinect.getSkeletons().at(index).end(); ++it ) 
				{
					// Get position and rotation
					SkeletonBone bone	= it->second;

					ofSetColor(255, 255, 255);
					ofSetLineWidth(3.0); // fat lines
					int startJoint = bone.getStartJoint();
					// do we have a start joint?					
					if ( kinect.getSkeletons().at(index).find( ( NUI_SKELETON_POSITION_INDEX ) startJoint ) != kinect.getSkeletons().at(index).end() ) 
					{
						// draw the line
						ofLine( scaleSkeletonJoint( bone.getScreenPosition() ), scaleSkeletonJoint( kinect.getSkeletons().at(index).find( ( NUI_SKELETON_POSITION_INDEX ) startJoint )->second.getScreenPosition() ) );
					}
					ofSetColor(255, 0, 0);
				}
				ofSetColor(255, 255, 255);
			}	
}

ofVec3f ofApp::scaleSkeletonJoint( ofVec3f joint )
{
	return ofVec3f(joint.x*ofGetHeight()/480, joint.y*ofGetHeight()/480, joint.z);
}

ofPoint ofApp::scalePoint( ofPoint joint, float aspect )
{
	return ofPoint(joint.x*ofGetHeight()/480, joint.y*ofGetHeight()/480);
}

void ofApp::currentPoseToString()
{
	std::cout << "pose newpose;" << std::endl;

	std::cout << "newpose.headPositionPosePos = ofPoint(" << headPosition << ");" << std::endl;
	std::cout << "newpose.lFootPositionPosePos = ofPoint(" << lFootPosition << ");" << std::endl;
	std::cout << "newpose.rFootPositionPosePos = ofPoint(" << rFootPosition << ");" << std::endl;
	std::cout << "newpose.lHandPositionPosePos = ofPoint(" << lHandPosition << ");" << std::endl;
	std::cout << "newpose.rHandPositionPosePos = ofPoint(" << rHandPosition << ");" << std::endl;
	std::cout << "newpose.lShoulderPositionPosePos = ofPoint(" << lShoulderPosition << ");" << std::endl;
	std::cout << "newpose.rShoulderPositionPosePos = ofPoint(" << rShoulderPosition << ");" << std::endl;
	std::cout << "newpose.lElbowPositionPosePos = ofPoint(" << lElbowPosition << ");" << std::endl;
	std::cout << "newpose.rElbowPositionPosePos = ofPoint(" << rElbowPosition << ");" << std::endl;

	std::cout << "pose_map.push_back(newpose);" << std::endl;
}

float ofApp::compareWithPose( int whatPose )
{
	float result = 1.0; // points
	pose newpose = pose_map.at( whatPose ); // what pose do we have to compare the current pose with
	float joint_radius = 45; // accepted distance from correct position
	
	ofPoint offset = newpose.headPositionPosePos - headPosition; // the position of each joint in relation to the head	

	if (newpose.lHandPositionPosePos.distance(lHandPosition + offset) > joint_radius) result -= 0.25;
	if (newpose.rHandPositionPosePos.distance(rHandPosition + offset) > joint_radius) result -= 0.25;
	if (newpose.lFootPositionPosePos.distance(lFootPosition + offset) > joint_radius) result -= 0.075;
	if (newpose.rFootPositionPosePos.distance(rFootPosition + offset) > joint_radius) result -= 0.075;

	//std::cout << result << std::endl;
	return result;
}

void ofApp::loadArt()
{
	picture_map.clear();
	pose_map.clear();
	mainmenu_buttons.clear();

	picture1.loadImage("img/botticelli_venus.jpg");
	picture_map.push_back(picture1);

	// VENUS POSE
	pose newpose;
	newpose.headPositionPosePos = ofPoint(255.896, 94.1851, 0)*ofGetHeight()/600;
	newpose.lFootPositionPosePos = ofPoint(218.72, 542.743, 0)*ofGetHeight()/600;
	newpose.rFootPositionPosePos = ofPoint(278.37, 541.126, 0)*ofGetHeight()/600;
	newpose.lHandPositionPosePos = ofPoint(290.954, 216.128, 0)*ofGetHeight()/600;
	newpose.rHandPositionPosePos = ofPoint(322.917, 318.526, 0)*ofGetHeight()/600;
	newpose.lShoulderPositionPosePos = ofPoint(223.228, 186.426, 0)*ofGetHeight()/600;
	newpose.rShoulderPositionPosePos = ofPoint(306.632, 178.922, 0)*ofGetHeight()/600;
	newpose.lElbowPositionPosePos = ofPoint(205.386, 245.572, 0)*ofGetHeight()/600;
	newpose.rElbowPositionPosePos = ofPoint(330.498, 247.506, 0)*ofGetHeight()/600;
	pose_map.push_back(newpose);

	picture1.loadImage("img/delacroix_liberte.png");
	picture_map.push_back(picture1);

	// LIBERTÉ GUIDANT LE PEUPLE POSE
	newpose.headPositionPosePos = ofPoint(255.54, 117.705, 0)*ofGetHeight()/600;
	newpose.lFootPositionPosePos = ofPoint(271.768, 516.077, 0)*ofGetHeight()/600;
	newpose.rFootPositionPosePos = ofPoint(296.114, 550.008, 0)*ofGetHeight()/600;
	newpose.lHandPositionPosePos = ofPoint(116.641, 20.8717, 0)*ofGetHeight()/600;
	newpose.rHandPositionPosePos = ofPoint(332.129, 290.221, 0)*ofGetHeight()/600;
	newpose.lShoulderPositionPosePos = ofPoint(206.314, 179.636, 0)*ofGetHeight()/600;
	newpose.rShoulderPositionPosePos = ofPoint(294.424, 202.223, 0)*ofGetHeight()/600;
	newpose.lElbowPositionPosePos = ofPoint(158.468, 109.926, 0)*ofGetHeight()/600;
	newpose.rElbowPositionPosePos = ofPoint(303.017, 252.634, 0)*ofGetHeight()/600;
	pose_map.push_back(newpose);

	// VELAZQUEZ MENINAS

	picture1.loadImage("img/velazquez_menina.jpg");
	picture_map.push_back(picture1);
	
	newpose.headPositionPosePos = ofPoint(269.774, 139.126, 0)*ofGetHeight()/600;
	newpose.lFootPositionPosePos = ofPoint(266.407, 539.657, 0)*ofGetHeight()/600;
	newpose.rFootPositionPosePos = ofPoint(302.804, 533.003, 0)*ofGetHeight()/600;
	newpose.lHandPositionPosePos = ofPoint(136.309, 267.238, 0)*ofGetHeight()/600;
	newpose.rHandPositionPosePos = ofPoint(357.478, 296.578, 0)*ofGetHeight()/600;
	newpose.lShoulderPositionPosePos = ofPoint(221.996, 228.436, 0)*ofGetHeight()/600;
	newpose.rShoulderPositionPosePos = ofPoint(301.927, 223.455, 0)*ofGetHeight()/600;
	newpose.lElbowPositionPosePos = ofPoint(167.488, 257.492, 0)*ofGetHeight()/600;
	newpose.rElbowPositionPosePos = ofPoint(326.364, 268.01, 0)*ofGetHeight()/600;
	pose_map.push_back(newpose);

	// ZEUS

	picture1.loadImage("img/zeus.jpg");
	picture_map.push_back(picture1);
	
	newpose.headPositionPosePos = ofPoint(259.112, 122.53, 0)*ofGetHeight()/600;
	newpose.lFootPositionPosePos = ofPoint(222.712, 540.02, 0)*ofGetHeight()/600;
	newpose.rFootPositionPosePos = ofPoint(315.324, 561.126, 0)*ofGetHeight()/600;
	newpose.lHandPositionPosePos = ofPoint(78.1095, 114.022, 0)*ofGetHeight()/600;
	newpose.rHandPositionPosePos = ofPoint(429.147, 188.493, 0)*ofGetHeight()/600;
	newpose.lShoulderPositionPosePos = ofPoint(206.055, 208.803, 0)*ofGetHeight()/600;
	newpose.rShoulderPositionPosePos = ofPoint(303.004, 200.549, 0)*ofGetHeight()/600;
	newpose.lElbowPositionPosePos = ofPoint(150.271, 196.281, 0)*ofGetHeight()/600;
	newpose.rElbowPositionPosePos = ofPoint(360.91, 202.483, 0)*ofGetHeight()/600;
	pose_map.push_back(newpose);

	// COLON

	picture1.loadImage("img/statue_colon.jpg");
	picture_map.push_back(picture1);
	
	newpose.headPositionPosePos = ofPoint(274.448, 97.5521, 0)*ofGetHeight()/600;
	newpose.lFootPositionPosePos = ofPoint(221.771, 525.882, 0)*ofGetHeight()/600;
	newpose.rFootPositionPosePos = ofPoint(290.847, 532.242, 0)*ofGetHeight()/600;
	newpose.lHandPositionPosePos = ofPoint(49.9308, 153.066, 0)*ofGetHeight()/600;
	newpose.rHandPositionPosePos = ofPoint(300.486, 244.553, 0)*ofGetHeight()/600;
	newpose.lShoulderPositionPosePos = ofPoint(225.392, 186.688, 0)*ofGetHeight()/600;
	newpose.rShoulderPositionPosePos = ofPoint(317.328, 195.275, 0)*ofGetHeight()/600;
	newpose.lElbowPositionPosePos = ofPoint(165.376, 162.401, 0)*ofGetHeight()/600;
	newpose.rElbowPositionPosePos = ofPoint(357.495, 233.097, 0)*ofGetHeight()/600;
	pose_map.push_back(newpose);

	//goya_fusilamientos

	picture1.loadImage("img/goya_fusilamientos.jpg");
	picture_map.push_back(picture1);
	
	newpose.headPositionPosePos = ofPoint(194.396, 223.452, 0)*ofGetHeight()/600;
	newpose.lFootPositionPosePos = ofPoint(189.587, 585.901, 0)*ofGetHeight()/600;
	newpose.rFootPositionPosePos = ofPoint(233.426, 577.207, 0)*ofGetHeight()/600;
	newpose.lHandPositionPosePos = ofPoint(42.3051, 179.186, 0)*ofGetHeight()/600;
	newpose.rHandPositionPosePos = ofPoint(350.796, 159.442, 0)*ofGetHeight()/600;
	newpose.lShoulderPositionPosePos = ofPoint(147.67, 304.146, 0)*ofGetHeight()/600;
	newpose.rShoulderPositionPosePos = ofPoint(243.988, 296.787, 0)*ofGetHeight()/600;
	newpose.lElbowPositionPosePos = ofPoint(96.305, 241.82, 0)*ofGetHeight()/600;
	newpose.rElbowPositionPosePos = ofPoint(290.803, 236.432, 0)*ofGetHeight()/600;
	pose_map.push_back(newpose);

	picture1.loadImage("img/bartholdi_liberty.jpg");
	picture_map.push_back(picture1);

	// LIBERTY STATUE
	newpose.headPositionPosePos = ofPoint(252.661, 90.1178, 0)*ofGetHeight()/600;
	newpose.lFootPositionPosePos = ofPoint(235.699, 524.322, 0)*ofGetHeight()/600;
	newpose.rFootPositionPosePos = ofPoint(279.183, 530.629, 0)*ofGetHeight()/600;
	newpose.lHandPositionPosePos = ofPoint(179.578, 3.71824, 0)*ofGetHeight()/600;
	newpose.rHandPositionPosePos = ofPoint(303.326, 258.225, 0)*ofGetHeight()/600;
	newpose.lShoulderPositionPosePos = ofPoint(208.144, 165.157, 0)*ofGetHeight()/600;
	newpose.rShoulderPositionPosePos = ofPoint(284.972, 166.809, 0)*ofGetHeight()/600;
	newpose.lElbowPositionPosePos = ofPoint(178.512, 92.3535, 0)*ofGetHeight()/600;
	newpose.rElbowPositionPosePos = ofPoint(299.672, 231.031, 0)*ofGetHeight()/600;
	pose_map.push_back(newpose);

	mainmenu_buttons.push_back( ofPoint(ofGetWidth()/4, 0) ); // SETTINGS
	mainmenu_buttons.push_back( ofPoint(ofGetWidth()/2, 0) ); // PLAY
	mainmenu_buttons.push_back( ofPoint(3*ofGetWidth()/4, 0) ); // HELP
}

void ofApp::StartHovering( int button )
{
    if (!isHovering)
    {
        isHovering = true;
		if (menu == -1) 
		{
			if (button == 0) buttonicon_map.at(0).loadImage("img/settings_icon_hover.png");
			if (button == 1) buttonicon_map.at(1).loadImage("img/play_icon_hover.png");
			if (button == 2) buttonicon_map.at(2).loadImage("img/help_icon_hover.png");
		}
		if (menu == -2) 
		{
			if (button == 0) buttonicon_map.at(3).loadImage("img/go_back_icon_hover.png");
			if (button == 1) buttonicon_map.at(4).loadImage("img/easy_mode_icon_hover.png");
			if (button == 2) buttonicon_map.at(5).loadImage("img/naked_mode_icon_hover.png");
		}
    }
}

void ofApp::StopHovering()
{
    if (isHovering)
    {
        isHovering = false;
		hover_timer = HOVERTIMER;
		buttonicon_map.at(0).loadImage("img/settings_icon.png");
		buttonicon_map.at(1).loadImage("img/play_icon.png");
		buttonicon_map.at(2).loadImage("img/help_icon.png");
		buttonicon_map.at(3).loadImage("img/go_back_icon.png");
		buttonicon_map.at(4).loadImage("img/easy_mode_icon.png");
		buttonicon_map.at(5).loadImage("img/naked_mode_icon.png");
    }
}

int ofApp::drawCursor()
{
	int button = isCursorInButton(lHandPosition);

    if ( button > -1 ) StartHovering( button );
    else StopHovering();

	ofPushStyle();
	ofEnableAlphaBlending();
		if ( isHovering ) 
		{
			ofSetColor( ofColor(ofColor::white, 100) );
			ofFill();		
			ofCircle(lHandPosition, 20+HOVERTIMER/std::max(5, hover_timer)+5*sin(2*PI*hover_timer/HOVERTIMER));
			//20 = original size
			//HOVERTIMER/std::max(5,hover_timer) = grows with time 10 units: 50/50 = 1, 50/5 = 10
			//5*sin(2*PI*hover_timer/HOVERTIMER)) = beats while hovering
		}
		else 
		{
			ofSetColor( ofColor(ofColor::white, 100) );
			ofFill();		
			ofCircle(lHandPosition, 20);
		}
	ofDisableAlphaBlending();
	ofPopStyle();

	if ( isHovering ) hover_timer--;
	//std::cout << hover_timer << std::endl;
	return button;
}

int ofApp::isCursorInButton( ofPoint cursor )
{
	int start = 0;
	if (menu == -2) start = 3;

	for ( int i = 0; i < mainmenu_buttons.size(); i++)
		if ( (mainmenu_buttons.at(i)+ofPoint(0, 25)).distance(cursor) < BUTTONRADIUS )
			return i;

    return -1;
}