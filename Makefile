# Variables
CC = emcc
CFLAGS = -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall', 'setValue']" -s "EXPORTED_FUNCTIONS=['_malloc', '_free', '_main']"
SHELL_FILE = html_template/shell_minimal.html
SRC = main.c
OUT = dist/index.html
RUN_CC = emrun
RUN_CFLAGS = --browser chrome

# Default target
all: build

copy-styles:
	cp html_template/styles.css dist/styles.css

# Build target
build:
	$(CC) -o $(OUT) $(SRC) --shell-file $(SHELL_FILE) $(CFLAGS) && make copy-styles
	
# Run target
run:
	${RUN_CC} $(RUN_CFLAGS) $(OUT)
