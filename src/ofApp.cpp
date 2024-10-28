#include "ofApp.h"


void ofApp::setComplexityLevels(int level) {
	switch (level) {
	case 1:
		spawnRate = 1;
		agentSpeed = 1;
		agentLifespan = 7;
		nAgents = 1;
		break;
	case 2:
		spawnRate = 3;
		agentSpeed = 5;
		agentLifespan = 10;
		nAgents = 3;
		break;
	case 3:
		spawnRate = 5;
		agentSpeed = 10;
		agentLifespan = 20;
		nAgents = 5;
		break;
	}
}

void ofApp::resetExplosion() {
	explosion.sys->reset();

	explosion.setLifespan(deathPause);
	explosion.setVelocity(ofVec3f(0, 0, 0));

}
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);

	bHide = false;
	gameState = false;
	gameOver = false;
	textColor = ofColor::white;

	background.load("images/cs134 proj2 background.png");
	coconut.load("images/cs134_proj2_agent.png");
	turtle.load("images/cs134_proj2_player.png");
	beachBall.load("images/cs134_proj2_particle.png");

	//scale beach ball (particle) sprite
	float beachBallScale = 0.5;
	beachBall.resize(beachBall.getWidth() * beachBallScale, beachBall.getHeight() * beachBallScale);
	float coconutScale = 0.8;
	coconut.resize(coconut.getWidth() * coconutScale, coconut.getHeight() * coconutScale);

	munch.load("sounds/minecraft_eating.mp3");
	moving.load("sounds/swimming sfx.mp3");
	boom.load("sounds/pop sfx.mp3");
	moving.setLoop(false);

	nEnergy = 10;

	trackComplexity = complexityLevel;

	gui.setup();
	gui.add(complexityLevel.setup("Complexity Level", 1, 1, 3));
	gui.add(playerToggleSprite.setup("Player Sprite", false));
	gui.add(agentToggleSprite.setup("Agent Sprite", false));
	gui.add(agentLifespan.setup("Agent Lifespan", 5, 1, 20));
	gui.add(agentScale.setup("Agent Scale", 1, 1, 5));
	gui.add(spawnRate.setup("Spawn Rate", 1, 1, 5));
	gui.add(nAgents.setup("nAgents", 1, 1, 5));
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
	emitter.setParticleImage(beachBall);
	emitter.start();

	//player setup, default player sprite toggle, etc
	playerSprite = false;
	player.pos = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);
	player.color = ofColor::yellow;
	player.setImage(turtle);

	//particle explosion setup
	turbForce = new TurbulenceForce(ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20));
	gravityForce = new GravityForce(ofVec3f(0, -10, 0));
	radialForce = new ImpulseRadialForce(5000.0);

	//explosion.sys->addForce(turbForce);
	explosion.sys->addForce(gravityForce);
	explosion.sys->addForce(radialForce);

	explosion.setLifespan(deathPause);
	explosion.setParticleRadius(5);
	explosion.setVelocity(ofVec3f(0, 0, 0));
	explosion.setEmitterType(RadialEmitter);
	explosion.setGroupSize(20);
	explosion.setOneShot(true);
}

