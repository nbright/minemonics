//# corresponding headers
#include <model/universe/evolution/population/creature/phenome/SRBPhenomeModel.hpp>

#include <list>
#include <map>

//## controller headers
//## model headers
#include <OgreQuaternion.h>

//## view headers
//# custom headers
//## base headers
#include <SimulationManager.hpp>

//## configuration headers
//## controller headers
//## model headers
#include <model/universe/environments/EnvironmentModel.hpp>
#include <model/universe/environments/physics/PhysicsController.hpp>
#include <model/universe/evolution/population/creature/CreatureModel.hpp>
#include <model/universe/evolution/population/creature/genome/genetics/BaseGenerator.hpp>
#include <model/universe/evolution/population/creature/genome/genetics/PhenotypeGenerator.hpp>
#include <model/universe/evolution/population/creature/genome/genetics/Embryogenesis.hpp>
#include <model/universe/evolution/population/creature/genome/Gene.hpp>
#include <model/universe/evolution/population/creature/genome/Genome.hpp>
#include <model/universe/evolution/population/creature/phenome/ComponentModel.hpp>
#include <model/universe/evolution/population/PopulationModel.hpp>
#include <model/universe/PlanetModel.hpp>
#include <model/universe/evolution/population/creature/phenome/controller/sine/SineController.hpp>
#include <model/universe/evolution/population/creature/phenome/controller/chaotic/ChaoticController.hpp>

//## view headers
//## utils headers
#include <utils/ogre3D/Euler.hpp>

BoostLogger SRBPhenomeModel::mBoostLogger; /*<! initialize the boost logger*/
SRBPhenomeModel::_Init SRBPhenomeModel::_initializer;
SRBPhenomeModel::SRBPhenomeModel() :
	mWorld(NULL) {
}

SRBPhenomeModel::SRBPhenomeModel(CreatureModel* const creatureModel) {
	mCreatureModel = creatureModel;
	mWorld = creatureModel->getWorld();
}

SRBPhenomeModel::SRBPhenomeModel(const SRBPhenomeModel& SRBPhenomeModel) {
	mInWorld = SRBPhenomeModel.mInWorld;
	mCreatureModel = SRBPhenomeModel.mCreatureModel;
	mDeveloped = SRBPhenomeModel.mDeveloped;
	mHasInterpenetrations = SRBPhenomeModel.mHasInterpenetrations;

	mWorld = SRBPhenomeModel.mWorld;

	for (std::vector<Controller*>::const_iterator cit =
		SRBPhenomeModel.mControllers.begin();
		cit != SRBPhenomeModel.mControllers.end(); cit++) {
		mControllers.push_back((*cit)->clone());
	}

	for (std::vector<ComponentModel*>::const_iterator coit =
		SRBPhenomeModel.mComponentModels.begin();
		coit != SRBPhenomeModel.mComponentModels.end(); coit++) {
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

SRBPhenomeModel::~SRBPhenomeModel() {
	mWorld = NULL;
}

void SRBPhenomeModel::initialize() {

	performEmbryogenesis(); /**!< Perform embryogenesis to build a body plan*/

	//initialize the limb models if it did not already happen in embryogenesis
	for (std::vector<LimbModel*>::iterator lit = mLimbModels.begin();
		lit != mLimbModels.end(); lit++) {
		(*lit)->setWorld(getWorld());
		(*lit)->initialize();
	}

	generateBody(); /**!< Build the body from the body plan */
}

void SRBPhenomeModel::update(const double timeSinceLastTick) {
	//update all controllers
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

			mHasInterpenetrations = true;
			break;
		}
		(*lit)->setInterpenetrationDepth(0);
	}

}

void SRBPhenomeModel::calm() {
	for (std::vector<LimbModel*>::iterator lit = mLimbModels.begin();
		lit != mLimbModels.end(); lit++) {
		(*lit)->calm();
	}
}

