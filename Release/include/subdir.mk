################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../include/asym_enc.cpp \
../include/comm_layer.cpp \
../include/encryption.cpp \
../include/utilities.cpp 

OBJS += \
./include/asym_enc.o \
./include/comm_layer.o \
./include/encryption.o \
./include/utilities.o 

CPP_DEPS += \
./include/asym_enc.d \
./include/comm_layer.d \
./include/encryption.d \
./include/utilities.d 


# Each subdirectory must supply rules for building sources it contributes
include/%.o: ../include/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


