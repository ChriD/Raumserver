# Makefile template for shared library

ifeq ($(arch),X64) 
  ARCHITECTURE = -m64
  CC = g++-4.9
  CXX = g++-4.9
  LIBSPATH = libs/linux_x64/
  TARGET = raumsrvDaemon
endif
	
ifeq ($(arch),X86) 
  ARCHITECTURE = -m32 
  CC = g++-4.9
  CXX = g++-4.9
  LIBSPATH = libs/linux_x86/
  TARGET = raumsrvDaemon
endif

ifeq ($(arch),ARMA53)
  ARCHITECTURE = -march=armv8-a
  CC = arm-linux-gnueabihf-g++-4.9 
  CXX = arm-linux-gnueabihf-g++-4.9
  LIBSPATH = libs/linux_ARMA53/
  TARGET = raumsrvDaemon
endif

ifeq ($(arch),ARMV7HF)
  ARCHITECTURE = -march=armv7-a
  CC = arm-linux-gnueabihf-g++-4.9 
  CXX = arm-linux-gnueabihf-g++-4.9
  LIBSPATH = libs/linux_ARMV7/
  TARGET = raumsrvDaemon
endif

ifeq ($(arch),ARMV6)
  ARCHITECTURE = -march=armv6
  CC = arm-linux-gnueabi-g++-4.9 
  CXX = arm-linux-gnueabi-g++-4.9
  LIBSPATH = libs/linux_ARMV6/
  TARGET = raumsrvDaemon
endif

LIBSDEF = $(LIBSPATH)raumkernel.so $(LIBSPATH)raumserver.so


DEBUG = 
ifeq ($(dbg),1)
  DEBUG = -g
endif

INCPATH = -I includes/ -I ../../../RaumkernelLib/source/includes/ -I ../../../RaumserverLib/source/includes/

CFLAGS = $(ARCHITECTURE) -std=c++11 -fPIC -Wall -Wno-unknown-pragmas -Wno-unused-parameter -Wunused-value -Wextra -O2 $(DEBUG) -c -pthread $(INCPATH)
CXXFLAGS = $(ARCHITECTURE) -std=c++11 -fPIC -Wall -Wno-unknown-pragmas -Wno-unused-parameter -Wunused-value -Wextra -O2 $(DEBUG) -c -pthread $(INCPATH)
#LDFLAGS = $(ARCHITECTURE) -Wl,-rpath,libs/ -Wl,--no-as-needed -pthread -static-libgcc -static-libstdc++ -ldl
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
