#******************************************************************************
#*
#*   FILE
#*   ----
#*   Makefile - Makefile fuer die Anwender-Bibliothek
#******************************************************************************
#*
#*   This file is generated by the 'acplt_makmak' command
#*
#******************************************************************************

#   Plattform
#   ---------
SYSTEM = NT
SYSDIR = nt

COMPILER = GNU

#	Filename conventions
#	--------------------

_C   = .c
_OBJ = .o
_LIB = .a
_EXE = .exe

_DLL = .dll
#	Include generic part
#	--------------------
include ../generic.mk

# Libraries
# ---------

OVLIBS = $(BASE_LIB_DIR)libov$(_LIB)
ADD_LIBS = $(USERLIB_DIR)/opcua$(_DLL) $(SYSLIB_DIR)/ksbase$(_DLL) $(SYSLIB_DIR)/fb$(_DLL)
ADD_LIBS += $(foreach lib, $(EXTRA_LIBS),$(lib))

#	Compiler
#	--------
OV_CODEGEN_EXE = $(BIN_DIR)ov_codegen$(_EXE)

ifneq "$(origin GCC_BIN_PREFIX)" "undefined"
CC = $(GCC_BIN_PREFIX)gcc
else
GCC_BIN_PREFIX		= 
endif

CC		?= $(GCC_BIN_PREFIX)gcc
ifneq ($(TARGET), debug)
	OPT = -O2 -fno-strict-aliasing
endif
CC_FLAGS	= -g -std=c99 -m32 -Wall -Wno-attributes $(OPT) $(EXTRA_CC_FLAGS)
CC_DEFINES	= $(DEFINES) -D__NT__=1 
CC_INCLUDES	= $(INCLUDES) -I.
COMPILE_C	= $(CC) $(CC_FLAGS) $(CC_DEFINES) $(CC_INCLUDES) -c
LD		= $(CC) -shared -m32 -Wl,--output-def,opcuaSubscription.def,--out-implib,opcuaSubscription.a
AR			= $(GCC_BIN_PREFIX)ar
RANLIB		= $(GCC_BIN_PREFIX)ranlib
STRIP		= $(GCC_BIN_PREFIX)strip

templates: 
	-@cmd /c del "..\..\source\sourcetemplates\*$(_C)"
	acplt_builder -l $(LIBRARY) $(MAKMAKOPTIONS)
	$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR) -I $(OPCUA_MODEL_DIR) -I $(KSBASE_MODEL_DIR) -I $(FB_MODEL_DIR) -f $(MODEL_DIR)$(LIBRARY).ovm -l $(LIBRARY)

	-@echo  ==== New templates have been created! ====

all: $(TARGETS)
ifndef STATIC_ONLY
	$(STRIP) --strip-debug $(USERLIB_LIB)
	$(STRIP) --strip-debug $(USERLIB_DLL)
	cmd /c copy $(USERLIB_DLL) $(subst /,\\, $(USERLIB_DIR))
endif


debug: $(TARGETS)
ifndef STATIC_ONLY
	cmd /c copy $(USERLIB_DLL) $(subst /,\\, $(USERLIB_DIR))
endif


#   Rules
#   -----
$(LIBRARY).c $(LIBRARY).h: $(wildcard $(MODEL_DIR)$(LIBRARY).ov?) Makefile
	$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR) -I $(OPCUA_MODEL_DIR) -I $(KSBASE_MODEL_DIR) -I $(FB_MODEL_DIR) -I $(MODEL_DIR) -f $(MODEL_DIR)$(LIBRARY).ovm -l $(notdir $(basename $<))

%.c %.h: %.ovm Makefile
	$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR) -I $(OPCUA_MODEL_DIR) -I $(KSBASE_MODEL_DIR) -I $(FB_MODEL_DIR) -I $(MODEL_DIR) -f $< -l $(notdir $(basename $<))


%.o: %.c
%.o: %.c $(HEADERS)
	$(COMPILE_C) -o $@ $<

$(USERLIB_LIB) : $(USERLIB_OBJ) $(ADD_LIBS) $(OVLIBS)

$(USERLIB_DLL) : $(USERLIB_OBJ) $(ADD_LIBS) $(OVLIBS)
	$(LD) -o $@ $^ $(LD_FLAGS)

#   Dependencies
#   ------------

.depend:
	touch $@

depend : $(SOURCES)
	$(COMPILE_C) -MM $(USERLIB_SRC) > .depend

#	Aufraeumen
#	----------

clean:
	-@cmd /c del *$(_C) *.h *$(_LIB) *$(_DLL) *$(_OBJ)
