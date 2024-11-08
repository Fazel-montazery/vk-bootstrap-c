src := src/main.c src/renderer.c src/window.c
libs := -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
CC := gcc
flags := -O2 -Wall
target := bin/main.out

all: main

main: ${src}
	${CC} -o ${target} ${src} ${libs} ${flags}

run:
	./${target}

clean:
	rm ${target} && clear