int SRBPhenomeModel::performEmbryogenesis() {
	int totalSegmentCounter = 0;
	if (!mDeveloped) {
		BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info)<< "--Perform an embryogenesis";
		cleanup();
		mBodyGenerated = false;
		std::list<PhenotypeGenerator*> generatorList;

		// get the first gene from the genome
		Gene* gene =
		mCreatureModel->getGenotype().getGenes()[mCreatureModel->getGenotype().getRootIndex()];

		//create a phenotype generator and initialize it with the starting point of the creation of the creature
		PhenotypeGenerator* rootGenerator = new PhenotypeGenerator();
		std::map<int, int> repList;
		rootGenerator->initialize(repList, mCreatureModel->getInitialPosition(),
			Ogre::Quaternion().IDENTITY, NULL, NULL, 1);
		rootGenerator->setGene(gene);
		rootGenerator->setRoot2LeafPath(0);
		generatorList.push_back(rootGenerator);

		while (!generatorList.empty()) { // this loop creates the creature up to the point at which we reach the correct root-to-leaf path length

			BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info)<< "Phenome generator qty:" << generatorList.size();

			PhenotypeGenerator* generator = generatorList.front();
			generatorList.pop_front();

			Embryogenesis::transcribeGene(generatorList, totalSegmentCounter,
				this, generator);

			delete generator;// delete the generator of this gene
		}

		mDeveloped = true;
	} else {
		mComponentModels.clear();
		for (std::vector<LimbModel*>::const_iterator it = mLimbModels.begin();
			it != mLimbModels.end(); it++) {
			mComponentModels.push_back((*it));
		}

		for (std::vector<JointModel*>::const_iterator it = mJointModels.begin();
			it != mJointModels.end(); it++) {
			mComponentModels.push_back((*it));
		}
	}
	return totalSegmentCounter;
}

