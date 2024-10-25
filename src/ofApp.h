#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include <set>;

//sources: https://www.geeksforgeeks.org/set-in-cpp-stl/#

class ofApp : public ofBaseApp {
private:
	void setComplexityDefault(int level);

	int trackComplexity;

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

	//agent gui
	ofxToggle agentToggleSprite;
	ofxFloatSlider agentLifespan;
	ofxFloatSlider spawnRate;
	ofxFloatSlider agentScale;
	ofxFloatSlider agentRotationSpeed = 3.0;
	ofxIntSlider nAgents = 1;
	ofxFloatSlider agentSpeed = 1.0;

	//player gui
	ofxFloatSlider playerScale;
	ofxFloatSlider playerRotSpeed = 5.0f;
	ofxToggle toggleHeadingVector;
	ofxFloatSlider playerSpeed = 10.0f;
	ofxFloatSlider playerEnergy = 10.0f;
	ofxToggle playerToggleSprite;

	ofColor textColor;

	bool gameState;
	bool gameOver;
	float endTime;
	float startTime;
	int nEnergy;
	bool playerSprite;
	const int maxAgents = 5;

	Emitter emitter;
	Sprite player;

	std::set<int> keysPressed;

	ofImage background;
	ofImage coconut;
	ofImage turtle;
	ofImage particle;

	ofSoundPlayer munch;
};
