################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Parser_Query.cpp \
../Parser_Schema.cpp \
../Schema.cpp \
../Types.cpp \
../code_generation.cpp \
../helpers.cpp \
../main.cpp \
../olap_query.cpp \
../query_1.cpp \
../schema_1.cpp 

OBJS += \
./Parser_Query.o \
./Parser_Schema.o \
./Schema.o \
./Types.o \
./code_generation.o \
./helpers.o \
./main.o \
./olap_query.o \
./query_1.o \
./schema_1.o 

CPP_DEPS += \
./Parser_Query.d \
./Parser_Schema.d \
./Schema.d \
./Types.d \
./code_generation.d \
./helpers.d \
./main.d \
./olap_query.d \
./query_1.d \
./schema_1.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++-6 -std=c++0x -O3 -g -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


