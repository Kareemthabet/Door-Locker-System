################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../buzzer.c \
../external_eeprom.c \
../gpio.c \
../i2c.c \
../main.c \
../motor.c \
../timer0.c \
../uart.c 

OBJS += \
./buzzer.o \
./external_eeprom.o \
./gpio.o \
./i2c.o \
./main.o \
./motor.o \
./timer0.o \
./uart.o 

C_DEPS += \
./buzzer.d \
./external_eeprom.d \
./gpio.d \
./i2c.d \
./main.d \
./motor.d \
./timer0.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