void SRBPhenomeModel::calculateChildPositionRelativeToParent(
	PhenotypeGenerator* generator, /**!< The generator of the child limb*/
	btTransform& parentJointAnchor,/**!< The joint anchor on the parent limb surface*/
	btTransform& childJointAnchor,/**!< joint anchor on the child limb surface*/
	Morphogene* childMorphogene,/**!< The morphogene to build the child limb*/
	Ogre::Vector3& localParentJointInRefParent,/**!< The joint location in the parent limb ref.*/
	Ogre::Vector3& localChildJointInRefChild /**!< The joint location in the child limb ref. */) {

	//PARENT
	//get the morphogene branch that defines the joint and connects the limbs
	MorphogeneBranch* parentMorphogeneBranch =
		((MorphogeneBranch*) generator->getGeneBranch());

	// get parent limb
	SRBLimbBt* parentLimb =
		((SRBLimbBt*) ((SRBLimbModel*) generator->getParentComponentModel())->getLimbPhysics());

	//get the parent limb's center of mass position
	Ogre::Vector3 parentLimbCOM =
		((LimbModel*) generator->getParentComponentModel())->getPosition();

	Ogre::Vector3 localParentAnchorDirInRefParent = Ogre::Vector3(
		parentMorphogeneBranch->getJointAnchorX(),
		parentMorphogeneBranch->getJointAnchorY(),
		parentMorphogeneBranch->getJointAnchorZ());

	//if the generator is the mirrored version of another generator
	//mirrored is: -O  -> O- | \O -> O\   mirroring = sign inversion
	if (generator->isMirrored()) {
		localParentAnchorDirInRefParent = -localParentAnchorDirInRefParent; //get mirrored direction
	}

	//if the generator is the flipped version of another generator
	//flipped is: \O -> O/ and the flipping axis is the parent limb direction
	if (generator->isFlipped()) {
		Ogre::Vector3 parentLimbDir(1, 0, 0); // get direction vector of parent limb
		parentLimbDir = OgreBulletUtils::convert(parentLimb->getOrientation())
			* parentLimbDir;

		//reflect on the direction vector
		localParentAnchorDirInRefParent = -localParentAnchorDirInRefParent
			- 2 * ((-localParentAnchorDirInRefParent).dotProduct(parentLimbDir))
				* parentLimbDir;
	}

	// ##
	// PARENT ANCHOR DIRECTION
	//get anchor direction of the parent limb in reference frame of itself
	parentJointAnchor = getParentIntersection(parentLimb,
		parentMorphogeneBranch, parentLimbCOM, localParentAnchorDirInRefParent);

	//get surface point of the parent limb in reference frame of itself
	Ogre::Vector3 localParentAnchorInRefParent(
		OgreBulletUtils::convert(parentJointAnchor.getOrigin()));

	//##
	// PARENT JOINT POSITION
	// joint direction of joint part of parent
	Ogre::Euler parentEulerJointDir(parentMorphogeneBranch->getJointYaw(),
		parentMorphogeneBranch->getJointPitch(),
		parentMorphogeneBranch->getJointRoll());

	generator->setOrientation(
		OgreBulletUtils::convert(parentJointAnchor.getRotation()) /**!< The parent surface direction */
			* parentEulerJointDir.toQuaternion() /**!< The parent joint direction change */);

	//get local joint rotation point in reference frame parent
	localParentJointInRefParent = localParentAnchorInRefParent; /**!< The direction of the surface*/

	//##
	// CHILD LIMB ANCHOR POINT IN PARENT REFERENCE FRAME
	// get local joint direction in the local reference frame of child
	Ogre::Euler childEulerJointDir(childMorphogene->getJointYaw(),
		childMorphogene->getJointPitch(), childMorphogene->getJointRoll());

	generator->setOrientation(
		generator->getOrientation()
			* childEulerJointDir.toQuaternion() /**!< The child joint direction change */);

	//get local surface anchor point of child in reference frame parent
	Ogre::Vector3 localChildAnchorInRefParent(localParentJointInRefParent);

	// get anchor direction of limb child in the local reference frame of child
	Ogre::Vector3 localChildAnchorDirInRefChild(
		childMorphogene->getJointAnchorX(), childMorphogene->getJointAnchorY(),
		childMorphogene->getJointAnchorZ());

	//##
	// CHILD LIMB ANCHOR POINT IN CHILD REFERENCE FRAME
	// find the joint anchor position of the limb by positioning the limb at an arbitrary position to cast a ray
	childJointAnchor = getOwnIntersection(childMorphogene, generator,
		localChildAnchorDirInRefChild);

	generator->setOrientation(
		generator->getOrientation()
			* OgreBulletUtils::convert(
				childJointAnchor.getRotation()) /**!< The child joint surface direction */);

	//get the surface point of child limb in the local reference frame of itself
	Ogre::Vector3 localChildAnchorInRefChild(
		OgreBulletUtils::convert(childJointAnchor.getOrigin()));

	localChildJointInRefChild = localChildAnchorInRefChild;

	// global center of mass of child limb
	Ogre::Vector3 childLimbCOM(
		parentLimbCOM + localChildAnchorInRefParent
			- localChildAnchorInRefChild);

#ifndef EXCLUDE_FROM_TEST
	// draw line from limb A to surface anchor point of A (GREEN LINE)
	SimulationManager::getSingleton()->getDebugDrawer().drawLine(parentLimbCOM,
		parentLimbCOM + localParentAnchorInRefParent,
		Ogre::ColourValue(0, 1, 0));

	SimulationManager::getSingleton()->getDebugDrawer().drawSphere(
		parentLimbCOM, 0.1, Ogre::ColourValue(0, 1, 0));

	SimulationManager::getSingleton()->getDebugDrawer().drawSphere(
		parentLimbCOM + localParentAnchorInRefParent, 0.1,
		Ogre::ColourValue(0, 1, 0));

	// draw line from anchor point of A to joint rotation point (BLUE LINE)
	SimulationManager::getSingleton()->getDebugDrawer().drawLine(
		parentLimbCOM + localParentAnchorInRefParent,
		parentLimbCOM + localParentJointInRefParent,
		Ogre::ColourValue(0, 0, 1));

	SimulationManager::getSingleton()->getDebugDrawer().drawSphere(
		parentLimbCOM + localParentAnchorInRefParent, 0.1,
		Ogre::ColourValue(0, 0, 1));
	SimulationManager::getSingleton()->getDebugDrawer().drawSphere(
		parentLimbCOM + localParentJointInRefParent, 0.1,
		Ogre::ColourValue(0, 0, 1));

	// draw line from joint rotation point to surface anchor point of B (BLUE LINE)
	SimulationManager::getSingleton()->getDebugDrawer().drawLine(
		parentLimbCOM + localParentJointInRefParent,
		parentLimbCOM + localChildAnchorInRefParent,
		Ogre::ColourValue(0, 0, 1));

	SimulationManager::getSingleton()->getDebugDrawer().drawSphere(
		parentLimbCOM + localParentJointInRefParent, 0.1,
		Ogre::ColourValue(0, 0, 1));
	SimulationManager::getSingleton()->getDebugDrawer().drawSphere(
		parentLimbCOM + localChildAnchorInRefParent, 0.1,
		Ogre::ColourValue(0, 0, 1));

	// draw line from limb B to anchor point of B (GREEN LINE)
	SimulationManager::getSingleton()->getDebugDrawer().drawLine(childLimbCOM,
		childLimbCOM + localChildAnchorInRefChild, Ogre::ColourValue(0, 1, 0));

	SimulationManager::getSingleton()->getDebugDrawer().drawSphere(childLimbCOM,
		0.1, Ogre::ColourValue(0, 1, 0));
	SimulationManager::getSingleton()->getDebugDrawer().drawSphere(
		childLimbCOM + localChildAnchorInRefChild, 0.1,
		Ogre::ColourValue(0, 1, 0));

	// draw line from limb A to limb B (WHITE LINE)
	SimulationManager::getSingleton()->getDebugDrawer().drawLine(childLimbCOM,
		parentLimbCOM, Ogre::ColourValue(1, 1, 1));
#endif

	// set global center of mass of child limb as the new generation point for generation
	generator->setPosition(childLimbCOM);
//	generator->setOrientation(); Orientation has been prepared already
}

