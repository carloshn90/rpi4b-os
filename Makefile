CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -std=c99 -ffreestanding -mgeneral-regs-only
BUILDDIR = build

all: clean kernel8.img 
	
boot.o: boot.S
	mkdir -p $(BUILDDIR)
	aarch64-linux-gnu-gcc $(GCCFLAGS) -c $< -o $(BUILDDIR)/$@
	
%.o: %.c
	aarch64-linux-gnu-gcc $(GCCFLAGS) -c $< -o $(BUILDDIR)/$@

kernel8.img: boot.o $(OFILES)
	aarch64-linux-gnu-ld -nostdlib $(BUILDDIR)/boot.o $(BUILDDIR)/$(OFILES) -T link.ld -o $(BUILDDIR)/kernel8.elf
	aarch64-linux-gnu-objcopy -O binary $(BUILDDIR)/kernel8.elf $(BUILDDIR)/kernel8.img

clean:
	rm -r build > /dev/null 2> /dev/null || true