//--------------------------------------------------------------
void ofApp::update() {
	if (gameState && !gameOver) {

		explosion.update();

		if (nEnergy <= 0) {
			if (!isDead) {

				isDead = true;
				deathTimer = 0.0f;

				boom.setPosition(1);
				boom.play();
				explosion.pos = player.pos;
				explosion.start();

				resetExplosion();
			}
			else {
				deathTimer += ofGetLastFrameTime();

				if (deathTimer >= deathPause) {
					gameOver = true;
					gameState = false;
					isDead = false;

					// Reset for next iteration
					for (int i = 0; i < emitter.sys->sprites.size(); i++)
						emitter.sys->remove(i);
					player.pos = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);
					player.rot = 0;
				}
			}

		}
		else {
			isDead = false;
		}

		// Handle complexity level
		if (complexityLevel != trackComplexity) {
			setComplexityLevels(complexityLevel);
			trackComplexity = complexityLevel;
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
			player.angularForce -= playerRotSpeed * 70;
		}
		if (keysPressed.count(OF_KEY_RIGHT)) {

			player.angularForce += playerRotSpeed * 70;
		}
		if (keysPressed.count(OF_KEY_UP)) {

			player.force += glm::vec3(glm::rotate(glm::mat4(1.0), glm::radians(player.rot), glm::vec3(0, 0, 1)) * glm::vec4(0, -1, 0, 1)) * (float)playerSpeed * 70;
		}
		if (keysPressed.count(OF_KEY_DOWN)) {
			player.force += glm::vec3(glm::rotate(glm::mat4(1.0), glm::radians(player.rot), glm::vec3(0, 0, 1)) * glm::vec4(0, 1, 0, 1)) * (float)playerSpeed * 70;
		}

		if (isKeyHeld && !moving.isPlaying()) {
			moving.play();
		}
		else if (!isKeyHeld && stopTimer > 0.0f && ofGetElapsedTimef() >= stopTimer) {
			moving.stop();
			stopTimer = 0.0f;
		}

		int trackAgents = emitter.sys->sprites.size();
		if (trackAgents < nAgents) {
			emitter.update();
		}

		emitter.setRate(spawnRate);
		emitter.setLifespan(agentLifespan * 1000);


		// Work backward so that removing does not cause skipping
		for (int i = emitter.sys->sprites.size() - 1; i > -1; i--) {
			emitter.sys->sprites[i].scale = glm::vec3(float(agentScale));
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
			}
			else {
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

			if (collision && !isDead) {
				emitter.sys->remove(i);
				explosion.pos = player.pos;
				explosion.start();
				resetExplosion();
				munch.play();
				if (nEnergy < playerEnergy)
					nEnergy++;
				continue;
			}

			// Write agent updates down here (motion, etc.)
			glm::vec3 headingVec = glm::vec3(glm::rotate(glm::mat4(1.0), glm::radians(emitter.sys->sprites[i].rot), glm::vec3(0, 0, 1)) * glm::vec4(0, -1, 0, 1));
			glm::vec3 toPlayerVec = glm::normalize(player.pos - emitter.sys->sprites[i].pos);
			emitter.sys->sprites[i].force += headingVec * (float)agentSpeed * 100;

			float angle = glm::degrees(glm::orientedAngle(headingVec, toPlayerVec, glm::vec3(0, 0, 1)));
			float scalar = pow(2, fmod(abs(angle), 360) / 360) - 1;
			if (angle < -0.01)
				emitter.sys->sprites[i].angularForce -= 1500 * agentSpeed * scalar;
			else if (angle > 0.01)
				emitter.sys->sprites[i].angularForce += 1500 * agentSpeed * scalar;

			emitter.sys->sprites[i].particleEmitter.pos = emitter.sys->sprites[i].pos;
			emitter.sys->sprites[i].particleEmitter.setVelocity(headingVec * 750);
			emitter.sys->sprites[i].particleEmitter.update();

			// Update emitter particles
			for (int j = emitter.sys->sprites[i].particleEmitter.sys->particles.size() - 1; j > -1; j--) {
				emitter.sys->sprites[i].particleEmitter.sys->particles[j].hasImage = agentToggleSprite;

				bool particleCollision = false;

				for (int k = 0; k < player.verts.size(); k++) {
					glm::vec3 v = player.getTransform() * glm::vec4(player.verts[k], 1);

					if (emitter.sys->sprites[i].particleEmitter.sys->particles[j].inside(v)) {
						particleCollision = true;
						break;
					}
				}

				if (!emitter.sys->sprites[i].particleEmitter.sys->particles[j].hasImage) {
					if (player.inside(emitter.sys->sprites[i].particleEmitter.sys->particles[j].position))
						particleCollision = true;
				}

				if (particleCollision && !isDead) {
					emitter.sys->sprites[i].particleEmitter.sys->particles.erase(emitter.sys->sprites[i].particleEmitter.sys->particles.begin() + j);
					if (nEnergy > 0)
						nEnergy--;
					continue;
				}
			}
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

		if (!isDead) {
			player.draw();
		}

		emitter.draw();
		explosion.draw();

		if (toggleHeadingVector) {
			glm::vec3 headingVecDirection = glm::vec3(glm::rotate(glm::mat4(1.0), glm::radians(player.rot), glm::vec3(0, 0, 1)) * glm::vec4(0, -1, 0, 1));
			ofSetColor(ofColor::red);

			if (playerToggleSprite) {
				float headingVecLength = 70;
				glm::vec3 headingVecPos = player.pos + headingVecDirection * headingVecLength;
				ofDrawLine(player.pos, headingVecPos);
			}
			else {
				float headingVecLength = 50;
				glm::vec3 headingVecPos = player.pos + headingVecDirection * headingVecLength;
				ofDrawLine(player.pos, headingVecPos);
			}
		}
	}
	else if (gameOver) {
		ofSetColor(ofColor::red);
		ofDrawBitmapString("Game Over", ofGetWidth() / 2 - 20, ofGetHeight() / 2);
		ofDrawBitmapString("Last Record: " + ofToString(endTime - startTime), ofGetWidth() / 2 - 70, ofGetHeight() / 2 + 20);
		ofDrawBitmapString("Press Space to Restart", ofGetWidth() / 2 - 80, ofGetHeight() / 2 + 40);
	}
	else {
		ofBitmapFont font = ofBitmapFont();
		string text = "Press Space to Start";
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
		if (emitter.sys == nullptr) {
			cout << "emitter particle system is null" << endl;
			return;
		}
		if (gameOver) {
			resetExplosion();
			nEnergy = 10;
			gameOver = false;
			gameState = false;
			complexityLevel = 1;
			playerToggleSprite = false;
			agentToggleSprite = false;
			startTime = ofGetElapsedTimef();
			playerScale = 1;
		}
		else if (gameState == false) {
			gameState = true;
			startTime = ofGetElapsedTimef();
		}
		break;
	}

	if (key == OF_KEY_UP || key == OF_KEY_DOWN || key == OF_KEY_LEFT || key == OF_KEY_RIGHT) {
		if (!isKeyHeld) {
			isKeyHeld = true;
			stopTimer = 0.0f;
			moving.play();

		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	keysPressed.erase(key);

	if (key == OF_KEY_UP || key == OF_KEY_DOWN || key == OF_KEY_LEFT || key == OF_KEY_RIGHT) {
		isKeyHeld = false;
		stopTimer = ofGetElapsedTimef() + 2.0f;
		moving.stop();

	}
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