LimbModel* SRBPhenomeModel::createLimb(PhenotypeGenerator* generator,
	Morphogene* childMorphogene) {
	double sizeX =
		(generator->getCurrentShrinkageFactor() * childMorphogene->getX()
			< MorphologyConfiguration::LIMB_MIN_SIZE) ?
			MorphologyConfiguration::LIMB_MIN_SIZE :
		(generator->getCurrentShrinkageFactor() * childMorphogene->getX()
			> MorphologyConfiguration::LIMB_MAX_SIZE) ?
			MorphologyConfiguration::LIMB_MAX_SIZE :
			generator->getCurrentShrinkageFactor() * childMorphogene->getX();

	double sizeY =
		(generator->getCurrentShrinkageFactor() * childMorphogene->getY()
			< MorphologyConfiguration::LIMB_MIN_SIZE) ?
			MorphologyConfiguration::LIMB_MIN_SIZE :
		(generator->getCurrentShrinkageFactor() * childMorphogene->getY()
			> MorphologyConfiguration::LIMB_MAX_SIZE) ?
			MorphologyConfiguration::LIMB_MAX_SIZE :
			generator->getCurrentShrinkageFactor() * childMorphogene->getY();

	double sizeZ =
		(generator->getCurrentShrinkageFactor() * childMorphogene->getZ()
			< MorphologyConfiguration::LIMB_MIN_SIZE) ?
			MorphologyConfiguration::LIMB_MIN_SIZE :
		(generator->getCurrentShrinkageFactor() * childMorphogene->getZ()
			> MorphologyConfiguration::LIMB_MAX_SIZE) ?
			MorphologyConfiguration::LIMB_MAX_SIZE :
			generator->getCurrentShrinkageFactor() * childMorphogene->getZ();

	//build the limb out of the morphogene
	SRBLimbModel* childLimb = new SRBLimbModel(getCreatureModel()->getWorld(),
		getCreatureModel(), childMorphogene->getPrimitiveType(),
		generator->getPosition(), generator->getOrientation(),
		Ogre::Vector3(
			generator->getPosition() - getCreatureModel()->getPosition()),
		generator->getOrientation(),
		/*size*/
		Ogre::Vector3(sizeX, sizeY, sizeZ),
		/*mass*/
		sizeX * sizeY * sizeZ,
		//log2(sizeX * sizeY * sizeZ)/log2(PhysicsConfiguration::WEIGHT_SCALE_SHRINK_LOGBASE),
		childMorphogene->getRestitution(), childMorphogene->getFriction(),
		Ogre::ColourValue(childMorphogene->getColorR(),
			childMorphogene->getColorB(), childMorphogene->getColorG()),
		childMorphogene->isIntraBodyColliding(), getLimbModels().size());

	childLimb->initialize();

	BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info)<< "Limb: " << getLimbModels().size();
	getLimbModels().push_back(childLimb);
	getComponentModels().push_back(childLimb);

	return childLimb;
}

