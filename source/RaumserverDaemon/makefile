# Makefile template for shared library

ifeq ($(arch),X64) 
  ARCHITECTURE = -m64
  CC = g++-4.9
  CXX = g++-4.9
  LIBSDEF =  libs/linux_x64/raumkernel.so libs/linux_x64/raumserver.so
  TARGET = raumsrvDaemon
endif
	
ifeq ($(arch),X86) 
  ARCHITECTURE = -m32 
  CC = g++-4.9
  CXX = g++-4.9
  LIBSDEF =  libs/linux_x86/raumkernel.so libs/linux_x86/raumserver.so
  TARGET = raumsrvDaemon
endif

ifeq ($(arch),ARMA53)
  ARCHITECTURE = -march=armv8-a
  CC = arm-linux-gnueabihf-g++-4.9 
  CXX = arm-linux-gnueabihf-g++-4.9
  LIBSDEF = libs/linux_ARMA53/raumkernel.so libs/linux_ARMA53/raumserver.so
  TARGET = raumsrvDaemon
endif

ifeq ($(arch),ARMV7HF)
  ARCHITECTURE = 
  CC = arm-linux-gnueabihf-g++-4.9 
  CXX = arm-linux-gnueabihf-g++-4.9
  LIBSDEF = libs/linux_ARMV7/raumkernel.so libs/linux_ARMV7/raumserver.so
  TARGET = raumsrvDaemon
endif

ifeq ($(arch),ARMV6)
  ARCHITECTURE = -march=armv6
  CC = arm-linux-gnueabi-g++-4.9 
  CXX = arm-linux-gnueabi-g++-4.9
  LIBSDEF = libs/linux_ARMV6/raumkernel.so libs/linux_ARMV6/raumserver.so
  TARGET = raumsrvDaemon
endif


DEBUG = 
ifeq ($(dbg),1)
  DEBUG = -g
endif

INCPATH = -I includes/ -I ../../../RaumkernelLib/source/includes/ -I ../../../RaumserverLib/source/includes/

CFLAGS = $(ARCHITECTURE) -std=c++11 -fPIC -Wall -Wno-unknown-pragmas -Wno-unused-parameter -Wextra -O2 $(DEBUG) -c -pthread $(INCPATH)
CXXFLAGS = $(ARCHITECTURE) -std=c++11 -fPIC -Wall -Wno-unknown-pragmas -Wno-unused-parameter -Wextra -O2 $(DEBUG) -c -pthread $(INCPATH)
LDFLAGS = $(ARCHITECTURE) -Wl,--no-as-needed -pthread -static-libgcc -static-libstdc++ -ldl


RM = rm -f 

SRCS = $(wildcard *.cpp)

OBJS = $(SRCS:.cpp=.o)


.PHONY: all
all: ${TARGET}

$(TARGET): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^ $(LIBSDEF)
 
$(SRCS:.cpp=.d):%.d:%.cpp
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.cpp=.d)


#.PHONY: clean
#clean:
#	-${RM} ${TARGET} ${OBJS} $(SRCS:.cpp=.d) 
.PHONY: clean
clean:
	-${RM} ${TARGET} ${OBJS} $(SRCS:.cpp=.d) 
