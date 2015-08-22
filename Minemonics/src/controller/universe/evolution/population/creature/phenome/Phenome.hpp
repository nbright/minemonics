#ifndef CONTROLLER_UNIVERSE_EVOLUTION_POPULATION_CREATURE_PHENOME_PHENOME_HPP_
#define CONTROLLER_UNIVERSE_EVOLUTION_POPULATION_CREATURE_PHENOME_PHENOME_HPP_

//# corresponding header
//# forward declarations
class SimulationManager;
class MixedGenome;
class Creature;

//# system headers
#include <vector>

//## controller headers
//## model headers
#include <bullet/BulletDynamics/ConstraintSolver/btHingeConstraint.h>
#include <OgreVector3.h>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/sources/basic_logger.hpp>

//## view headers
//# custom headers
//## base headers
//## configuration headers
//## controller headers
#include <model/universe/environments/physics/PhysicsController.hpp>
#include <controller/universe/evolution/population/creature/phenome/Component.hpp>
#include <controller/universe/evolution/population/creature/phenome/morphology/Joint.hpp>
#include <controller/universe/evolution/population/creature/phenome/morphology/Limb.hpp>

//## model headers
#include <model/universe/evolution/population/creature/genome/MixedGenome.hpp>
#include <model/universe/evolution/population/creature/genome/Gene.hpp>
#include <model/universe/evolution/population/creature/phenome/PhenomeModel.hpp>

//## view headers
//## utils headers

/**
 * @brief
 * 		The phenome controller synchronizes the graphical representation of the creature with
 * the physical model in the physics engine.
 * @details
 * 		The phenome controller synchronizes the graphical representation of the creature with
 * the physical model in the physics engine. It also acts as a facade for methods of the physical
 *  and graphical models. Each creature's body is composed of a number of cuboids called "body segments".
 * The segments are arranged in a tree structure with each segment connected to its parent by a
 * type of physical constraint known as a universal joint. Segments are divided into groups
 * called types, with the properties of each type encoded in the genotype. Each
 * cuboid is a rigid solid with uniform density. The density value is a variable.
 * The body tree of a creature is grown in a depth-first manner. The root segment is first
 * constructed, and then the entire subtree of its first enabled branch specification is
 * constructed, followed by the next enabled branch, and so on. This process is
 * recursive, so each subtree is constructed in the same manner. When a new segment
 * has been instantiated the physics engine is queried to obtain a list of all objects that
 * intersect the segment. A segment is permitted to intersect its own parent segment
 * since this is necessary anyway to allow for the full range of joint motion during
 * simulation. However, if a newly-instantiated segment is found to be intersecting some
 * other previously-instantiated part of the creature's body, other than its own parent
 * segment, the newly-instantiated segment will be deleted and none of its subtrees will
 * be constructed. This truncation of growth at intersecting branches ensures that a
 * newly-instantiated creature will not find itself suddenly experiencing the enormous
 * repulsion forces that would be generated by the physics engine attempting to resolve
 * and remove such intersections. It can also cause a disruption of symmetry in some
 * bilaterally-symmetric creatures or their limbs.
 *
 * The final creature body produced during embryogenesis is guaranteed to be free of
 * segment intersections, except those between segments directly connect by a joint. It
 * should be noted that in such cases the physics engine will not generate repulsion forces to
 * resolve the intersection. Segments connected by a common joint are permitted by the
 * physics engine to pass unimpeded through one another.
 * @date		2015-03-09
 * @author		Benjamin Ellenberger
 */
class Phenome {
public:
	Phenome();
	Phenome(const Phenome& phenome);

	virtual ~Phenome();

	/**
	 * @brief Initialize the phenotype with the simulation manager handle.
	 * @details Details
	 */
	void initialize(Creature* const creature);

	/**
	 * @brief Perform the generation of the creature embryo.
	 * @details Details
	 * @param creatureModel The creatureModel handle we want to get back from the physics engine when we pick the creature.
	 */
	int performEmbryogenesis(CreatureModel* const creatureModel);

	/**
	 * Reset the creature to the way it was born.
	 * @param position The position of the creature.
	 */
	void reset(const Ogre::Vector3 position);

	/**
	 * Reposition the creature without resetting it.
	 * @param position The position of the creature.
	 */
	void reposition(const Ogre::Vector3 position);

	/**
	 * Cleanup the phenome for a new embryogenesis.
	 */
	void cleanup();

	/**
	 * Clone the phenome.
	 */
	Phenome* clone();

	/**
	 * Update the graphical representation of the creature with its physical representation.
	 */
	void update(double timeSinceLastFrame);

	/**
	 * Add the phenotype to the physics world.
	 */
	void addToPhysicsWorld();

	/**
	 * Add the phenotype to the physical and graphical world.
	 */
	void addToWorld();

	/**
	 * Remove the phenotype from the physical and graphical world.
	 */
	void removeFromWorld();

	//Accessor methods
	/**
	 * Get the components the phenotype is built of.
	 * @return The components the phenotype is built of.
	 */
	std::vector<Component*>& getComponents() {
		return mComponents;
	}

	const std::vector<Component*>& getComponents() const {
		return mComponents;
	}

	/**
	 * Is the phenotype in the world?
	 * @return
	 */
	bool isInWorld() const {
		return mPhenotypeModel->isInWorld();
	}

	/**
	 * Set whether the phenotype is in the world or not.
	 * @param inWorld Whether the phenotype is in the world or not.
	 */
	void setInWorld(bool inWorld) {
		mPhenotypeModel->setInWorld(inWorld);
	}

	bool isDeveloped() const {
		return mPhenotypeModel->isDeveloped();
	}

	void setDeveloped(bool developed){
		mPhenotypeModel->setDeveloped(developed);
	}

	std::vector<Joint*>& getJoints() {
		return mJoints;
	}

	const std::vector<Joint*>& getJoints() const {
		return mJoints;
	}

	std::vector<Limb*>& getLimbs() {
		return mLimbs;
	}

	const std::vector<Limb*>& getLimbs() const {
		return mLimbs;
	}

	PhenomeModel* getPhenotypeModel() {
		return mPhenotypeModel;
	}

	const PhenomeModel* getPhenotypeModel() const {
		return mPhenotypeModel;
	}

	void setPhenotypeModel(PhenomeModel* const phenotypeModel) {
		mPhenotypeModel = phenotypeModel;
	}

private:
	/**
	 * The boost logger.
	 */
	static BoostLogger mBoostLogger;

	/**
	 * Initializer of the boost logger to include the class name into the logging messages.
	 */
	static class _Init {
	public:
		_Init() {
			mBoostLogger.add_attribute("ClassName",
					boost::log::attributes::constant<std::string>(
							"Phenome"));
		}
	} _initializer;

	// PARENT
	/**
	 * The creature this phenome belongs to.
	 */
	Creature* mCreature;

	// COMPONENTS
	/**
	 * The phenome model representation.
	 */
	PhenomeModel* mPhenotypeModel;

	/**
	 * The vector of phenotype components
	 */
	std::vector<Component*> mComponents;

	/**
	 * The vector of the phenotype's limbs
	 */
	std::vector<Limb*> mLimbs;

	/**
	 * The vector of the phenotype's joints between the limbs.
	 */
	std::vector<Joint*> mJoints;
};

#endif /* CONTROLLER_UNIVERSE_EVOLUTION_POPULATION_CREATURE_PHENOME_PHENOME_HPP_ */
