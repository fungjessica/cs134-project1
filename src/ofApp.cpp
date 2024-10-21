#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	bHide = false;
	gameState = false;
	textColor = ofColor::white;
	
	nEnergy = 10;

	gui.setup();
	gui.add(complexityLevel.setup("Complexity Level", 1, 1, 3));
	gui.setPosition(10, ofGetHeight() - gui.getHeight() - 10);

	emitter = Emitter();
	emitter.pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
	emitter.drawable = false;
	emitter.spriteScale = 0.5;
	emitter.start();
}

//--------------------------------------------------------------
void ofApp::update(){
	if (gameState) {
		// Handle complexity level
		switch (complexityLevel) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		}

		// Write player methods here
		
		// Boilerplate code for checking collisions (for images only rn)
		// Work backward so that removing does not cause skipping
		for (int i = emitter.sys->sprites.size() - 1; i > -1; i--) {
			// Check for collisions
			bool collision = false;

			if (emitter.sys->sprites[i].bShowImage) {
				for (int j = 0; j < player.verts.size(); j++) {
					glm::vec3 v = player.getTransform() * glm::vec4(player.verts[j], 1);

					if (emitter.sys->sprites[i].inside(v)) {
						collision = true;
						break;
					}
				}
			}

			if (collision) {
				emitter.sys->remove(i);
				continue;
			}

			// Write agent updates down here (motion, etc.)
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Draw gui
	if (!bHide)
		gui.draw();

	// Draw depending on gameState (started, stopped)
	if (gameState) {
		endTime = ofGetElapsedTimef();
		ofSetColor(textColor);
		ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 10, 20);
		ofDrawBitmapString("Energy: " + ofToString(nEnergy) + "/" + ofToString(10), 10, 40);
		ofDrawBitmapString("Time: " + ofToString(endTime - startTime), 10, 60);
	} else {
		ofBitmapFont font = ofBitmapFont();
		string text = (nEnergy > 0) ? "Press Space to Start" : "Last Record: " + ofToString(endTime - startTime) + "\nPress Space to Start";
		int width = font.getBoundingBox(text, 0, 0).getWidth();
		int height = font.getBoundingBox(text, 0, 0).getHeight();
		ofSetColor(textColor);
		ofDrawBitmapString(text, ofGetWidth() / 2 - width / 2, ofGetHeight() / 2 - height / 2);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 'h':
		bHide = !bHide;
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case ' ':
		if (gameState == false) {
			gameState = true;
			startTime = ofGetElapsedTimef();
		}
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
