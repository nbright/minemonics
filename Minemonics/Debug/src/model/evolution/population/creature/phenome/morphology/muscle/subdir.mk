################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/model/evolution/population/creature/phenome/morphology/muscle/Muscle.cpp 

OBJS += \
./src/model/evolution/population/creature/phenome/morphology/muscle/Muscle.o 

CPP_DEPS += \
./src/model/evolution/population/creature/phenome/morphology/muscle/Muscle.d 


# Each subdirectory must supply rules for building sources it contributes
src/model/evolution/population/creature/phenome/morphology/muscle/%.o: ../src/model/evolution/population/creature/phenome/morphology/muscle/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


