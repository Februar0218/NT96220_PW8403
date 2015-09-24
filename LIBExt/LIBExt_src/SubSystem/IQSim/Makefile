
#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = IQSim

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
SRC = \
	ImageQualitySim.c \
	ImageQualitySim_value.c \
	ImageQualitySim_buffer.c \
	ImageQualitySim_flow_capture.c \
	ImageQualitySim_util.c \
	ImageQualitySim_SIE.c \
	ImageQualitySim_IFE.c \
	ImageQualitySim_IPE.c \
	ImageQualitySim_IME.c \
	ImageQualitySim_JPG.c


ASM =

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = .. \
	../../../../Include \

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = ../../..

#----------------------------------------------------------------------
# add additional C and assembly flags here
#----------------------------------------------------------------------
CFLAGS_D = -Otime 
ASMFLAGS_D =

CFLAGS_R = -Otime 
ASMFLAGS_R =

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = ../../../../MakeCommon
include $(MAKE_COMMON_DIR)/OutputLib.txt
