# Makefile
DTARGET := build/raumsrvDaemonDyn
STARGET := build/raumsrvDaemonStat

# defining the source files for the project
SRCFILES := $(wildcard *.cpp)

INCPATH = -I includes/ -I ../../../RaumkernelLib/source/includes/ -I ../../../RaumserverLib/source/includes/ -I includes/libunwind/
#LIBSPATH    := build/linux_$(arch)/libs/
LIBSPATH    := libs/linux_$(arch)/
LIBSPATHEXE := libs
DLIBSDEF    :=  -Bdynamic -lraumserver -lraumkernel -lunwind
SLIBSDEF    :=  -Bstatic libs/linux_$(arch)/libraumserver.a libs/linux_$(arch)/libraumkernel.a libs/linux_$(arch)/libohNetCore.a libs/linux_$(arch)/libohNetDevices.a libs/linux_$(arch)/libohNetProxies.a -Bdynamic -lunwind


ifeq ($(arch),) 
  COMPILER          := g++
  ARCHITECTURE      := 
  ARCHCOMPILERFLAGS :=
  ARCHDLIBSDEF	    :=
  ARCHSLIBSDEF	    :=
  ARCHLINKERFLAGS   :=
endif

ifeq ($(arch),X64) 
  COMPILER          := g++-5
  ARCHITECTURE      := -m64
  ARCHCOMPILERFLAGS :=
  ARCHDLIBSDEF	    := -lunwind-x86_64
  ARCHSLIBSDEF	    := -lunwind-x86_64
  ARCHLINKERFLAGS   :=
endif

ifeq ($(arch),ARMV7HF)
  COMPILER          := arm-linux-gnueabihf-g++-5
  ARCHITECTURE      := -march=armv7-a
  ARCHCOMPILERFLAGS := -fasynchronous-unwind-tables -mapcs-frame -mthumb -mtpcs-frame -mtpcs-leaf-frame
  ARCHDLIBSDEF	    := -lunwind-arm -llzma
  ARCHSLIBSDEF	    := -lunwind-arm -llzma
  ARCHLINKERFLAGS   :=
endif

ifeq ($(arch),ARMV5TE)
  COMPILER          := arm-linux-gnueabi-g++-5 
  ARCHITECTURE      := -march=armv5te -mtune=xscale
  ARCHCOMPILERFLAGS := -fasynchronous-unwind-tables -mapcs-frame -mthumb -mtpcs-frame -mtpcs-leaf-frame
  ARCHDLIBSDEF	    := -lunwind-arm -llzma
  ARCHSLIBSDEF	    := -lunwind-arm -llzma
  ARCHLINKERFLAGS   :=
endif 


DLIBSDEF := $(DLIBSDEF) $(ARCHDLIBSDEF)
SLIBSDEF := $(SLIBSDEF) $(ARCHSLIBSDEF)


COMPILERFLAGS :=  -std=c++11 -fPIC -Wall -Wno-unknown-pragmas -Wno-unused-parameter -funwind-tables -Wextra -O0 -c -pthread -fno-omit-frame-pointer 
#LINKERFLAGS   :=  -pthread -static-libgcc -static-libstdc++ -rdynamic -Wl,--no-as-needed -ldl -L$(LIBSPATH) -Wl,-rpath,$(LIBSPATHEXE)
LINKERFLAGS   :=  -pthread -rdynamic -static-libstdc++ -Wl,--no-as-needed -ldl -L$(LIBSPATH) -Wl,-rpath,$(LIBSPATHEXE)  


DEBUG = 
ifeq ($(dbg),1)
  DEBUG = -g
endif

DLINKERFLAGS  	 := $(ARCHITECTURE) $(LINKERFLAGS) $(ARCHLINKERFLAGS)
SLINKERFLAGS  	 := $(ARCHITECTURE) $(LINKERFLAGS) $(ARCHLINKERFLAGS)
DCOMPILERFLAGS   := $(ARCHITECTURE) $(COMPILERFLAGS) $(DEBUG) $(ARCHCOMPILERFLAGS) $(INCPATH)
SCOMPILERFLAGS   := $(ARCHITECTURE) $(COMPILERFLAGS) $(DEBUG) $(ARCHCOMPILERFLAGS) $(INCPATH)

RM  := rm -f 
RMR := rm -rf

DOBJDIR := build/obj-dyn/
SOBJDIR := build/obj-stat/

DOBJFILES := $(addprefix $(DOBJDIR), $(SRCFILES:.cpp=.o))
SOBJFILES := $(addprefix $(SOBJDIR), $(SRCFILES:.cpp=.o))


.PHONY: all


### when calling make then build static and dynamic target
all: ${DTARGET} ${STARGET}
	
### create dynamic library
$(DTARGET): $(DOBJFILES)	
	$(COMPILER) ${DLINKERFLAGS} -o $@ $^ $(DLIBSDEF) 

$(DOBJDIR)%.o: %.cpp
	@ mkdir -p $(basename $@)
	$(COMPILER) $(DCOMPILERFLAGS) -MMD -c -o $@ $<

-include $(DOBJFILES:.o=.d)


### create static library
$(STARGET): $(SOBJFILES)	
	$(COMPILER) ${SLINKERFLAGS} -o $@ $^ $(SLIBSDEF) 

$(SOBJDIR)%.o: %.cpp
	@ mkdir -p $(basename $@)
	$(COMPILER) $(SCOMPILERFLAGS) -MMD -c -o $@ $<

-include $(SOBJFILES:.o=.d)



### clear all build relevant files 
.PHONY: clean
clean:
	-${RM} ${DTARGET} ${STARGET} ${DOBJFILES} ${SOBJFILES} $(DOBJFILES:.o=.d) 
	-${RMR} ${DOBJDIR} ${SOBJDIR}
