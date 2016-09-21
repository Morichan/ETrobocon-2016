mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=

APPL_CXXOBJS += \
util.o \
Walker.o \
pid.o \
Emoter.o \
Lifter.o \
Flagman.o \
PrizeArea.o \
PidWalker.o \
ColorChecker.o \
Pedestrian.o \
self_localization.o \
SelfLocalMoving.o \
Puzzle.o \
area.o \
area_control.o \
area_method_startup.o \
area_method_standard_run.o \
area_method_finished.o


SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIR += $(mkfile_path)app $(mkfile_path)unit

INCLUDES += \
	-I$(mkfile_path)app \
	-I$(mkfile_path)unit

# COPTS += -fno-use-cxa-atexit
