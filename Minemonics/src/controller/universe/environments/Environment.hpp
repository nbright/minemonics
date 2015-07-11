#ifndef CONTROLLER_UNIVERSE_ENVIRONEMTNS_ENVIRONMENT_H_
#define CONTROLLER_UNIVERSE_ENVIRONEMTNS_ENVIRONMENT_H_

//# corresponding header
//# forward declarations
class SimulationManager;
class EnvironmentGraphics;
class EnvironmentPhysics;
class EnvironmentModel;
class PhysicsController;
class EnvironmentO3D;

//# system headers
//## controller headers
//## model headers
#include <btBulletDynamicsCommon.h>

//## view headers
//# custom headers
//## base headers
//## configuration headers
//## controller headers
//## model headers
//## view headers
//## utils headers

/**
 * @brief		The environment controller is the base class for all environments.
 * @details		Details
 * @date		2014-11-17
 * @author		Benjamin Ellenberger
 */
class Environment {
public:
	enum EnvironmentType {
		UNKNOWN_ENVIRONMENT_TYPE = 0, HILLS = 1, PLANE = 2, OPENSEA = 3
	};

	EnvironmentType mEnvironmentType;

	Environment();
	virtual ~Environment();

	/**
	 * Initialize the environment.
	 * @param environmentType The type of environment.
	 */
	void initialize(EnvironmentType environmentType);

	/**
	 * Is the environment in the world?
	 * @return If the environment is in the world.
	 */
	bool isInWorld();

	/**
	 * Add the environment to the world.
	 */
	void addToWorld();

	/**
	 * Remove the environment from the world.
	 */
	void removeFromWorld();

	EnvironmentO3D* getEnvironmentO3D() {
		return (EnvironmentO3D*) mEnvironmentGraphics;
	}

	virtual EnvironmentModel* getEnvironmentModel() {
		return mEnvironmentModel;
	}

	virtual void update() = 0;

protected:
	/**
	 * The graphical representation of the environment.
	 */
	EnvironmentGraphics* mEnvironmentGraphics;

	/**
	 * The model representation of the environment.
	 */
	EnvironmentModel* mEnvironmentModel;

};

#endif /* CONTROLLER_UNIVERSE_ENVIRONEMTNS_ENVIRONMENT_H_ */
