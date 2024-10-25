#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include <set>;

//sources: https://www.geeksforgeeks.org/set-in-cpp-stl/#


class ofApp : public ofBaseApp {

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
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	bool bHide;

	ofxPanel gui;
	ofxIntSlider complexityLevel;
	ofColor textColor;
	ofxLabel screenSize;

	//agent gui
	ofxToggle agentToggleSprite;
	ofxFloatSlider agentLifespan;
	ofxFloatSlider spawnRate;
	ofxFloatSlider agentScale;
	ofxFloatSlider agentRotationSpeed = 3.0;
	ofxFloatSlider nAgents = 1;
	ofxFloatSlider agentSpeed = 1.0;

	//player gui
	ofxFloatSlider playerScale;
	ofxFloatSlider playerRotSpeed = 5.0f;
	ofxToggle toggleHeadingVector;
	ofxFloatSlider playerSpeed = 10.0f;
	ofxFloatSlider playerEnergy = 10.0f;
	ofxToggle playerToggleSprite;

	bool gameState;
	float endTime;
	float startTime;
	int nEnergy;
	bool playerSprite = false;

	Emitter emitter;
	Sprite player;
	vector<Sprite> agents;

	std::set<int> keysPressed;

	ofImage turtle;
	ofImage coconut;
	ofImage background;

};
