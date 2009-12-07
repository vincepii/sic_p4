################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/comunicazione/apeer.cpp \
../test/comunicazione/kdc.cpp 

OBJS += \
./test/comunicazione/apeer.o \
./test/comunicazione/kdc.o 

CPP_DEPS += \
./test/comunicazione/apeer.d \
./test/comunicazione/kdc.d 


# Each subdirectory must supply rules for building sources it contributes
test/comunicazione/%.o: ../test/comunicazione/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


