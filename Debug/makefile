################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS

GEN_OPTS__FLAG := --cmd_file="configPkg/compiler.opt"
GEN_CMDS__FLAG := -l"configPkg/linker.cmd"

ORDERED_OBJS += \
"./main.obj" \
"./RTOS/RTOSBluetooth.obj" \
"./RTOS/RTOSCAN.obj" \
"./RTOS/RTOSCommander.obj" \
"./RTOS/RTOSDistance.obj" \
"./RTOS/RTOSFlashing.obj" \
"./RTOS/RTOSLightSensor.obj" \
"./RTOS/RTOSMotor.obj" \
"./RTOS/RTOSPID.obj" \
"../EK_TM4C123GXL.cmd" \
$(GEN_CMDS__FLAG) \
-l"D:/ti/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b/grlib/ccs/Debug/grlib.lib" \
-l"D:/ti/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b/usblib/ccs/Debug/usblib.lib" \
-l"D:/ti/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b/driverlib/ccs/Debug/driverlib.lib" \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include RTOS/subdir_vars.mk
-include subdir_rules.mk
-include RTOS/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
final\ code.out \

EXE_OUTPUTS__QUOTED += \
"final code.out" \

BIN_OUTPUTS += \
final\ code.hex \

BIN_OUTPUTS__QUOTED += \
"final code.hex" \


# All Target
all: final\ code.out

# Tool invocations
final\ code.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"D:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=ccs --define=TIVAWARE --diag_wrap=off --diag_warning=225 --diag_warning=255 --display_error_number --gen_func_subsections=on --abi=eabi -z -m"final code.map" --stack_size=512 --heap_size=0 -i"D:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/lib" -i"D:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --reread_libs --diag_wrap=off --warn_sections --display_error_number --xml_link_info="final code_linkInfo.xml" --rom_model -o "final code.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

final\ code.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"D:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armhex"  -o "final code.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_OPTS__QUOTED)$(GEN_CMDS__QUOTED)
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-$(RM) "main.d" "RTOS\RTOSBluetooth.d" "RTOS\RTOSCAN.d" "RTOS\RTOSCommander.d" "RTOS\RTOSDistance.d" "RTOS\RTOSFlashing.d" "RTOS\RTOSLightSensor.d" "RTOS\RTOSMotor.d" "RTOS\RTOSPID.d" 
	-$(RM) "main.obj" "RTOS\RTOSBluetooth.obj" "RTOS\RTOSCAN.obj" "RTOS\RTOSCommander.obj" "RTOS\RTOSDistance.obj" "RTOS\RTOSFlashing.obj" "RTOS\RTOSLightSensor.obj" "RTOS\RTOSMotor.obj" "RTOS\RTOSPID.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