void SRBPhenomeModel::appendToParentLimb(LimbModel* childLimb,
	PhenotypeGenerator* generator, Ogre::Vector3& localParentJointInRefParent,
	Ogre::Vector3& localChildJointInRefChild, btTransform& parentHitTransform,
	btTransform& childHitTransform) {

	//PARENT
	//get the morphogene branch that defines the joint and connects the limbs
	MorphogeneBranch* parentMorphogeneBranch =
		((MorphogeneBranch*) generator->getGeneBranch());

	// get parent limb
	SRBLimbModel* parentLimb =
		(SRBLimbModel*) generator->getParentComponentModel();

	// transformation from the parent limb and child limb center of mass to the joint in the respective reference frames
	btTransform localParentJointTransform, localChildJointTransform;
	localParentJointTransform.setIdentity();
	localChildJointTransform.setIdentity();

	// define the position and direction of the joint in the reference frame of the parent
	localParentJointTransform.setOrigin(
		OgreBulletUtils::convert(localParentJointInRefParent));
	//TODO: Fix rotation
//	localParentJointTransform.getBasis().setRotation(
//		parentHitTransform.getRotation());

	// define the position and direction of the joint in the reference frame of child
	localChildJointTransform.setOrigin(
		OgreBulletUtils::convert(-localChildJointInRefChild));
	//TODO: Fix rotation
	//set the direction of the joint normals
//	localChildJointTransform.getBasis().setRotation(
//		childHitTransform.getRotation());

	//create the joint from the two limbs using limb A, limb B and their joint definitions in the respective reference frames
	SRBJointModel* joint = new SRBJointModel(getCreatureModel()->getWorld(),
	/*parent limb*/
	((FSLimbBt*) parentLimb->getLimbPhysics())->getRigidBody(),
	/*child limb*/
	((FSLimbBt*) childLimb->getLimbPhysics())->getRigidBody(),
		localParentJointTransform, localChildJointTransform,
		parentLimb->getOwnIndex(), childLimb->getOwnIndex(),
		getJointModels().size(), parentMorphogeneBranch->getJointType(),
		parentMorphogeneBranch->isJointPitchEnabled(),
		parentMorphogeneBranch->isJointYawEnabled(),
		parentMorphogeneBranch->isJointRollEnabled(),
		Ogre::Vector3(parentMorphogeneBranch->getJointPitchAxisX(),
			parentMorphogeneBranch->getJointPitchAxisY(),
			parentMorphogeneBranch->getJointPitchAxisZ()),
		Ogre::Vector3(parentMorphogeneBranch->getJointRollMinAngle(),
			parentMorphogeneBranch->getJointPitchMinAngle(),
			parentMorphogeneBranch->getJointYawMinAngle()),
		Ogre::Vector3(parentMorphogeneBranch->getJointRollMaxAngle(),
			parentMorphogeneBranch->getJointPitchMaxAngle(),
			parentMorphogeneBranch->getJointYawMaxAngle()));

	BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info)<< "Joint: Parent: " << joint->getParentIndex() << " /Child: "
	<< joint->getChildIndex();

	parentLimb->addChildJointIndex(joint->getIndex());
	childLimb->setParentJointIndex(joint->getIndex());

	// add the joint to the phenotype joints
	getJointModels().push_back(joint);
	getComponentModels().push_back(joint);

	//initialize rotational limit motors
	double mass1 = parentLimb->getMass();
	double mass2 = childLimb->getMass();
	double maxTorque = (0.01f * (mass1 + mass2) + 0 * pow(mass1 + mass2, 2));

	//		std::cout << mass1 << "," << mass2 << "," << maxTorque << std::endl;
	joint->generateMotors(Ogre::Vector3(maxTorque, maxTorque, maxTorque),
		Ogre::Vector3(parentMorphogeneBranch->getJointPitchMinAngle(),
			parentMorphogeneBranch->getJointYawMinAngle(),
			parentMorphogeneBranch->getJointRollMinAngle()),
		Ogre::Vector3(parentMorphogeneBranch->getJointPitchMaxAngle(),
			parentMorphogeneBranch->getJointYawMaxAngle(),
			parentMorphogeneBranch->getJointRollMaxAngle()));

	// add controllers
	for (int i = 0; i < joint->getMotors().size(); i++) {
		switch (parentMorphogeneBranch->getControllerGenes()[i]->getControllerType()) {
		case ControllerGene::SineControllerGene: {
			SineController* controller =
				new SineController(
					((SineControllerGene*) parentMorphogeneBranch->getControllerGenes()[i])->getAmplitude(),
					((SineControllerGene*) parentMorphogeneBranch->getControllerGenes()[i])->getFrequency(),
					((SineControllerGene*) parentMorphogeneBranch->getControllerGenes()[i])->getXOffset(),
					((SineControllerGene*) parentMorphogeneBranch->getControllerGenes()[i])->getYOffset());
			controller->initialize();
			controller->addControlOutput(joint->getMotors()[i]);
			getControllers().push_back(controller);
			break;
		}
		case ControllerGene::ChaoticControllerGene: {
			ChaoticController* controller = new ChaoticController();
			controller->initialize();
			controller->addControlOutput(joint->getMotors()[i]);
			getControllers().push_back(controller);
		}
		}

	}
}

