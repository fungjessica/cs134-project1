#include "Particle.h"


Particle::Particle() {

	// initialize particle with some reasonable values first;
	//
	velocity.set(0, 0, 0);
	acceleration.set(0, 0, 0);
	position.set(0, 0, 0);
	forces.set(0, 0, 0);
	lifespan = 5;
	birthtime = 0;
	radius = .1;
	damping = .99;
	mass = 1;
	color = ofColor::aquamarine;
}

void Particle::draw() {
	//	ofSetColor(color);
	if (hasImage) {
		ofSetColor(ofColor::white);
		image.draw(glm::vec3(position.x - image.getWidth() / 2, position.y - image.getHeight() / 2, 0));
	}
	else {
		ofSetColor(ofMap(age(), 0, lifespan, 255, 10), 0, 0);
		ofDrawSphere(position, radius);
	}
}

void Particle::integrate() {
	// interval for this step
	//
	float dt = 1.0 / ofGetFrameRate();

	// update position based on velocity
	//
	position += (velocity * dt);

	// update acceleration with accumulated paritcles forces
	// remember :  (f = ma) OR (a = 1/m * f)
	//
	ofVec3f accel = acceleration;    // start with any acceleration already on the particle
	accel += (forces * (1.0 / mass));
	velocity += accel * dt;

	// add a little damping for good measure
	//
	velocity *= damping;

	// clear forces on particle (they get re-added each step)
	//
	forces.set(0, 0, 0);
}

//  return age in seconds
//
float Particle::age() {
	return (ofGetElapsedTimeMillis() - birthtime) / 1000.0;
}

void Particle::setImage(ofImage image) {
	hasImage = true;
	this->image = image;
}

bool Particle::inside(glm::vec3 p) {
	if (hasImage) {
		glm::vec3 s = p - position;
		int w = image.getWidth();
		int h = image.getHeight();
		if (s.x > -w / 2 && s.x < w / 2 && s.y > -h / 2 && s.y < h / 2) {
			int x = s.x + w / 2;
			int y = s.y + h / 2;
			ofColor color = image.getColor(x, y);
			return (color.a != 0);   // check if color is opaque (not the transparent background)
		}
		else return false;
	}
	else {
		if (glm::length(p - position) < radius)
			return true;
	}
	return false;
}
