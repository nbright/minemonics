################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/view/ogre3D/evolution/population/creature/phenome/morphology/muscle/MuscleO3D.cpp 

OBJS += \
./src/view/ogre3D/evolution/population/creature/phenome/morphology/muscle/MuscleO3D.o 

CPP_DEPS += \
./src/view/ogre3D/evolution/population/creature/phenome/morphology/muscle/MuscleO3D.d 


# Each subdirectory must supply rules for building sources it contributes
src/view/ogre3D/evolution/population/creature/phenome/morphology/muscle/%.o: ../src/view/ogre3D/evolution/population/creature/phenome/morphology/muscle/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


