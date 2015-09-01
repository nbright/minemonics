//# corresponding headers
#include <model/universe/evolution/population/creature/phenome/PhenomeModel.hpp>

//# forward declarations
//# system headers
#include <list>
#include <map>
#include <vector>

//## controller headers
//## model headers
#include <BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h>
#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#include <BulletDynamics/Featherstone/btMultiBodyLinkCollider.h>
#include <BulletDynamics/Featherstone/btMultiBodyLink.h>
#include <BulletDynamics/Featherstone/btMultiBodyConstraint.h>
#include <BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.h>
#include <BulletDynamics/Featherstone/btMultiBodyJointMotor.h>
#include <BulletDynamics/Featherstone/btMultiBodyPoint2Point.h>
#include <OgreQuaternion.h>

//## view headers
//# custom headers
//## base headers
#include <SimulationManager.hpp>

//## configuration headers
#include <configuration/MorphologyConfiguration.hpp>

//## controller headers
//## model headers
#include <model/universe/PlanetModel.hpp>
#include <model/universe/environments/EnvironmentModel.hpp>
#include <model/universe/environments/physics/PhysicsController.hpp>
#include <model/universe/evolution/population/PopulationModel.hpp>
#include <model/universe/evolution/population/creature/phenome/morphology/limb/LimbBt.hpp>
#include <model/universe/evolution/population/creature/CreatureModel.hpp>
#include <model/universe/evolution/population/creature/genome/genetics/embryogenesis/BaseGenerator.hpp>
#include <model/universe/evolution/population/creature/genome/genetics/embryogenesis/Embryogenesis.hpp>
#include <model/universe/evolution/population/creature/genome/genetics/embryogenesis/PhenotypeGenerator.hpp>
#include <model/universe/evolution/population/creature/genome/Gene.hpp>
#include <model/universe/evolution/population/creature/genome/Genome.hpp>
#include <model/universe/evolution/population/creature/phenome/ComponentModel.hpp>
#include <model/universe/evolution/population/creature/phenome/PhenomeModel.hpp>
#include <model/universe/evolution/population/creature/phenome/morphology/effector/motor/ServoMotor.hpp>

//## view headers
//## utils headers

BoostLogger PhenomeModel::mBoostLogger; /*<! initialize the boost logger*/
PhenomeModel::_Init PhenomeModel::_initializer;
PhenomeModel::PhenomeModel() :
	mCreatureModel(NULL), mInWorld(false), mDeveloped(false), mHasInterpenetrations(
		true), mMultiBody(NULL), mWorld(NULL) {
	mControllers.clear();
}

PhenomeModel::PhenomeModel(const PhenomeModel& phenomeModel) {
	mInWorld = phenomeModel.mInWorld;
	mCreatureModel = phenomeModel.mCreatureModel;
	mDeveloped = phenomeModel.mDeveloped;
	mHasInterpenetrations = phenomeModel.mHasInterpenetrations;
	mMultiBody = NULL;
	mWorld = phenomeModel.mWorld;
	for (std::vector<Controller*>::const_iterator cit =
		phenomeModel.mControllers.begin();
		cit != phenomeModel.mControllers.end(); cit++) {
		mControllers.push_back((*cit)->clone());
	}

	for (std::vector<ComponentModel*>::const_iterator coit =
		phenomeModel.mComponentModels.begin();
		coit != phenomeModel.mComponentModels.end(); coit++) {
		ComponentModel* componentModel = (*coit)->clone();
		mComponentModels.push_back(componentModel);
		switch (componentModel->getComponentType()) {
		case ComponentModel::LimbComponent:
			mLimbModels.push_back((LimbModel*) componentModel);
			break;
		case ComponentModel::JointComponent:
			mJointModels.push_back((JointModel*) componentModel);
			break;

		}
	}
}

PhenomeModel::~PhenomeModel() {
	mControllers.clear();
}

