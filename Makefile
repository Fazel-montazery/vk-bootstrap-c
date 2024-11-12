src := src/main.c src/state.c src/utils.c src/renderer.c src/window.c src/instance.c src/device.c src/queue.c src/surface.c src/swap_chain.c src/graphics_pipeline.c src/framebuffer.c src/command.c src/sync.c
includes := include/c_vector/vec.c
libs := -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
CC := gcc
flags := -Wall -Wunused
target := bin/main.out

all: main

main: ${src}
	${CC} -o ${target} ${includes} ${src} src/debug.c src/validation_layers.c ${libs} ${flags} -Os -g

release: ${src}
	${CC} -o ${target} ${includes} ${src} ${libs} -O2 -DNDEBUG

run:
	./${target}

clean:
	rm ${target} && clear

check:
	valgrind --leak-check=yes ./${target}

deepCheck:
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s ./${target}
