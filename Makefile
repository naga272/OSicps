RUST_PROJECT_DIR=./src/utilities/icps

FILES = ./build/kernel.asm.o ./build/kernel.o ./build/video/video.o ./build/video/panic.o ./build/idt/idt.asm.o ./build/idt/idt.o ./build/memory/heap.o ./build/memory/kheap.o ./build/io/io.asm.o ./build/stdlib/stdlib.o  
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc


all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin


./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin


./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o


./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o


./build/kernel.o: ./src/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -I/usr/include/python3.12 -c ./src/kernel.c -o ./build/kernel.o


./build/memory/heap.o: ./src/utilities/memory/heap.c
	# compilazione file ./src/utilities/memory/heap.c
	i686-elf-gcc $(INCLUDES) -I./src/utilities/memory $(FLAGS) -std=gnu99 -c ./src/utilities/memory/heap.c -o ./build/memory/heap.o


./build/memory/kheap.o: ./src/utilities/memory/kheap.c
	# compilazione file ./src/utilities/memory/kheap.c
	i686-elf-gcc $(INCLUDES) -I./src/utilities/memory $(FLAGS) -std=gnu99 -c ./src/utilities/memory/kheap.c -o ./build/memory/kheap.o


./build/stdlib/stdlib.o: ./src/utilities/stdlib/stdlib.c
	# compilazione file ./src/stdlib/stdlib.c
	i686-elf-gcc $(INCLUDES) -I./src/utilities/stdlib/ $(FLAGS) -std=gnu99 -c ./src/utilities/stdlib/stdlib.c -o ./build/stdlib/stdlib.o


./build/idt/idt.asm.o: ./src/utilities/idt/idt.asm
	# compilazione file ./src/utilities/idt/idt.asm
	nasm -f elf -g ./src/utilities/idt/idt.asm -o ./build/idt/idt.asm.o


./build/idt/idt.o: ./src/utilities/idt/idt.c
	# compilazione file ./src/utilities/idt/idt.c
	i686-elf-gcc $(INCLUDES) -I./src/idt $(FLAGS) -std=gnu99 -c ./src/utilities/idt/idt.c -o ./build/idt/idt.o


./build/io/io.asm.o: ./src/utilities/io/io.asm
	# compilazione file ./src/utilities/io/io.asm
	nasm -f elf -g ./src/utilities/io/io.asm -o ./build/io/io.asm.o


./build/video/video.o: ./src/utilities/video/print/video.c
	i686-elf-gcc $(INCLUDES) -I./src/utilities/video/print $(FLAGS) -std=gnu99 -c ./src/utilities/video/print/video.c -o ./build/video/video.o


./build/video/panic.o: ./src/utilities/video/panic/panic.c
	i686-elf-gcc $(INCLUDES) -I./src/utilities/video/panic/ $(FLAGS) -std=gnu99 -c ./src/utilities/video/panic/panic.c -o ./build/video/panic.o


run:
	qemu-system-x86_64 -hda ./bin/os.bin


clean:
	rm -rf ./bin/boot.bin
	rm -rf ./bin/kernel.bin
	rm -rf ./bin/os.bin
	rm -rf ${FILES}
	rm -rf ./build/kerne
	clear
