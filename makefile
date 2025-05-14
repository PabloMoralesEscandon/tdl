# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build

# Compiler and flags
CC = gcc
CFLAGS = -I$(INCDIR) -g -O0
LDFLAGS = -ljansson

# Source and object files
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC))

# Target executable
TARGET = $(BUILDDIR)/myprog

# Default target: build everything
all: $(BUILDDIR) $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# Compile each .c file in src/ to .o in build/
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Make sure build directory exists
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Clean up build artifacts
clean:
	rm -rf $(BUILDDIR)
