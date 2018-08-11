DEBUG = no
ifeq ($(DEBUG),yes)
	OPT = -g3 -O0 
endif

ifeq ($(DEBUG),no)
	OPT = -O2
endif

BUILD_DIR = build
TARGET = Template

BINPATH = E:/riscv-none-gcc-win32/bin
PREFIX = riscv-none-embed-
CC = $(BINPATH)/$(PREFIX)gcc
AS = $(BINPATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(BINPATH)/$(PREFIX)objcopy
AR = $(BINPATH)/$(PREFIX)ar
SZ = $(BINPATH)/$(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

ABI = ilp32
ARCH = rv32im

C_SOURCES =  \
$(wildcard *.c)	\
$(wildcard lib/*.c)	\
$(wildcard src/*.c)	\
$(wildcard dep/freertos/*.c)	\
$(wildcard dep/freertos/portable/GCC/RISCV/*.c)

ASM_SOURCES =  \
startup.s

C_INCLUDES = \
-Isrc	\
-Ilib	\
-Iinc	\
-Idep/freertos/include	\
-Idep/freertos/portable/GCC/RISCV

ASFLAGS = -mabi=$(ABI) -march=$(ARCH) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS += -mabi=$(ABI) -march=$(ARCH) $(OPT) $(C_INCLUDES) -Wall -fdata-sections -ffunction-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"
LIBS = -lc -lm -specs=nano.specs -specs=nosys.specs
LIBDIRS = 
LDFLAGS += -mabi=$(ABI) -march=$(ARCH) -mcmodel=medany -Tvrlt.lds $(LIBS) $(LIBDIRS) -nostartfiles -Wl,-Map,$(BUILD_DIR)/$(TARGET).map -Wl,--print-memory-usage -Wl,--gc-sections

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

all: $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@
	
$(BUILD_DIR):
	mkdir $@

clean:
	-rm -fR .dep $(BUILD_DIR)
  
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
