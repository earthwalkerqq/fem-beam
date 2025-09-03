CC := gcc
CFLAGS = -Wall -Werror -Wextra
ARFLAGS = rcs

MAIN_FILE = main.c

OS := $(shell uname -s)

INCLUDE_FLAG = -Iinc

BUILD_FOLDER = ../build
FEM_LIB = $(BUILD_FOLDER)/libfem.a

SRC_FILES = $(wildcard src/*.c)
OBJ_FILES = $(patsubst src/%.c,$(BUILD_FOLDER)/%.o,$(SRC_FILES))

ifeq ($(OS), Darwin)
    LIBOMP_PATH := $(shell brew --prefix libomp)
    CHECK_PATH := $(shell brew --prefix check)
    OPENMP_FLAG = -Xpreprocessor -fopenmp
else
    LIBOMP_PATH := /usr/lib/libomp
    CHECK_PATH := /usr/lib/check
    OPENMP_FLAG = -fopenmp
endif

.PHONY: all fem build rebuild clean check_deps style_check

all: build

build: check_deps fem

# итоговый бинарь (не .o!)
fem: $(FEM_LIB) $(MAIN_FILE)
	$(CC) $(CFLAGS) $(INCLUDE_FLAG) $(MAIN_FILE) $(FEM_LIB) -o $(BUILD_FOLDER)/fem

$(BUILD_FOLDER):
	mkdir -p $(BUILD_FOLDER)

$(FEM_LIB): $(OBJ_FILES)
	ar $(ARFLAGS) $@ $^
	rm -rf $(BUILD_FOLDER)/*.o

$(BUILD_FOLDER)/%.o: src/%.c | $(BUILD_FOLDER)
	$(CC) $(CFLAGS) $(INCLUDE_FLAG) -c $< -o $@

check_deps:
ifeq ($(OS), Darwin)
	@echo "Checking dependencies for MacOS..."
	@brew update
	@if ! brew list libomp >/dev/null 2>&1; then \
		echo "Installing libomp..."; \
		brew install libomp; \
	fi
	@if ! brew list check >/dev/null 2>&1; then \
		echo "Installing check..."; \
		brew install check; \
	fi
	$(eval LIBOMP_PATH := $(shell brew --prefix libomp))
	$(eval CHECK_PATH := $(shell brew --prefix check))
else
	@echo "Checking dependencies for Linux..."
	@if ! dpkg -l | grep -q libomp-dev; then \
		echo "Installing libomp-dev..."; \
		sudo apt update && sudo apt install -y libomp-dev; \
	fi
	@if ! dpkg -l | grep -q check; then \
		echo "Installing check..."; \
		sudo apt update && sudo apt install -y check; \
	fi
endif

style_check:
	mv ../materials/linters/.clang-format ./.clang-format
	clang-format -i *.c *.h
	rm ./.clang-format

clean:
	rm -rf $(BUILD_FOLDER)
	rm -rf ./*.o
	rm -rf ./*.out

rebuild: clean build
