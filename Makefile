CC = clang
LD = ld.lld
AS = nasm
CFLAGS = -target i386-elf -Wall -Wextra -O2 -std=c11 -ffreestanding -I include -MMD -MP -mno-sse -mno-mmx -mno-sse2 -mno-3dnow
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib
SRCS = $(shell find src -name '*.c')
ASM_SRCS = $(shell find src -name '*.asm')
OBJS = $(SRCS:%.c=build/%.o) $(ASM_SRCS:%.asm=build/%.o)
DEPS = $(OBJS:%.o=%.d)

LOG_INFO = \033[1;32m
LOG_RESET = \033[0m

all: build/os.bin
build/%.o: %.c
	@echo -e "$(LOG_INFO)Compile C object: $@$(LOG_RESET)"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.asm
	@echo -e "$(LOG_INFO)Compile ASM object: $@$(LOG_RESET)"
	@mkdir -p $(dir $@)
	$(AS) -f elf32 $< -o $@

build/os.bin: $(OBJS)
	@echo -e "$(LOG_INFO)Link: $@$(LOG_RESET)"
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

iso: build/os.bin
	@echo -e "$(LOG_INFO)Make ISO$(LOG_RESET)"
	@mkdir -p build/isodir/boot/grub
	cp build/os.bin build/isodir/boot/os.bin
	@echo 'menuentry "StardanceOS" {' > build/isodir/boot/grub/grub.cfg
	@echo '		multiboot2 /boot/os.bin' >> build/isodir/boot/grub/grub.cfg
	@echo '		boot' >> build/isodir/boot/grub/grub.cfg
	@echo '}' >> build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/os.iso build/isodir

run: iso
	@echo -e "$(LOG_INFO)Run$(LOG_RESET)"
	qemu-system-i386 -cdrom build/os.iso -serial stdio

clean:
	rm -rfv build

-include $(DEPS)