btTransform SRBPhenomeModel::getParentIntersection(LimbPhysics* parentLimb,
	MorphogeneBranch* parentMorphogeneBranch, Ogre::Vector3 parentLimbCOM,
	Ogre::Vector3 localParentAnchorDirInRefParent) {

	((SRBLimbBt*) parentLimb)->addToWorld(); // add parent limb to world

	btTransform parentHitLocation =
		parentLimb->getLocalIntersection(
			OgreBulletUtils::convert(
				parentLimbCOM) /**!< origin of limb parent*/,
			OgreBulletUtils::convert(
				localParentAnchorDirInRefParent) /**!< direction of anchor of limb parent*/);

	((SRBLimbBt*) parentLimb)->removeFromWorld(); // remove parent limb from world

	return parentHitLocation;
}

btTransform SRBPhenomeModel::getOwnIntersection(Morphogene* childMorphogene,
	PhenotypeGenerator* generator,
	Ogre::Vector3 localChildAnchorDirInRefChild) {
	SRBLimbBt* childLimbBt = new SRBLimbBt(getCreatureModel()->getWorld(),
	NULL, childMorphogene->getPrimitiveType(), generator->getPosition(),
		generator->getOrientation(), Ogre::Vector3(),
		generator->getOrientation(),
		/*dimensions*/
		Ogre::Vector3(
			generator->getCurrentShrinkageFactor() * childMorphogene->getX(),
			generator->getCurrentShrinkageFactor() * childMorphogene->getY(),
			generator->getCurrentShrinkageFactor() * childMorphogene->getZ()),
		/*mass*/
		generator->getCurrentShrinkageFactor() * childMorphogene->getX()
			* generator->getCurrentShrinkageFactor() * childMorphogene->getY()
			* generator->getCurrentShrinkageFactor() * childMorphogene->getZ(),
		childMorphogene->getRestitution(), childMorphogene->getFriction(),
		Ogre::ColourValue(0, 0, 0), false);

	childLimbBt->initialize();

	// add child limb to world
	childLimbBt->addToWorld();

	btTransform childHitTransform = childLimbBt->getLocalIntersection(
	/*origin of child limb*/
	OgreBulletUtils::convert(generator->getPosition()),
		/*direction of anchor of child limb*/
		OgreBulletUtils::convert(
			generator->getOrientation() * localChildAnchorDirInRefChild));

	// remove child limb from world
	childLimbBt->removeFromWorld();

	delete childLimbBt;
	childLimbBt = NULL;
	return childHitTransform;
}

void SRBPhenomeModel::generateBody() {
	bool isFixedBase = false;
	bool isMultiDof = true;
	bool setDamping = true;
	bool gyro = true;
	bool canSleep = true;

	if (mBodyGenerated) {
		return;
	}

	BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info)<< "--Generate the body";

	if (mJointModels.size() != 0) {

		for (int i = 0; i < mJointModels.size(); i++) {

			((SRBJointModel*) mJointModels[i])->setBodyA(
				((SRBLimbModel*) mLimbModels[((long) mJointModels[i]->getParentIndex())])->getRigidBody());

			((SRBJointModel*) mJointModels[i])->setFrameInA(
				mJointModels[i]->getParentComToPivot());

			((SRBJointModel*) mJointModels[i])->setBodyB(
				((SRBLimbModel*) mLimbModels[((long) mJointModels[i]->getChildIndex())])->getRigidBody());

			((SRBJointModel*) mJointModels[i])->setFrameInB(
				mJointModels[i]->getPivotToChildCom());

			BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info)<< "Joint: Parent: " << mJointModels[i]->getParentIndex() << " /Child: "
			<< mJointModels[i]->getChildIndex();

			((SRBJointModel*) mJointModels[i])->setWorld(getWorld());

			if (!setDamping) {
//				mJointModels[i]->setLinearDamping(0.f,0.f,0.f);
				mJointModels[i]->setAngularDamping(0.f, 0.f, 0.f);
			} else {
//				mJointModels[i]->setLinearDamping(0.1f,0.1f,0.1f);
				mJointModels[i]->setAngularDamping(0.9f, 0.9f, 0.9f);
			}

			mJointModels[i]->initialize();

			for (std::vector<Motor*>::iterator mit =
				mJointModels[i]->getMotors().begin();
				mit != mJointModels[i]->getMotors().end(); mit++) {
				((SRBServoMotor*) (*mit))->instantiate(
					mJointModels[i]->getJointPhysics(),
					mJointModels[i]->getIndex());
			}
		}
	}
}

