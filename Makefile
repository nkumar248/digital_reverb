# Project Name
TARGET = schroeder_reverb

# Sources
CPP_SOURCES = schroeder_reverb.cpp

# Library Locations
LIBDAISY_DIR = ../../DaisyExamples/libDaisy/
DAISYSP_DIR = ../../DaisyExamples/DaisySP/

OPT = -O0

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
