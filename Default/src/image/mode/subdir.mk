################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/image/mode/MultiDeepModeTCDThread.cpp 

OBJS += \
./src/image/mode/MultiDeepModeTCDThread.o 

CPP_DEPS += \
./src/image/mode/MultiDeepModeTCDThread.d 


# Each subdirectory must supply rules for building sources it contributes
src/image/mode/%.o: ../src/image/mode/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ljd/workspace/MyTCD/include" -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


