//# corresponding header
#include <model/universe/evolution/population/creature/phenome/morphology/effector/motor/SRBServoMotor.hpp>

//## controller headers
//## model headers
#include <model/universe/evolution/population/creature/phenome/controller/ControlInput.hpp>

//# forward declarations
//# system headers
//## controller headers
//## model headers
//## view headers
//# custom headers
//## base headers
//## configuration headers
//## controller headers
//## model headers
//## view headers
//## utils headers

BoostLogger SRBServoMotor::mBoostLogger; /*<! initialize the boost logger*/
SRBServoMotor::_Init SRBServoMotor::_initializer;
SRBServoMotor::SRBServoMotor() :
	mLastPosition(0) {
}

SRBServoMotor::SRBServoMotor(
	const JointPhysics::RotationalDegreeOfFreedom jointMotorIndex,
	const double maxForce, double lowerLimit, double upperLimit,
	bool positionControlled) : mLastPosition(0) {
	mJointMotorIndex = jointMotorIndex;
	mMaxForce = maxForce;
	mLowerLimit = lowerLimit;
	mUpperLimit = upperLimit;
	mPositionControlled = positionControlled;
}

SRBServoMotor::SRBServoMotor(const SRBServoMotor& SRBServoMotor) {
	mEnabled = SRBServoMotor.mEnabled;
	mIndex = SRBServoMotor.mIndex;
	mJointMotorIndex = SRBServoMotor.mJointMotorIndex;
	mMaxForce = SRBServoMotor.mMaxForce;
	mMotorType = SRBServoMotor.mMotorType;
	mPositionControlled = SRBServoMotor.mPositionControlled;
	mLowerLimit = SRBServoMotor.mLowerLimit;
	mUpperLimit = SRBServoMotor.mUpperLimit;
	mMotorBt = SRBServoMotor.mMotorBt;
	mLastPosition = SRBServoMotor.mLastPosition;
}

SRBServoMotor::~SRBServoMotor() {
	mJointMotorIndex = JointPhysics::RDOF_PITCH;
}

void SRBServoMotor::initialize(MOTOR_TYPE* const motorBt) {

//	mMotorBt = motorBt;
//
//	if (mMotorBt) {
//		mMotorBt->m_hiLimit = mUpperLimit;
//		mMotorBt->m_loLimit = mLowerLimit;
//		mLastPosition = mMotorBt->m_currentPosition;
//	}

//#ifdef USE_6DOF2
//	constraint->enableMotor(mJointMotorIndex, true);
//	constraint->setTargetVelocity(mJointMotorIndex, 0);
//	constraint->setMaxMotorForce(mJointMotorIndex, mMaxForce);
//	constraint->setServo(mJointMotorIndex, true);
//#else
//	if (mMotorBt) {
//	mMotorBt->m_enableMotor = true;
//	mMotorBt->m_maxMotorForce = 0;
//	//servo motor is not implemented in 6dofspring constraint
//#endif

}

void SRBServoMotor::instantiate(JointPhysics* jointPhysics,
	const int jointIndex) {
	mJoint = jointPhysics;
	mJointIndex = jointIndex;
}

void SRBServoMotor::apply(double timeSinceLastTick) {
#ifdef CONSTRAINT_INDEX
	if (mPositionControlled) {
		//clamp the input value to [0;1] because otherwise the motor does not work anymore.
		btScalar clampedInputValue =
			(getInputValue() > 1.0f) ? 1.0f :
			(getInputValue() < 0.0f) ? 0.0f : getInputValue();

		//calculate the target angle of the motor
		btScalar targetAngle = mLowerLimit
			+ clampedInputValue * (mUpperLimit - mLowerLimit);

		//calculate the angle error
		btScalar angleError = targetAngle - mJoint->getJointPos(mJointMotorIndex);
		btScalar velocityError = 0 - mLastPosition
			+ mJoint->getJointPos(mJointMotorIndex);
		mLastPosition = mJoint->getJointPos(mJointMotorIndex);



		//simple p(roportional) controller
		//calculate the target force and clamp it with the maximum force
		float kP = 200000000;
		float kD = 2000;
		double correction = kP * angleError + kD * velocityError;
		mJoint->applyJointTorque(mJointMotorIndex,
			btScalar(
				(correction > mMaxForce) ? mMaxForce :
				(correction < -mMaxForce) ? -mMaxForce : correction));
		BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info)<< mJoint->getJointPos(mJointMotorIndex) << "," << targetAngle;
	}
	else {

		//clamp the input value to [0;1] because otherwise the motor does not work anymore.
		btScalar clampedInputValue =
		(getInputValue() > 1.0f) ? 1.0f :
		(getInputValue() < 0.0f) ? 0.0f : getInputValue();
		mJoint->applyJointTorque(0,
			btScalar(getInputValue()));
	}
#endif
}

SRBServoMotor* SRBServoMotor::clone() {
	return new SRBServoMotor(*this);
}
