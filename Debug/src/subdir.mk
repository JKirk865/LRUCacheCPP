################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/LRUCache.cpp \
../src/LRUCacheUnitTests.cpp \
../src/main.cpp 

OBJS += \
./src/LRUCache.o \
./src/LRUCacheUnitTests.o \
./src/main.o 

CPP_DEPS += \
./src/LRUCache.d \
./src/LRUCacheUnitTests.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++2a -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


