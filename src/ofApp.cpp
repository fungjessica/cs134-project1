#include "ofApp.h"




//--------------------------------------------------------------
void ofApp::setup() {
	bHide = false;
	gameState = false;
	textColor = ofColor::white;

	nEnergy = 10;

	gui.setup();
	gui.add(complexityLevel.setup("Complexity Level", 1, 1, 3));
	gui.add(playerToggleSprite.setup("Player Sprite", false));
	gui.add(agentToggleSprite.setup("Agent Sprite", false));
	gui.add(agentLifespan.setup("Agent Lifespan", 5, 1, 10));
	gui.add(agentScale.setup("Agent Scale", 1, 1, 5));
	gui.add(spawnRate.setup("Spawn Rate", 1, 1, 10));
	gui.add(agentRotationSpeed.setup("Agent Rotation Speed", 3, 1, 10));
	gui.add(agentSpeed.setup("Agent Speed", 1, 1, 10));

	gui.add(playerScale.setup("Player Scale", 1, 1, 5));
	gui.add(playerRotSpeed.setup("Player Rotation Speed", 5, 1, 10));
	gui.add(playerSpeed.setup("Player Speed", 10, 1, 20));
	gui.add(playerEnergy.setup("Player Energy", 10, 1, 15));
	gui.add(toggleHeadingVector.setup("Toggle Heading Vector", false));

	gui.setPosition(10, ofGetHeight() - gui.getHeight() - 10);

	emitter = Emitter();
	//agent = Sprite();
	emitter.pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
	emitter.drawable = false;
	emitter.spriteScale = 0.5;


	//player setup, default player sprite toggle, etc
	playerSprite = false;
	player.pos = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);


	turtle.load("images/cs134_proj2_player.png");
	player.setImage(turtle);

	coconut.load("images/cs134_proj2_agent.png");
	emitter.setChildImage(coconut);
	emitter.start();

	background.load("images/cs134 proj2 background.png");

}

//--------------------------------------------------------------
void ofApp::update() {
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
		float screenWidth = ofGetScreenWidth();
		float screenHeight = ofGetScreenHeight();

		player.integrate();
		emitter.update();

		if (player.pos.x > screenWidth) {
			player.pos.x = 0;
		}
		if (player.pos.x < 0) {
			player.pos.x = screenWidth;
		}
		if (player.pos.y > screenHeight) {
			player.pos.y = 0;
		}
		if (player.pos.y < 0) {
			player.pos.y = screenHeight;
		}

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
void ofApp::draw() {
	ofSetColor(255, 255, 255);
	background.draw(0, 0, ofGetWidth(), ofGetHeight());

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


		//toggle between triangle and turtle
		if (playerToggleSprite) {
			player.bShowImage = true;
			ofSetColor(255, 255, 255);
			player.draw();
		}
		else {
			player.bShowImage = false;
			player.draw();
		}
		if (agentToggleSprite) {
			emitter.haveChildImage = true;
			emitter.setChildImage(coconut);
			emitter.draw();
		}
		else {
			emitter.haveChildImage = false;
			emitter.draw();
		}


	}
	else {
		ofBitmapFont font = ofBitmapFont();
		string text = (nEnergy > 0) ? "Press Space to Start" : "Last Record: " + ofToString(endTime - startTime) + "\nPress Space to Start";
		int width = font.getBoundingBox(text, 0, 0).getWidth();
		int height = font.getBoundingBox(text, 0, 0).getHeight();
		ofSetColor(textColor);
		ofDrawBitmapString(text, ofGetWidth() / 2 - width / 2, ofGetHeight() / 2 - height / 2);
	}


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	keysPressed.insert(key);

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

	if (keysPressed.count(OF_KEY_LEFT)) {
		player.angularForce -= 300.0f;
	}
	if (keysPressed.count(OF_KEY_RIGHT)) {
		player.angularForce += 300.0f;
	}
	if (keysPressed.count(OF_KEY_UP)) {
		float forceX = cos(glm::radians(player.rot)) * -500.0f;
		float forceY = sin(glm::radians(player.rot)) * -500.0f;
		player.force += glm::vec3(forceX, forceY, 0);
		player.integrate();
	}
	if (keysPressed.count(OF_KEY_DOWN)) {
		float forceX = cos(glm::radians(player.rot)) * 500.0f;
		float forceY = sin(glm::radians(player.rot)) * 500.0f;
		player.force += glm::vec3(forceX, forceY, 0);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	keysPressed.erase(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
