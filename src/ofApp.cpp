#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);

	bHide = false;
	gameState = false;
	textColor = ofColor::white;

	background.load("images/cs134_proj2_background.png");
	coconut.load("images/cs134_proj2_agent.png");
	turtle.load("images/cs134_proj2_player.png");
	particle.load("images/cs134_proj2_particle.png");

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
	emitter.pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
	emitter.drawable = false;
	emitter.spriteScale = 0.5;
	emitter.setRate(spawnRate);
	emitter.setLifespan(agentLifespan * 1000);
	emitter.nAgents = nAgents;
	emitter.setChildImage(coconut);
	emitter.start();

	//player setup, default player sprite toggle, etc
	playerSprite = false;
	player.pos = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);
	player.color = ofColor::yellow;
	player.setImage(turtle);
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
		player.bShowImage = playerToggleSprite;
		player.scale = glm::vec3(float(playerScale), float(playerScale), float(playerScale));

		player.integrate();

		if (player.pos.x > ofGetWidth()) {
			player.pos.x = 0;
		}
		if (player.pos.x < 0) {
			player.pos.x = ofGetWidth();
		}
		if (player.pos.y > ofGetHeight()) {
			player.pos.y = 0;
		}
		if (player.pos.y < 0) {
			player.pos.y = ofGetHeight();
		}

		if (keysPressed.count(OF_KEY_LEFT)) {
			player.angularForce -= 300.0f;
		}
		if (keysPressed.count(OF_KEY_RIGHT)) {
			player.angularForce += 300.0f;
		}
		if (keysPressed.count(OF_KEY_UP)) {
			player.force += glm::vec3(glm::rotate(glm::mat4(1.0), glm::radians(player.rot), glm::vec3(0, 0, 1)) * glm::vec4(0, -1, 0, 1)) * 500;
		}
		if (keysPressed.count(OF_KEY_DOWN)) {
			player.force += glm::vec3(glm::rotate(glm::mat4(1.0), glm::radians(player.rot), glm::vec3(0, 0, 1)) * glm::vec4(0, 1, 0, 1)) * 500;
		}

		emitter.update();

		// Work backward so that removing does not cause skipping
		for (int i = emitter.sys->sprites.size() - 1; i > -1; i--) {
			emitter.sys->sprites[i].bShowImage = agentToggleSprite;

			emitter.sys->sprites[i].integrate();

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
			} else {
				for (int j = 0; j < emitter.sys->sprites[i].verts.size(); j++) {
					if (player.inside(emitter.sys->sprites[i].getTransform() * glm::vec4(emitter.sys->sprites[i].verts[j], 1))) {
						collision = true;
						break;
					}
				}

				for (int j = 0; j < player.verts.size(); j++) {
					if (emitter.sys->sprites[i].insideTriangle(player.getTransform() * glm::vec4(player.verts[j], 1))) {
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
			glm::vec3 headingVec = glm::vec3(glm::rotate(glm::mat4(1.0), glm::radians(emitter.sys->sprites[i].rot), glm::vec3(0, 0, 1)) * glm::vec4(0, -1, 0, 1));
			glm::vec3 toPlayerVec = glm::normalize(player.pos - emitter.sys->sprites[i].pos);
			emitter.sys->sprites[i].force += headingVec * 200;
			float angle = glm::degrees(glm::orientedAngle(headingVec, toPlayerVec, glm::vec3(0, 0, 1)));
			if (angle < 0.01)
				emitter.sys->sprites[i].angularForce -= 250;
			else
				emitter.sys->sprites[i].angularForce += 250;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(textColor);
	float scale = max(ofGetWidth() / background.getWidth(), ofGetHeight() / background.getHeight());
	float width = background.getWidth() * scale;
	float height = background.getHeight() * scale;
	background.draw((ofGetWidth() - width) / 2, (ofGetHeight() - height) / 2, width, height);

	// Draw depending on gameState (started, stopped)
	if (gameState) {
		endTime = ofGetElapsedTimef();
		ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 10, 20);
		ofDrawBitmapString("Energy: " + ofToString(nEnergy) + "/" + ofToString(10), 10, 40);
		ofDrawBitmapString("Time: " + ofToString(endTime - startTime), 10, 60);

		player.draw();
		emitter.draw();
	} else {
		ofBitmapFont font = ofBitmapFont();
		string text = (nEnergy > 0) ? "Press Space to Start" : "Last Record: " + ofToString(endTime - startTime) + "\nPress Space to Start";
		int width = font.getBoundingBox(text, 0, 0).getWidth();
		int height = font.getBoundingBox(text, 0, 0).getHeight();
		ofDrawBitmapString(text, ofGetWidth() / 2 - width / 2, ofGetHeight() / 2 - height / 2);
	}

	// Draw gui
	if (!bHide)
		gui.draw();
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
