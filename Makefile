src := src/main.c src/renderer.c src/window.c src/instance.c
includes := include/c_vector/vec.c
libs := -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
CC := gcc
flags := -O2 -Wall
target := bin/main.out

all: main

main: ${src}
	${CC} -o ${target} ${includes} ${src} ${libs} ${flags}

run:
	./${target}

clean:
	rm ${target} && clear

check:
	valgrind --leak-check=yes ./${target}

deepCheck:
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s ./${target}
