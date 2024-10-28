
//  Kevin M. Smith - CS 134 SJSU

#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() {
	sys = new ParticleSystem();
	createdSys = true;
	init();
}

ParticleEmitter::ParticleEmitter(ParticleSystem* s) {
	if (s == nullptr)
	{
		cout << "fatal error: null particle system passed to ParticleEmitter()" << endl;
		ofExit();
	}
	sys = s;
	createdSys = false;
	init();
}

ParticleEmitter::~ParticleEmitter() {

	// deallocate particle system if emitter created one internally
	//
	// This causes an error
	/*if (sys == nullptr) {
		cout << "Error: Particle system is null" << endl;
		return; 
	}

	if (createdSys && sys != nullptr) {
		delete sys;
		sys = nullptr;
	}*/
}

void ParticleEmitter::init() {
	rate = 1;
	velocity = ofVec3f(0, 20, 0);
	lifespan = 3;
	started = false;
	oneShot = false;
	fired = false;
	lastSpawned = 0;
	radius = 1;
	particleRadius = .1;
	visible = true;
	type = DirectionalEmitter;
	groupSize = 1;
}



void ParticleEmitter::draw() {
	if (!visible) return;

	if (sys != nullptr) {
		switch (type) {
		case DirectionalEmitter:
			ofDrawSphere(pos, radius / 10);  // just draw a small sphere for point emitters 
			break;
		case SphereEmitter:
		case RadialEmitter:
			ofDrawSphere(pos, radius / 10);  // just draw a small sphere as a placeholder
			break;
		default:
			break;
		}
		sys->draw();
	}
	else {
		cout << "particle system is null in draw()" << endl;
	}
	
}
void ParticleEmitter::start() {
	if (sys == nullptr) {
		cout << "particle system not initialized" << endl;
		return; 
	}
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void ParticleEmitter::stop() {
	started = false;
	fired = false;
}
void ParticleEmitter::update() {

	float time = ofGetElapsedTimeMillis();

	if (sys == nullptr) {
		cout << "particle system is null in update()" << endl;
		return;
	}
	if (oneShot && started) {
		if (!fired) {

			// spawn a new particle(s)
			//
			for (int i = 0; i < groupSize; i++)
				spawn(time);

			lastSpawned = time;
		}
		fired = true;
		stop();
	}

	else if (((time - lastSpawned) > (1000.0 / rate)) && started) {

		// spawn a new particle(s)
		//
		for (int i = 0; i < groupSize; i++)
			spawn(time);

		lastSpawned = time;
	}

	sys->update();
}

// spawn a single particle.  time is current time of birth
//
void ParticleEmitter::spawn(float time) {

	Particle particle;

	// set initial velocity and position
	// based on emitter type
	//
	switch (type) {
	case RadialEmitter:
	{
		ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
		float speed = velocity.length();
		particle.velocity = dir.getNormalized() * speed;
		particle.position.set(pos);
	}
	break;
	case SphereEmitter:
		break;
	case DirectionalEmitter:
		particle.velocity = velocity;
		particle.position.set(pos);
		break;
	}

	// other particle attributes
	//
	particle.lifespan = lifespan;
	particle.birthtime = time;
	particle.radius = particleRadius;

	if (hasParticleImage)
		particle.setImage(particleImage);

	// add to system
	//
	sys->add(particle);
}

void ParticleEmitter::setParticleImage(ofImage image) {
	hasParticleImage = true;
	particleImage = image;
}