void PhenomeModel::initialize(CreatureModel* const creatureModel) {
	mCreatureModel = creatureModel;
#ifndef EXCLUDE_FROM_TEST
	mWorld =
		(btMultiBodyDynamicsWorld*) mCreatureModel->getPopulationModel()->getPlanetModel()->getEnvironmentModel()->getPhysicsController()->getDynamicsWorld();
#endif
}

void PhenomeModel::update(const double timeSinceLastTick) {
	//update all controllers
	//TODO: Hacks to make it run, make nicer
	// let the controller perform
	for (std::vector<Controller*>::iterator cit = mControllers.begin();
		cit != mControllers.end(); cit++) {
		(*cit)->perform(timeSinceLastTick);
	}

	// Update all limb models
	mHasInterpenetrations = false;
	for (std::vector<LimbModel*>::iterator lit = mLimbModels.begin();
		lit != mLimbModels.end(); lit++) {

		//detect interpenetrations
		if ((*lit)->getInterpenetrationDepth() < 0 && !mHasInterpenetrations) {

//			std::cout << "Limb interpenetration depth: "
//					<< (*lit)->getInterpenetrationDepth()
//					<< std::endl;
			mHasInterpenetrations = true;
			break;
		}
		(*lit)->setInterpenetrationDepth(0);
	}

}

void PhenomeModel::addToWorld() {
	if (!isInWorld()) {
		if (mMultiBody != NULL) {
			mWorld->addMultiBody(mMultiBody);
		}
		setInWorld(true);
	}
}

void PhenomeModel::removeFromWorld() {
	if (isInWorld()) {
		if (mMultiBody != NULL) {
			mWorld->removeMultiBody(mMultiBody);
		}
		setInWorld(false);
	}
}

void PhenomeModel::calm() {
	for (std::vector<LimbModel*>::iterator lit = mLimbModels.begin();
		lit != mLimbModels.end(); lit++) {
		(*lit)->calm();
	}
}

int PhenomeModel::performEmbryogenesis(CreatureModel* const creatureModel) {
	int totalSegmentCounter = 0;
	if (!mDeveloped) {
		cleanup();
		std::list<PhenotypeGenerator*> generatorList;

		mCreatureModel = creatureModel;

		// get the first gene from the genome
		Gene* gene = mCreatureModel->getGenotype().getGenes().front();

		//create a phenotype generator and initialize it with the starting point of the creation of the creature
		PhenotypeGenerator* rootGenerator = new PhenotypeGenerator();
		std::map<int, int> repList;
		rootGenerator->initialize(repList, creatureModel->getInitialPosition(),
			Ogre::Quaternion().IDENTITY, NULL, NULL, 1);
		rootGenerator->setGene(gene);
		rootGenerator->setRoot2LeafPath(0);
		generatorList.push_back(rootGenerator);

		// this loop creates the creature up to the point at which we reach the correct root-to-leaf path length
		while (!generatorList.empty()) {

			BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info)
				<< "Phenome generator qty:" << generatorList.size();

			PhenotypeGenerator* generator = generatorList.front();
			generatorList.pop_front();

			Embryogenesis::transcribeGene(generatorList, totalSegmentCounter,
				this, generator);

			// delete the generator of this gene
			delete generator;
		}

		generateBody();
		//TODO: the joint limit constraint does not yet support the limiting of the spherical joint
		addJointConstraints();

		mDeveloped = true;
	}
	return totalSegmentCounter;
}

