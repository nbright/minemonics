#ifndef CONTROLLER_UNIVERSE_EVOLUTION_POPULATION_CREATURE_PHENOME_MORPHOLOGY_LIMB_HPP_
#define CONTROLLER_UNIVERSE_EVOLUTION_POPULATION_CREATURE_PHENOME_MORPHOLOGY_LIMB_HPP_

//# corresponding headers
#include <configuration/Definitions.hpp>
#include <controller/universe/evolution/population/creature/phenome/Component.hpp>

//# forward declarations
class Creature;
class SimulationManager;

//# system headers
#include <string>

//## controller headers
//## model headers
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/sources/basic_logger.hpp>

//## view headers
#include <OgreColourValue.h>
#include <OgreQuaternion.h>

//# custom headers
//## base headers
//## configuration headers
//## controller headers
#include <controller/universe/evolution/population/creature/phenome/Component.hpp>

//## model headers
#include <model/universe/evolution/population/creature/phenome/morphology/limb/LimbPhysics.hpp>
#include <model/universe/evolution/population/creature/phenome/morphology/limb/LimbModel.hpp>

//## view headers
#include <view/universe/evolution/population/creature/phenome/morphology/limb/LimbGraphics.hpp>

//## utils headers
#include <utils/logging/Logger.hpp>

/**
 * @brief		The limb controller synchronizes the limb graphical representation with the limb model.
 * @details		Details
 * @date		2014-12-19
 * @author		Benjamin Ellenberger
 */
class Limb: public Component {
public:
	Limb();
	Limb(const Limb& limb);
	Limb(Creature* const creature, LimbModel* const limbModel);
	virtual ~Limb();

	/**
	 * Initialize the limb.
	 * @param creature A handle to the creature the limb belongs to.
	 * @param type The primitive type of the limb.
	 * @param position The position of the limb.
	 * @param orientation The orientation of the limb.
	 * @param size The size of the limb.
	 * @param mass The mass of the limb.
	 * @param restitution The restitution of the limb.
	 * @param friction The friction of the limb.
	 * @param color The color of the limb.
	 * @param ownIndex The limb's own index in the array of limbs.
	 */
	void initialize(Creature* const creature,
		const LimbPhysics::PrimitiveType type, const Ogre::Vector3 position,
		const Ogre::Quaternion orientation,
		const Ogre::Vector3 initialRelativePosition,
		const Ogre::Quaternion initialOrientation, const Ogre::Vector3 size,
		const double mass, const double restitution = 1.0,
		const double friction = 0.8, const Ogre::ColourValue color =
			Ogre::ColourValue(1, 1, 1), bool isIntraBodyColliding = true,
		const int ownIndex = 0);

	/**
	 * Build a limb from the limb model.
	 * @param simulationManager The handle of the simulationManager.
	 * @param creature The handle of the creature.
	 * @param limbModel The model of the limb.
	 */
	void buildFrom(SimulationManager* const simulationManager,
		Creature* const creature, LimbModel* const limbModel);

	/**
	 * Clone the limb.
	 * @return The clone of the limb.
	 */
	Limb* clone();

	/**
	 * Reset the creature to the way it was born.
	 */
	void reset(const Ogre::Vector3 position);

	/**
	 * Reposition the creature without resetting it.
	 */
	void reposition(const Ogre::Vector3 position);

	/**
	 * Update the graphical representation of the limb with the physical representation.
	 */
	void update(double timeSinceLastTick);

	/**
	 * Add the limb to the physics world.
	 */
	void addToPhysicsWorld();

	/**
	 * Add the limb to the graphical and the physical world.
	 */
	void addToWorld();

	/**
	 * Remove the limb from the graphical and the physical world.
	 */
	void removeFromWorld();

	// Accessor methods ##########################

	const Ogre::Vector3 getPosition() const;

	const Ogre::Quaternion getOrientation() const;

	/**
	 * Get the Graphics part of the limb.
	 */
	LimbGraphics* const getLimbGraphics() const;

	/**
	 * Get the Physics part of the limb.
	 */
	LimbPhysics* const getLimbPhysics() const;

	LimbModel* getLimbModel() {
		return mLimbModel;
	}

	const LimbModel* getLimbModel() const {
		return mLimbModel;
	}

private:

	static BoostLogger mBoostLogger; /**!< The boost logger. */

	/**
	 * Initializer of the boost logger to include the class name into the logging messages.
	 */
	static class _Init {
	public:
		_Init() {
			mBoostLogger.add_attribute("ClassName",
				boost::log::attributes::constant<std::string>("Limb"));
		}
	} _initializer;

	//PARENT
	Creature* mCreature; /**!< The creature the limb belongs to. */

	LimbGraphics* mLimbGraphics; /**!< Graphical representation of the limb. */

	LimbModel* mLimbModel; /**!< The model representation of the limb. */

};

#endif /* CONTROLLER_UNIVERSE_EVOLUTION_POPULATION_CREATURE_PHENOME_MORPHOLOGY_LIMB_HPP_ */