void SRBPhenomeModel::reset(const Ogre::Vector3 position) {
	/**The vector of limb models.*/
	for (std::vector<LimbModel*>::const_iterator it = mLimbModels.begin();
		it != mLimbModels.end(); it++) {
		(*it)->reset(position);
	}
}

void SRBPhenomeModel::cleanup() {
	for (std::vector<ComponentModel*>::iterator cit = mComponentModels.begin();
		cit != mComponentModels.end();) {
		delete *cit;
		cit = mComponentModels.erase(cit);
	}
	mLimbModels.clear();
	mJointModels.clear();
}

void SRBPhenomeModel::reposition(const Ogre::Vector3 position) {
	/**The vector of limb models.*/
	for (std::vector<LimbModel*>::const_iterator it = mLimbModels.begin();
		it != mLimbModels.end(); it++) {
		(*it)->reposition(position);

	}
}

bool SRBPhenomeModel::equals(const SRBPhenomeModel& SRBPhenomeModel) const {
	if (mInWorld != SRBPhenomeModel.mInWorld) {
		return false;
	}

	if (mDeveloped != SRBPhenomeModel.mDeveloped) {
		return false;
	}

	/**The vector of limb models.*/
	if (mLimbModels.size() != SRBPhenomeModel.mLimbModels.size()) {
		return false;
	}
	std::vector<LimbModel*>::const_iterator it = mLimbModels.begin();
	std::vector<LimbModel*>::const_iterator it2 =
		SRBPhenomeModel.mLimbModels.begin();
	for (; it != mLimbModels.end(), it2 != SRBPhenomeModel.mLimbModels.end();
		it++, it2++) {
		if (!(*it)->equals(**(it2))) {
			return false;
		}
	}

	/**The vector of joint models.*/
	if (mJointModels.size() != SRBPhenomeModel.mJointModels.size()) {
		return false;
	}
	std::vector<JointModel*>::const_iterator it3 = mJointModels.begin();
	std::vector<JointModel*>::const_iterator it4 =
		SRBPhenomeModel.mJointModels.begin();
	for (; it3 != mJointModels.end(), it4 != SRBPhenomeModel.mJointModels.end();
		it3++, it4++) {
		if (!(*it3)->equals(**(it4))) {
			return false;
		}
	}

	/**The vector of controllers.*/
	if (mControllers.size() != SRBPhenomeModel.mControllers.size()) {
		return false;
	}
	std::vector<Controller*>::const_iterator it5 = mControllers.begin();
	std::vector<Controller*>::const_iterator it6 =
		SRBPhenomeModel.mControllers.begin();
	for (; it5 != mControllers.end(), it6 != SRBPhenomeModel.mControllers.end();
		it5++, it6++) {
		if (!(*it5)->equals(**(it6))) {
			return false;
		}
	}
	return true;
}

void SRBPhenomeModel::addToWorld() {
	if (!isInWorld()) {

	}
}

void SRBPhenomeModel::removeFromWorld() {
	if (isInWorld()) {

	}
}

PhenomeModel* SRBPhenomeModel::clone() {
	return new SRBPhenomeModel(*this);
}

btDynamicsWorld* SRBPhenomeModel::getWorld() {
	if (!mWorld) {
#ifndef EXCLUDE_FROM_TEST
		mWorld = (btDynamicsWorld*) mCreatureModel->getWorld();
#endif
	}
	return mWorld;

}
