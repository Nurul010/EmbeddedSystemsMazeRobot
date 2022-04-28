#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = D:/ti/tirtos_tivac_2_16_01_14/packages;D:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages;D:/ti/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages;D:/ti/tirtos_tivac_2_16_01_14/products/ndk_2_25_00_09/packages;D:/ti/tirtos_tivac_2_16_01_14/products/uia_2_00_05_50/packages;D:/ti/tirtos_tivac_2_16_01_14/products/ns_1_11_00_10/packages;D:/ti/ccsv6/ccs_base
override XDCROOT = D:/ti/xdctools_3_32_00_06_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = D:/ti/tirtos_tivac_2_16_01_14/packages;D:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages;D:/ti/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages;D:/ti/tirtos_tivac_2_16_01_14/products/ndk_2_25_00_09/packages;D:/ti/tirtos_tivac_2_16_01_14/products/uia_2_00_05_50/packages;D:/ti/tirtos_tivac_2_16_01_14/products/ns_1_11_00_10/packages;D:/ti/ccsv6/ccs_base;D:/ti/xdctools_3_32_00_06_core/packages;..
HOSTOS = Windows
endif
