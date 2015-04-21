/*
 * Limb.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: leviathan
 */

//# corresponding header
#include "Limb.h"

//# forward declarations
//# system headers
//## controller headers
#include <btBulletDynamicsCommon.h>

//## model headers
//## view headers
#include <boost/lexical_cast.hpp>

//# custom headers
//## base headers
#include "SimulationManager.h"

//## configuration headers
#include "configuration/PhysicsConfiguration.h"
#include "configuration/MorphologyConfiguration.h"

//## controller headers
//## model headers
#include "model/evolution/population/creature/phenome/morphology/LimbBt.h"

//## view headers
#include "view/ogre3D/evolution/population/creature/phenome/morphology/LimbO3D.h"

//## utils headers

#ifndef NULL
#define NULL 0
#endif

Limb::Limb() :
		mLimbGraphics(NULL), mLimbPhysics(NULL) {
}

Limb::~Limb() {
	delete mLimbGraphics;
	delete mLimbPhysics;
}

void Limb::initialize(SimulationManager* simulationManager,
		MorphologyConfiguration::PrimitiveType type, Ogre::Vector3 position,
		Ogre::Quaternion orientation, Ogre::Vector3 size, double mass,Ogre::ColourValue color) {
	// Define the new component as a limb
	Component::initialize(Component::LimbComponent);

	// initialize the graphics part of the limb
	mLimbGraphics = new LimbO3D();
	((LimbO3D*) mLimbGraphics)->initialize(simulationManager, type, size,color);

	// initialize the physics model of the limb
	mLimbPhysics = new LimbBt();
	((LimbBt*) mLimbPhysics)->initialize(
			simulationManager->getPhysicsController().getDynamicsWorld(), type,
			btVector3(position.x, position.y, position.z),
			btQuaternion(orientation.x, orientation.y, orientation.z,
					orientation.w), btVector3(size.x, size.y, size.z),
			btScalar(mass));

	// Update the state of the limb.
	update();
}

/**
 * Update the state of the limb.
 */
void Limb::update() {
	// get the rigid body of the limb
	btRigidBody* body = ((LimbBt*) mLimbPhysics)->getRigidBody();

	// if the limb's rigid body is existing
	if (body) {

		// update the position of the limb graphics
		btVector3 point = body->getCenterOfMassPosition();
		mLimbGraphics->setPosition(
				Ogre::Vector3(point.x(), point.y(), point.z()));

		// Get the Orientation of the rigid body as a bullet Quaternion
		// Convert it to an Ogre quaternion
		btQuaternion btq = body->getOrientation();
		Ogre::Quaternion quart = Ogre::Quaternion(btq.w(), btq.x(), btq.y(),
				btq.z());

		// update the orientation of the limb graphics
		mLimbGraphics->setOrientation(quart);
	}

	mLimbGraphics->update();
}

std::string Limb::getInfo() {
	std::string text;
	text.append("Box coordinate: ");
	text.append(
			boost::lexical_cast<std::string>(mLimbGraphics->getPosition().x));
	text.append(",");
	text.append(
			boost::lexical_cast<std::string>(mLimbGraphics->getPosition().y));
	text.append(",");
	text.append(
			boost::lexical_cast<std::string>(mLimbGraphics->getPosition().z));
	text.append(",\n");
	text.append("---------");
	return text;
}

/**
 * Get the Graphics part of the limb.
 */
LimbO3D* Limb::getLimbGraphics() {
	return ((LimbO3D*) mLimbGraphics);

}

/**
 * Get the Physics part of the limb.
 */
LimbBt* Limb::getLimbPhysics() {
	return ((LimbBt*) mLimbPhysics);
}

/**
 * Add the limb to the world.
 */
void Limb::addToWorld() {
	mLimbGraphics->addToWorld();
	mLimbPhysics->addToWorld();
}

/**
 * Remove the limb from the world.
 */
void Limb::removeFromWorld() {
	mLimbGraphics->removeFromWorld();
	mLimbPhysics->removeFromWorld();
}

/**
 * Get intersection point with the limb graphics given a straight line defined by origin and direction.
 */
Ogre::Vector3 Limb::getIntersection(Ogre::Vector3 origin,
		Ogre::Vector3 direction) {
	return mLimbGraphics->getIntersection(origin, direction);
}

