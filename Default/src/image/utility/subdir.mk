################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/image/utility/ReadFile.cpp \
../src/image/utility/SynSemaphore.cpp \
../src/image/utility/ThreadManager.cpp \
../src/image/utility/TimerTest.cpp 

OBJS += \
./src/image/utility/ReadFile.o \
./src/image/utility/SynSemaphore.o \
./src/image/utility/ThreadManager.o \
./src/image/utility/TimerTest.o 

CPP_DEPS += \
./src/image/utility/ReadFile.d \
./src/image/utility/SynSemaphore.d \
./src/image/utility/ThreadManager.d \
./src/image/utility/TimerTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/image/utility/%.o: ../src/image/utility/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ljd/workspace/MyTCD/include" -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


