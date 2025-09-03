CC := gcc
CFLAGS = -Wall -Werror -Wextra

MATH_FALG = -lm

BUILD_FOLDER = ../build

all: build

build:

style_check:
	mv ../materials/linters/.clang-format ./.clang-format
	clang-format -i *.c *.h
	rm ./.clang-format

clean:
	rm -rf ../build
	rm -rf ./*.o
	rm -rf ./*.out

rebuild: clean build