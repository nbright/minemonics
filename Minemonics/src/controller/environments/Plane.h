/*
 * Plane.h
 *
 *  Created on: Mar 24, 2015
 *      Author: leviathan
 */

#ifndef CONTROLLER_ENVIRONMENTS_PLANE_H_
#define CONTROLLER_ENVIRONMENTS_PLANE_H_

//# corresponding header
//# forward declarations
//# system headers
//## controller headers
//## model headers
//## view headers
#include <OgreLight.h>

//# custom headers
//## base headers
//## configuration headers
//## controller headers
#include "controller/environments/Environment.h"

//## model headers
//## view headers

class Plane: public Environment {
public:
	Plane();
	virtual ~Plane();

	void initialize(SimulationManager* simulationMgr, Ogre::Light* l);
};

#endif /* CONTROLLER_ENVIRONMENTS_PLANE_H_ */