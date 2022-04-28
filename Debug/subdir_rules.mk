################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
build-271932401: ../empty.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"D:/ti/xdctools_3_32_00_06_core/xs" --xdcpath="D:/ti/tirtos_tivac_2_16_01_14/packages;D:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages;D:/ti/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages;D:/ti/tirtos_tivac_2_16_01_14/products/ndk_2_25_00_09/packages;D:/ti/tirtos_tivac_2_16_01_14/products/uia_2_00_05_50/packages;D:/ti/tirtos_tivac_2_16_01_14/products/ns_1_11_00_10/packages;D:/ti/ccsv6/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.tiva:TM4C123GH6PM -r release -c "D:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path=\"E:/Users/Azman/workspace_v6_2/final code\" --include_path=\"D:/ti/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b\" --include_path=\"D:/ti/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix\" --include_path=\"D:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include\" -g --gcc --define=ccs=\"ccs\" --define=PART_TM4C123GH6PM --define=ccs --define=TIVAWARE --diag_wrap=off --diag_warning=225 --diag_warning=255 --display_error_number --gen_func_subsections=on --abi=eabi  " "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: build-271932401
configPkg/compiler.opt: build-271932401
configPkg/: build-271932401

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="E:/Users/Azman/workspace_v6_2/final code" --include_path="D:/ti/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="D:/ti/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="D:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=ccs --define=TIVAWARE --diag_wrap=off --diag_warning=225 --diag_warning=255 --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