void PhenomeModel::generateBody() {
	bool isFixedBase = false;
	bool isMultiDof = true;
	bool setDamping = true;
	bool gyro = true;
	bool multibodyOnly = false;
	bool canSleep = true;

	if (mJointModels.size() != 0) {
		bool selfCollision = false;	//mLimbModels[0]->isIntraBodyColliding();

		mMultiBody = new btMultiBody(mJointModels.size(),
			mLimbModels[0]->getMass(), mLimbModels[0]->getInertia(),
			isFixedBase, canSleep, isMultiDof);

		mMultiBody->setBasePos(
			OgreBulletUtils::convert(mLimbModels[0]->getPosition()));
		mMultiBody->setWorldToBaseRot(
			OgreBulletUtils::convert(mLimbModels[0]->getOrientation()));

		for (int i = 0; i < mJointModels.size(); i++) {
			switch (mJointModels[i]->getType()) {
			case JointPhysics::HINGE_JOINT:
				mMultiBody->setupRevolute(
					((long) mJointModels[i]->getChildIndex()) - 1,
					mLimbModels[i + 1]->getMass(),
					mLimbModels[i + 1]->getInertia(),
					((long) mJointModels[i]->getParentIndex()) - 1,
					mJointModels[i]->getParentComToPivot().getRotation().normalized()
						* mJointModels[i]->getPivotToChildCom().getRotation().normalized(),
					mJointModels[i]->getJointPitchAxis().normalized(),
					mJointModels[i]->getParentComToPivot().getOrigin()
						* MorphologyConfiguration::LINK_LENGTH,
					-mJointModels[i]->getPivotToChildCom().getOrigin()
						* MorphologyConfiguration::LINK_LENGTH, true);
				break;
			case JointPhysics::SPHERICAL_JOINT:
				mMultiBody->setupSpherical(
					((long) mJointModels[i]->getChildIndex()) - 1,
					mLimbModels[i + 1]->getMass(),
					mLimbModels[i + 1]->getInertia(),
					((long) mJointModels[i]->getParentIndex()) - 1,
					mJointModels[i]->getParentComToPivot().getRotation().normalized()
						* mJointModels[i]->getPivotToChildCom().getRotation().normalized(),
					mJointModels[i]->getParentComToPivot().getOrigin()
						* MorphologyConfiguration::LINK_LENGTH,
					-mJointModels[i]->getPivotToChildCom().getOrigin()
						* MorphologyConfiguration::LINK_LENGTH, true);
				break;
			default:
				break;
			}

			for (std::vector<Motor*>::iterator mit =
				mJointModels[i]->getMotors().begin();
				mit != mJointModels[i]->getMotors().end(); mit++) {
				((ServoMotor*) (*mit))->instantiate(mMultiBody,
					mJointModels[i]->getIndex());
			}
		}

		mMultiBody->finalizeMultiDof();

		mMultiBody->setCanSleep(canSleep);
		mMultiBody->setHasSelfCollision(selfCollision);
		mMultiBody->setUseGyroTerm(gyro);
		//
		if (!setDamping) {
			mMultiBody->setLinearDamping(0.f);
			mMultiBody->setAngularDamping(0.f);
		} else {
			mMultiBody->setLinearDamping(0.1f);
			mMultiBody->setAngularDamping(0.9f);
		}

		btAlignedObjectArray < btQuaternion > worldtoLocal;
		worldtoLocal.resize(mMultiBody->getNumLinks() + 1);

		btAlignedObjectArray < btVector3 > localOrigin;
		localOrigin.resize(mMultiBody->getNumLinks() + 1);

		worldtoLocal[0] = mMultiBody->getWorldToBaseRot();
		localOrigin[0] = mMultiBody->getBasePos();
		for (int i = 0; i < mMultiBody->getNumLinks(); ++i) {
			const int parent = mMultiBody->getParent(i);
			worldtoLocal[i + 1] = mMultiBody->getParentToLocalRot(i)
				* worldtoLocal[parent + 1];
			localOrigin[i + 1] = localOrigin[parent + 1]
				+ (quatRotate(worldtoLocal[i + 1].inverse(),
					mMultiBody->getRVector(i)));
		}

		{
			btVector3 origin = localOrigin[0];

			btQuaternion orientation(-worldtoLocal[0].x(), -worldtoLocal[0].y(),
				-worldtoLocal[0].z(), worldtoLocal[0].w());

			mLimbModels[0]->generateLink(mMultiBody, origin, orientation, -1);
			mMultiBody->setBaseCollider(mLimbModels[0]->getLink());
		}

		for (int i = 0; i < mMultiBody->getNumLinks(); ++i) {

			btVector3 origin = localOrigin[i + 1];

			btQuaternion orientation(-worldtoLocal[i + 1].x(),
				-worldtoLocal[i + 1].y(), -worldtoLocal[i + 1].z(),
				worldtoLocal[i + 1].w());
			mLimbModels[i + 1]->generateLink(mMultiBody, origin, orientation,
				i);

			mMultiBody->getLink(i).m_collider = mLimbModels[i + 1]->getLink();
		}
	}
}

