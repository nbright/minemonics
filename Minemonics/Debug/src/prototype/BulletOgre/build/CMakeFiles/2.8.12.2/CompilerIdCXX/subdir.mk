################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/prototype/BulletOgre/build/CMakeFiles/2.8.12.2/CompilerIdCXX/CMakeCXXCompilerId.cpp 

OBJS += \
./src/prototype/BulletOgre/build/CMakeFiles/2.8.12.2/CompilerIdCXX/CMakeCXXCompilerId.o 

CPP_DEPS += \
./src/prototype/BulletOgre/build/CMakeFiles/2.8.12.2/CompilerIdCXX/CMakeCXXCompilerId.d 


# Each subdirectory must supply rules for building sources it contributes
src/prototype/BulletOgre/build/CMakeFiles/2.8.12.2/CompilerIdCXX/%.o: ../src/prototype/BulletOgre/build/CMakeFiles/2.8.12.2/CompilerIdCXX/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


