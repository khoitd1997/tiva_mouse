################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
drivers/buttons.obj: /home/kd/tiva_mouse/tiva_mouse/Tiva_Extra/Tivaware_Dep/examples/boards/ek-tm4c123gxl/drivers/buttons.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/kd/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="/home/kd/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --include_path="/home/kd/tiva_mouse/tiva_mouse/Tiva_Extra" --include_path="/home/kd/tiva_mouse/tiva_mouse/Tiva_Extra/Tivaware_Dep" --advice:power="all" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RA1 --define=UART_BUFFERED --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="drivers/$(basename $(<F)).d_raw" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