void PhenomeModel::addJointConstraints() {
	for (int i = 0; i < mJointModels.size(); i++) {
		//TODO: Limit joints that way, the joint limit constraint does not yet support the limiting of the spherical joint
		// Link joint limits
		btMultiBodyConstraint* limitCons = new btMultiBodyJointLimitConstraint(
			mMultiBody, i,
			mJointModels[i]->getLowerLimits()[JointPhysics::RDOF_PITCH],
			mJointModels[i]->getUpperLimits()[JointPhysics::RDOF_PITCH]);
//			// The default value (100) behaves like a lock on -1.6
//			limitCons->setMaxAppliedImpulse(40);
		mLimitConstraints.push_back(limitCons);
	}

}

void PhenomeModel::reset(const Ogre::Vector3 position) {
	/**The vector of limb models.*/
	for (std::vector<LimbModel*>::const_iterator it = mLimbModels.begin();
		it != mLimbModels.end(); it++) {
		(*it)->reset(position);
	}
}

void PhenomeModel::cleanup() {
	for (std::vector<ComponentModel*>::iterator cit = mComponentModels.begin();
		cit != mComponentModels.end();) {
		delete *cit;
		cit = mComponentModels.erase(cit);
	}
	mLimbModels.clear();
	mJointModels.clear();
}

void PhenomeModel::reposition(const Ogre::Vector3 position) {
	/**The vector of limb models.*/
	for (std::vector<LimbModel*>::const_iterator it = mLimbModels.begin();
		it != mLimbModels.end(); it++) {
		(*it)->reposition(position);

	}
}

bool PhenomeModel::equals(const PhenomeModel& phenomeModel) const {
	if (mInWorld != phenomeModel.mInWorld) {
		return false;
	}

	if (mDeveloped != phenomeModel.mDeveloped) {
		return false;
	}

	/**The vector of limb models.*/
	if (mLimbModels.size() != phenomeModel.mLimbModels.size()) {
		return false;
	}
	std::vector<LimbModel*>::const_iterator it = mLimbModels.begin();
	std::vector<LimbModel*>::const_iterator it2 =
		phenomeModel.mLimbModels.begin();
	for (; it != mLimbModels.end(), it2 != phenomeModel.mLimbModels.end();
		it++, it2++) {
		if (!(*it)->equals(**(it2))) {
			return false;
		}
	}

	/**The vector of joint models.*/
	if (mJointModels.size() != phenomeModel.mJointModels.size()) {
		return false;
	}
	std::vector<JointModel*>::const_iterator it3 = mJointModels.begin();
	std::vector<JointModel*>::const_iterator it4 =
		phenomeModel.mJointModels.begin();
	for (; it3 != mJointModels.end(), it4 != phenomeModel.mJointModels.end();
		it3++, it4++) {
		if (!(*it3)->equals(**(it4))) {
			return false;
		}
	}

	/**The vector of controllers.*/
	if (mControllers.size() != phenomeModel.mControllers.size()) {
		return false;
	}
	std::vector<Controller*>::const_iterator it5 = mControllers.begin();
	std::vector<Controller*>::const_iterator it6 =
		phenomeModel.mControllers.begin();
	for (; it5 != mControllers.end(), it6 != phenomeModel.mControllers.end();
		it5++, it6++) {
		if (!(*it5)->equals(**(it6))) {
			return false;
		}
	}
	return true;
}

PhenomeModel* PhenomeModel::clone() {
	return new PhenomeModel(*this);
}
