ARM_TOOLCHAIN_PREFIX=arm-none-eabi
ARM_AS=$(ARM_TOOLCHAIN_PREFIX)-as
ARM_LD=$(ARM_TOOLCHAIN_PREFIX)-ld
ARM_OBJCPY=$(ARM_TOOLCHAIN_PREFIX)-objcopy

all: unicorn thumb.bin

unicorn: unicorn.c 
	$(CC) unicorn.c -o unicorn -lunicorn

thumb.bin: thumb
	$(ARM_OBJCPY) -O binary thumb thumb.bin


thumb: thumb.S
	$(ARM_AS) thumb.S -o thumb.o
	$(ARM_LD) thumb.o -o thumb

test: all
	./unicorn thumb.bin

clean:
	rm *.o *.bin
	find -executable -type f -exec rm {} \;

.PHONY: test clean
