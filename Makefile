# Variables
CC = emcc
CFLAGS = -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']"
SHELL_FILE = html_template/shell_minimal.html
SRC = main.c
OUT = index.html
RUN_CC = emrun
RUN_CFLAGS = --browser chrome

# Default target
all: build

# Build target
build:
	$(CC) -o $(OUT) $(SRC) --shell-file $(SHELL_FILE) $(CFLAGS)

# Run target
run:
	${RUN_CC} $(RUN_CFLAGS) $(OUT)
