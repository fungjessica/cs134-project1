#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include <set>;

//sources: https://www.geeksforgeeks.org/set-in-cpp-stl/#

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		bool bHide;

		ofxPanel gui;
		ofxIntSlider complexityLevel;
		ofColor textColor;
		ofxToggle playerToggle;

		bool gameState;
		float endTime;
		float startTime;
		int nEnergy;
		bool playerSprite;

		Emitter emitter;
		Sprite player;

		std::set<int> keysPressed;
};
