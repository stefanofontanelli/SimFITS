#
# Makefile for ReVANS.
#

# Name of target to be created (-o option)

AIMSUN_DIR = /Applications/Aimsun6/Aimsun.app/Contents/PlugIns
OMNETPP_BASE = /Applications/omnetpp-4.1
OMNETPP_SRC = $(OMNETPP_BASE)/src
OMNETPP_DIR = $(OMNETPP_BASE)/include
OMNETPP_LIB = $(OMNETPP_BASE)/lib
ENVIR_DIR = $(OMNETPP_SRC)/envir
CMDENV_DIR = $(OMNETPP_SRC)/cmdenv
MiXiM_BASE = /Applications/MiXiM/base
MiXiM_BCM = $(MiXiM_BASE)/connectionManager
MiXiM_UTILS = $(MiXiM_BASE)/utils
MiXiM_MODS = /Applications/MiXiM/modules
MiXiM_CM = $(MiXiM_MODS)/connectionManager

SRC_DIR = src
OBJ_DIR = build
DIST_DIR = dist
SHARED_LIB_SUFFIX = .dylib
TARGET = librevans$(SHARED_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH =	-I. \
				-I$(OMNETPP_DIR) \
				-I$(OMNETPP_DIR)/platdep \
				-I$(ENVIR_DIR) \
				-I$(CMDENV_DIR) \
				-I$(OMNETPP_SRC)/common \
				-I$(MiXiM_BCM) \
				-I$(MiXiM_BASE)/messages \
				-I$(MiXiM_BASE)/modules \
				-I$(MiXiM_BASE)/phyLayer \
				-I$(MiXiM_BASE)/timers \
				-I$(MiXiM_UTILS) \
				-I$(MiXiM_MODS)/analogueModel \
				-I$(MiXiM_MODS)/application \
				-I$(MiXiM_CM) \
				-I$(MiXiM_MODS)/mac \
				-I$(MiXiM_MODS)/messages \
				-I$(MiXiM_MODS)/mobility \
				-I$(MiXiM_MODS)/netw \
				-I$(MiXiM_MODS)/nic \
				-I$(MiXiM_MODS)/node \
				-I$(MiXiM_MODS)/phy \
				-I$(MiXiM_MODS)/power \
				-I$(MiXiM_MODS)/transport \
				-I$(MiXiM_MODS)/utility

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =	-L$(AIMSUN_DIR) -la2kernel \
		-L$(OMNETPP_LIB) -loppsim -loppenvir -loppcommon \
		-loppeventlog -lopplayout -loppnedxml -loppscave \
		-L$(MiXiM_BASE) -lmiximbase -L$(MiXiM_MODS) -lmiximmodules

OUTPUT = -dynamiclib -o $(DIST_DIR)/$(TARGET)
ARCH_OPTS = 

# Object files for local .cc and .msg files
OBJS =

# Main target
all: revans.o

revans.o:	$(SRC_DIR)/revans.cpp \
			aimsun.o
	g++ $(SRC_DIR)/revans.cpp \
		$(OBJ_DIR)/aimsun.o \
		$(OBJ_DIR)/aimsunenv.o \
		$(OBJ_DIR)/hostutility.o \
		$(OBJ_DIR)/aimsunmobility.o \
		$(OBJ_DIR)/aimsunworldutility.o \
		$(OBJ_DIR)/vehicleapp.o \
		$(OBJ_DIR)/junctionapp.o \
		$(OBJ_DIR)/aimsunapplpkt.o \
		$(OBJ_DIR)/aimsunmap.o \
		$(OBJ_DIR)/section.o \
		$(OBJ_DIR)/junction.o \
		$(ARCH_OPTS) \
		$(INCLUDE_PATH) $(LIBS) $(OUTPUT)

aimsun.o:	$(SRC_DIR)/aimsun.cpp \
			aimsunenv.o
	g++ $(SRC_DIR)/aimsun.cpp \
		-c $(INCLUDE_PATH) $(ARCH_OPTS) -o $(OBJ_DIR)/aimsun.o

aimsunenv.o:	$(SRC_DIR)/AimsunEnv.cpp \
					hostutility.o
	g++ $(SRC_DIR)/AimsunEnv.cpp \
		-c $(INCLUDE_PATH) $(ARCH_OPTS) -o $(OBJ_DIR)/aimsunenv.o

hostutility.o:	$(SRC_DIR)/HostUtility.cpp \
						aimsunmobility.o
	g++ $(SRC_DIR)/HostUtility.cpp \
		-c $(INCLUDE_PATH) $(ARCH_OPTS) -o $(OBJ_DIR)/hostutility.o

aimsunmobility.o:	$(SRC_DIR)/AimsunMobility.cpp \
					aimsunworldutility.o \
					vehicleapp.o
	g++ $(SRC_DIR)/AimsunMobility.cpp \
		-c $(INCLUDE_PATH) $(ARCH_OPTS) -o $(OBJ_DIR)/aimsunmobility.o

aimsunworldutility.o:	$(SRC_DIR)/AimsunWorldUtility.cpp \
						aimsunmap.o
	g++ $(SRC_DIR)/AimsunWorldUtility.cpp \
		-c $(INCLUDE_PATH) $(ARCH_OPTS) -o $(OBJ_DIR)/aimsunworldutility.o

vehicleapp.o:	$(SRC_DIR)/VehicleApp.cpp \
				junctionapp.o
	g++ $(SRC_DIR)/VehicleApp.cpp \
		-c $(INCLUDE_PATH) $(ARCH_OPTS) -o $(OBJ_DIR)/vehicleapp.o

junctionapp.o:	$(SRC_DIR)/JunctionApp.cpp \
				aimsunapplpkt.o
	g++ $(SRC_DIR)/JunctionApp.cpp \
		-c $(INCLUDE_PATH) $(ARCH_OPTS) -o $(OBJ_DIR)/junctionapp.o

aimsunapplpkt.o:	$(SRC_DIR)/AimsunApplPkt.cpp
	g++ $(SRC_DIR)/AimsunApplPkt.cpp \
		-c $(INCLUDE_PATH) $(ARCH_OPTS) -o $(OBJ_DIR)/aimsunapplpkt.o

aimsunmap.o:	$(SRC_DIR)/AimsunMap.cpp \
				section.o \
				junction.o
	g++ $(SRC_DIR)/AimsunMap.cpp \
		-c $(INCLUDE_PATH) $(ARCH_OPTS) -o $(OBJ_DIR)/aimsunmap.o

section.o:	$(SRC_DIR)/Section.cpp
	g++ $(SRC_DIR)/Section.cpp \
		-c $(INCLUDE_PATH) $(ARCH_OPTS) -o $(OBJ_DIR)/section.o

junction.o:	$(SRC_DIR)/Junction.cpp
	g++ $(SRC_DIR)/Junction.cpp \
		-c $(INCLUDE_PATH) $(ARCH_OPTS) -o $(OBJ_DIR)/junction.o

clean:
	rm -r $(OBJ_DIR)
	rm -r $(DIST_DIR)
	mkdir $(OBJ_DIR)
	mkdir $(DIST_DIR)



