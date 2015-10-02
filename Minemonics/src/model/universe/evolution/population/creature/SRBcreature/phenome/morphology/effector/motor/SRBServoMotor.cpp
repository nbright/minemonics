//# corresponding header
#include <iostream>
#include <iomanip>
#include <string>

//## controller headers
//## model headers
#include <BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.h>
#include <BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h>
#include <BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h>
#include <BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h>
#include <model/universe/evolution/population/creature/phenome/controller/ControlOutput.hpp>
#include <model/universe/evolution/population/creature/SRBcreature/phenome/morphology/effector/motor/SRBServoMotor.hpp>
#include <model/universe/evolution/population/creature/SRBcreature/phenome/morphology/joint/SRBJointBt.hpp>
//## view headers
//# custom headers
//## base headers
//## configuration headers
//## controller headers
//## model headers
#include <utils/ogre3D/OgreBulletUtils.hpp>
#include <utils/ogre3D/Euler.hpp>

SRBServoMotor::SRBServoMotor() :
	mJointMotorIndex(JointPhysics::RDOF_PITCH), mMotorBt(NULL), mLowerLimit(0), mUpperLimit(
		0) {
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
}

SRBServoMotor::~SRBServoMotor() {
	mJointMotorIndex = JointPhysics::RDOF_PITCH;
	mMotorBt = NULL;
}

void SRBServoMotor::initialize(
	const JointPhysics::RotationalDegreeOfFreedom jointMotorIndex,
	MOTOR_TYPE* const motorBt, const double maxForce, double lowerLimit,
	double upperLimit) {

	mJointMotorIndex = jointMotorIndex;
	mMaxForce = maxForce;
	mLowerLimit = lowerLimit;
	mUpperLimit = upperLimit;

	mMotorBt = motorBt;

	mMotorBt->m_hiLimit = mUpperLimit;
	mMotorBt->m_loLimit = mLowerLimit;

#ifdef USE_6DOF2
	constraint->enableMotor(mJointMotorIndex, true);
	constraint->setTargetVelocity(mJointMotorIndex, maxSpeed);
	constraint->setMaxMotorForce(mJointMotorIndex, mMaxForce);
	constraint->setServo(mJointMotorIndex, true);
#else
	mMotorBt->m_enableMotor = true;
	mMotorBt->m_maxMotorForce = mMaxForce;
	//servo motor is not implemented in 6dofspring constraint
#endif

}

void SRBServoMotor::apply(double timeSinceLastTick) {

	//clamp the input value to [0;1] because otherwise the motor does not work anymore.
	btScalar clampedInputValue =
		(getInputValue() > 1.0f) ? 1.0f :
		(getInputValue() < 0.0f) ? 0.0f : getInputValue();

	//calculate the target angle of the motor
	btScalar targetAngle = mLowerLimit
		+ clampedInputValue * (mUpperLimit - mLowerLimit);
	//calculate the angle error
	btScalar angleError = targetAngle - mMotorBt->m_currentPosition;

#ifdef USE_6DOF2
	mConstraint->setServoTarget(mJointMotorIndex, targetAngle);
#else

	float kP = 2.0f;
	float mMaxForce = 1000.0f;
	//simple p(roportional) controller
	//calculate the target velocity and clamp it with the maximum speed
	mMotorBt->m_targetVelocity = 10000000;
	mMotorBt->m_maxMotorForce =
		(kP * angleError > mMaxForce) ? mMaxForce :
		(kP * angleError < -mMaxForce) ? -mMaxForce : kP * angleError;
#endif
	std::cout << mMaxForce << "," << kP * angleError << std::endl;
}

SRBServoMotor* SRBServoMotor::clone() {
	return new SRBServoMotor(*this);
}