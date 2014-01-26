# Recursive Wildcard Function
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

CFLAGS= -I ../include -fno-stack-protector

# Sources
C_SRC=$(call rwildcard, ../src/, *.c)
ASM_SRC=$(call rwildcard, ../src/, *.asm)

C_OBJ_TMP=$(C_SRC:.c=.o)
ASM_OBJ_TMP=$(ASM_SRC:.asm=)

C_OBJ=$(filter-out ../src/boot/bootloader/x86/kernel.o,$(C_OBJ_TMP))
ASM_OBJ=$(filter-out ../src/boot/bootloader/x86/bootloader,$(ASM_OBJ_TMP))

all: t build floppy clean
#iso

t:
	@echo $(C_OBJ)
	@echo $(ASM_OBJ)

build: $(C_OBJ) $(ASM_OBJ)
	mkdir -p ../bin
	mkdir -p ../tmp
# compile the bootloader
	nasm -f bin -o ../tmp/bootsect ../src/boot/bootloader/x86/bootloader.asm
# compile the kernel
	gcc -m32 -c -o ../tmp/kernel.o ../src/boot/bootloader/x86/kernel.c $(CFLAGS)

%.o: %.c
	gcc -m32 -c -o $@ $< $(CFLAGS)

%: %.asm
#	mkdir -p $(dir %.asmo) -> à créer dans tmp
	nasm -f elf -o $@ $<

floppy:
# kernel is set to offset 0x1000
	ld --oformat binary -m elf_i386 -Ttext 1000 ../tmp/kernel.o $(ASM_OBJ) $(C_OBJ) -o ../tmp/kernel
# create a floppy image which executes the kernel
	cat ../tmp/bootsect ../tmp/kernel /dev/zero | dd of=../bin/floppyA bs=512 count=2880


iso:
# kernel for grub is set to offset 0x100000
	ld --oformat binary -m elf_i386 -Ttext 1000 ../tmp/kernel.o ../tmp/screen.o -o ../tmp/kernel
#
	rm -f ../bin/os.iso
	bximage  -q ../bin/os.iso -hd -mode=flat -size=10
	(echo x; echo c; echo 20; echo h; echo 16; echo s; echo 63; echo r; echo n; echo p; echo 1;echo ; echo ; echo a; echo 1; echo "w";) | fdisk ../bin/os.iso
	sudo losetup /dev/loop0 ../bin/os.iso
	sudo losetup -o 32256 --sizelimit 5644800 /dev/loop1 ../bin/os.iso
#	sudo losetup -o 5677056 /dev/loop2 c.iso
	sudo mke2fs -O ^resize_inode /dev/loop1
	sudo mkdir -p /mnt/virtual
	sudo mount -t ext2 /dev/loop1 /mnt/virtual
	sudo mkdir -p /mnt/virtual/boot/grub
	sudo touch /mnt/virtual/boot/grub/grub.cfg
	echo "(hd0) /dev/loop0\n(hd0,1) /dev/loop1" | sudo tee -a /mnt/virtual/boot/grub/device.map	
	echo "set default=0\nset timeout=5\nset root=(hd0,1)\nmenuentry \"AdaKernel\" {\n  multiboot /boot/kernel\n  boot\n}\n" | sudo tee -a /mnt/virtual/boot/grub/grub.cfg	
	sudo cp ../tmp/kernel /mnt/virtual/boot/kernel
	sudo grub-install --root-directory=/mnt/virtual /dev/loop0
	sudo umount /mnt/virtual
	sudo losetup -d /dev/loop1
	sudo losetup -d /dev/loop0
	sudo chmod 0777 ../bin/os.iso 
	
clean:
	rm -f ../src/boot/bootloader/x86/kernel.o
	rm -f ../src/boot/bootloader/x86/bootloader
	rm -f $(C_OBJ)
	rm -f $(ASM_OBJ)

mrproper: clean
	rm -rf ../tmp
	rm -rf ../bin
