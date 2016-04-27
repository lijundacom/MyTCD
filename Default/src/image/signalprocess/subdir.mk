################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/image/signalprocess/MultiDeepModeTCDSignalManage.cpp \
../src/image/signalprocess/MultiDeepModeTCDSignalProcess.cpp 

OBJS += \
./src/image/signalprocess/MultiDeepModeTCDSignalManage.o \
./src/image/signalprocess/MultiDeepModeTCDSignalProcess.o 

CPP_DEPS += \
./src/image/signalprocess/MultiDeepModeTCDSignalManage.d \
./src/image/signalprocess/MultiDeepModeTCDSignalProcess.d 


# Each subdirectory must supply rules for building sources it contributes
src/image/signalprocess/%.o: ../src/image/signalprocess/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ljd/workspace/MyTCD/include" -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


