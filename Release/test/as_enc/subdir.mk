################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../test/as_enc/asym_enc.o \
../test/as_enc/comm_layer.o \
../test/as_enc/utilities.o 

CPP_SRCS += \
../test/as_enc/apeer.cpp \
../test/as_enc/kdc.cpp 

OBJS += \
./test/as_enc/apeer.o \
./test/as_enc/kdc.o 

CPP_DEPS += \
./test/as_enc/apeer.d \
./test/as_enc/kdc.d 


# Each subdirectory must supply rules for building sources it contributes
test/as_enc/%.o: ../test/as_enc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

