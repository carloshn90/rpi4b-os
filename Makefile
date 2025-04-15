BUILD_DIR = build
CFILES = $(wildcard *.c)
OFILES = $(patsubst %.c,$(BUILD_DIR)/%.o,$(CFILES))
GCCFLAGS = -std=c99 -ffreestanding -mgeneral-regs-only


all: clean $(BUILD_DIR)/kernel8.img

# Ensure build directory exists before compiling
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.o: boot.S | $(BUILD_DIR)
	aarch64-linux-gnu-gcc $(GCCFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	aarch64-linux-gnu-gcc $(GCCFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel8.img: $(BUILD_DIR)/boot.o $(OFILES)
	aarch64-linux-gnu-ld -nostdlib $(BUILD_DIR)/boot.o $(OFILES) -T link.ld -o $(BUILD_DIR)/kernel8.elf
	aarch64-linux-gnu-objcopy -O binary $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/kernel8.img

clean:
	rm -rf $(BUILD_DIR)
