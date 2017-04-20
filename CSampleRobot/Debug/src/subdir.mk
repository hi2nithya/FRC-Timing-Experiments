################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Robot.cpp 

OBJS += \
./src/Robot.o 

CPP_DEPS += \
./src/Robot.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-frc-linux-gnueabi-g++ -std=c++1y -I"C:\Users\Srini/wpilib/cpp/current/include" -I"C:\Users\Srini\git\CSampleRobot\CSampleRobot\src" -I"C:\Users\Srini/wpilib/user/cpp/include" -O0 -Og -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


