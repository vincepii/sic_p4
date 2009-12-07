################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../bpeer/bpeer.cpp 

OBJS += \
./bpeer/bpeer.o 

CPP_DEPS += \
./bpeer/bpeer.d 


# Each subdirectory must supply rules for building sources it contributes
bpeer/%.o: ../bpeer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


