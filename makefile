# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build

# Compiler and flags
CC = clang
CFLAGS = -I$(INCDIR) -g -O0
LDFLAGS = -ljansson

# Source and object files
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC))

# Targets
MYPROG = $(BUILDDIR)/myprog
TDL = tdl

# Default target: build everything
all: $(BUILDDIR) $(MYPROG) $(TDL)

# Build myprog (debug in build/)
$(MYPROG): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# Build tdl (symlink/copy to root)
$(TDL): $(MYPROG)
	cp $< $@

# Compile each .c file in src/ to .o in build/
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Make sure build directory exists
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Clean up build artifacts
clean:
	rm -rf $(BUILDDIR) $(TDL)

# Debug target
debug: all
	@echo "Debug build ready: $(MYPROG), root symlink: $(TDL)"
