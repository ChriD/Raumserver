# Makefile template for shared library

ifeq ($(arch),X64) 
  ARCHITECTURE = -m64 -D_GLIBCXX_USE_CXX11_ABI=0
  CC = g++-5
  CXX = g++-5 
  LIBSPATH = libs/linux_x64/
  LIBSDEFADD = $(LIBSPATH)libsciter-gtk-64.so
  TARGET = raumsrvInstaller
endif
	
ifeq ($(arch),X86) 
  ARCHITECTURE = -m32 
  CC = g++-4.9
  CXX = g++-4.9
  LIBSPATH = libs/linux_x86/
  LIBSDEFADD =
  TARGET = raumsrvInstaller
endif

ifeq ($(arch),ARMA53)
  ARCHITECTURE = -march=armv8-a
  CC = arm-linux-gnueabihf-g++-4.9 
  CXX = arm-linux-gnueabihf-g++-4.9
  LIBSPATH = libs/linux_ARMA53/
  LIBSDEFADD =
  TARGET = raumsrvInstaller
endif

ifeq ($(arch),ARMV7HF)
  ARCHITECTURE = 
  CC = arm-linux-gnueabihf-g++-4.9 
  CXX = arm-linux-gnueabihf-g++-4.9
  LIBSPATH = libs/linux_ARMV7/
  LIBSDEFADD =
  TARGET = raumsrvInstaller
endif

ifeq ($(arch),ARMV6)
  ARCHITECTURE = -march=armv6
  CC = arm-linux-gnueabi-g++-4.9 
  CXX = arm-linux-gnueabi-g++-4.9
  LIBSPATH = libs/linux_ARMV6/
  LIBSDEFADD =
  TARGET = raumsrvInstaller
endif

GTKLIBS = -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0
LIBSDEF =  $(LIBSPATH)raumserverInstaller.so $(LIBSPATH)libssh.so $(LIBSPATH)libssh_threads.so $(LIBSDEFADD) -ldl -fPIC $(GTKLIBS)

DEBUG = 
ifeq ($(dbg),1)
  DEBUG = -g
endif

INCPATH = -I includes/ -I includes/sciter/ -I ../../../RaumserverInstallerLib/source/includes/ 

CFLAGS = $(ARCHITECTURE) -std=c++11 -fPIC -Wall -Wno-unknown-pragmas -Wno-unused-parameter -Wextra -O2 $(DEBUG) -c -pthread $(INCPATH) `pkg-config --cflags --libs gtk+-3.0`
CXXFLAGS = $(ARCHITECTURE) -std=c++11 -fPIC -Wall -Wno-unknown-pragmas -Wno-unused-parameter -Wextra -O2 $(DEBUG) -c -pthread $(INCPATH) `pkg-config --cflags --libs gtk+-3.0`
#LDFLAGS = $(ARCHITECTURE) -pthread -static-libgcc -static-libstdc++
LDFLAGS = $(ARCHITECTURE) -pthread

RM = rm -f 

#SRCS = $(wildcard *.cpp)
SRCS += raumserverInstaller.cpp
SRCS += raumserverInstallerView.cpp
SRCS += $(wildcard json/*.cpp)
SRCS += includes/sciter/sciter-gtk-main.cpp

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
