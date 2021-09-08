NAME = edge-impulse-standalone

LINKER_SCRIPT = blinky_gcc_nrf52.ld

OPT = -O3 -g3

__WRLDS__ = 1

CFLAGS += $(OPT)
CFLAGS += -DBOARD_PCA10040
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
#CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DNRF52
CFLAGS += -DNRF52832_XXAA
CFLAGS += -DNRF52_PAN_74
CFLAGS += -DNRF_SD_BLE_API_VERSION=7
CFLAGS += -DS132
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -mfpu=fpv4-sp-d16
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums
CFLAGS += -Wall -g -DTF_LITE_DISABLE_X86_NEON=1 -Wno-strict-aliasing
CFLAGS += -DEIDSP_SIGNAL_C_FN_POINTER=1 -DEI_C_LINKAGE=1
CFLAGS += -DNDEBUG
CFLAGS += -g
CFLAGS += -fPIC

CXXFLAGS += -std=c++11
CXXFLAGS += $(OPT)

LDFLAGS += -lm -lstdc++
LDFLAGS += -fPIC

CSOURCES = edge-impulse-sdk/tensorflow/lite/c/common.c $(wildcard edge-impulse-sdk/CMSIS/DSP/Source/BasicMathFunctions/*.c) $(wildcard edge-impulse-sdk/CMSIS/DSP/Source/FastMathFunctions/*.c) $(wildcard edge-impulse-sdk/CMSIS/DSP/Source/StatisticsFunctions/*.c) $(wildcard edge-impulse-sdk/CMSIS/DSP/Source/TransformFunctions/*fft*.c) $(wildcard edge-impulse-sdk/CMSIS/DSP/Source/CommonTables/*.c) $(wildcard edge-impulse-sdk/CMSIS/DSP/Source/TransformFunctions/*bit*.c)
CSOURCES += $(nRF_SDK)/components/libraries/timer/app_timer.c
CXXSOURCES = $(wildcard tflite-model/*.cpp) $(wildcard edge-impulse-sdk/dsp/kissfft/*.cpp) $(wildcard edge-impulse-sdk/dsp/dct/*.cpp) $(wildcard ./edge-impulse-sdk/dsp/memory.cpp) $(wildcard ./edge-impulse-sdk/classifier/*.cpp) $(wildcard edge-impulse-sdk/porting/wrlds/*.c*)
CCSOURCES = $(wildcard edge-impulse-sdk/tensorflow/lite/kernels/*.cc) $(wildcard edge-impulse-sdk/tensorflow/lite/kernels/internal/*.cc) $(wildcard edge-impulse-sdk/tensorflow/lite/micro/kernels/*.cc) $(wildcard edge-impulse-sdk/tensorflow/lite/micro/*.cc) $(wildcard edge-impulse-sdk/tensorflow/lite/micro/memory_planner/*.cc) $(wildcard edge-impulse-sdk/tensorflow/lite/core/api/*.cc)

CFLAGS += \
  -I$(nRF_SDK)/components \
  -I$(nRF_SDK)/modules/nrfx/mdk \
  -I$(nRF_SDK)/components/libraries/strerror \
  -I$(nRF_SDK)/components/softdevice/s132/headers/nrf52 \
  -I$(nRF_SDK)/components/toolchain/cmsis/include \
  -I$(nRF_SDK)/components/libraries/util \
  -I$(nRF_SDK)/components/libraries/balloc \
  -I$(nRF_SDK)/components/libraries/ringbuf \
  -I$(nRF_SDK)/modules/nrfx/hal \
  -I$(nRF_SDK)/components/libraries/bsp \
  -I$(nRF_SDK)/components/libraries/log \
  -I$(nRF_SDK)/modules/nrfx \
  -I$(nRF_SDK)/components/libraries/experimental_section_vars \
  -I$(nRF_SDK)/components/softdevice/s132/headers \
  -I$(nRF_SDK)/components/libraries/delay \
  -I$(nRF_SDK)/integration/nrfx \
  -I$(nRF_SDK)/components/libraries/atomic \
  -I$(nRF_SDK)/components/boards \
  -I$(nRF_SDK)/components/libraries/memobj \
  -I$(nRF_SDK)/components/softdevice/common \
  -I$(nRF_SDK)/external/fprintf \
  -I$(nRF_SDK)/components/libraries/log/src \
  -I$(nRF_SDK)/components/libraries/timer

ifeq (${CMSIS_NN},1)
CFLAGS += -Wno-unknown-attributes
CFLAGS += -DEI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN=1 -D__ARM_FEATURE_DSP=1 -D__GNUC_PYTHON__=1
CFLAGS += -Iedge-impulse-sdk/CMSIS/NN/Include/
CFLAGS += -Iedge-impulse-sdk/CMSIS/DSP/PrivateInclude/
CSOURCES += $(wildcard edge-impulse-sdk/CMSIS/NN/Source/ActivationFunctions/*.c) $(wildcard edge-impulse-sdk/CMSIS/NN/Source/BasicMathFunctions/*.c) $(wildcard edge-impulse-sdk/CMSIS/NN/Source/ConcatenationFunctions/*.c) $(wildcard edge-impulse-sdk/CMSIS/NN/Source/ConvolutionFunctions/*.c) $(wildcard edge-impulse-sdk/CMSIS/NN/Source/FullyConnectedFunctions/*.c) $(wildcard edge-impulse-sdk/CMSIS/NN/Source/NNSupportFunctions/*.c) $(wildcard edge-impulse-sdk/CMSIS/NN/Source/PoolingFunctions/*.c) $(wildcard edge-impulse-sdk/CMSIS/NN/Source/ReshapeFunctions/*.c) $(wildcard edge-impulse-sdk/CMSIS/NN/Source/SoftmaxFunctions/*.c)
endif

COBJECTS := $(patsubst %.c,%.o,$(CSOURCES))
CXXOBJECTS := $(patsubst %.cpp,%.o,$(CXXSOURCES))
CCOBJECTS := $(patsubst %.cc,%.o,$(CCSOURCES))

ifeq ($(OS),Windows_NT)
	MKDIR_BUILD = if not exist build mkdir build
else
	MKDIR_BUILD = mkdir -p build
endif

all: build/libedgeimpulse.so

TEMPLATE_PATH := $(nRF_SDK)/components/toolchain/gcc
include $(TEMPLATE_PATH)/Makefile.common

.PHONY: app clean

$(COBJECTS) : %.o : %.c
$(CXXOBJECTS) : %.o : %.cpp
$(CCOBJECTS) : %.o : %.cc

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

%.o: %.cc
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c $^ -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c $^ -o $@

build/libedgeimpulse.so: $(COBJECTS) $(CXXOBJECTS) $(CCOBJECTS)
	$(MKDIR_BUILD)
	$(CXX) $(COBJECTS) $(CXXOBJECTS) $(CCOBJECTS) -shared -o build/libedgeimpulse.so $(LDFLAGS)

clean:
ifeq ($(OS),Windows_NT)
	del /Q $(subst /,\,$(patsubst %.c,%.o,$(CSOURCES))) >nul 2>&1 || exit 0
	del /Q $(subst /,\,$(patsubst %.cpp,%.o,$(CXXSOURCES))) >nul 2>&1 || exit 0
	del /Q $(subst /,\,$(patsubst %.cc,%.o,$(CCSOURCES))) >nul 2>&1 || exit 0
	del /Q $(subst /,\,$(patsubst %.c,%.o,$(APP_COBJECTS))) >nul 2>&1 || exit 0
	del /Q build\libedgeimpulse.so >nul 2>&1 || exit 0
else
	rm -f $(COBJECTS)
	rm -f $(CCOBJECTS)
	rm -f $(CXXOBJECTS)
	rm -f build/libedgeimpulse.so
endif
