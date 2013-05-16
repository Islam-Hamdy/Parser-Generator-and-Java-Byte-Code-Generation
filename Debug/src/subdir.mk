################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CompilersPhase2.cpp \
../src/Parser.cpp \
../src/PredictiveParser.cpp 

OBJS += \
./src/CompilersPhase2.o \
./src/Parser.o \
./src/PredictiveParser.o 

CPP_DEPS += \
./src/CompilersPhase2.d \
./src/Parser.d \
./src/PredictiveParser.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


