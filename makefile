# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build

# Compiler and flags
CC = clang
WARNFLAGS = -Wall -Wextra
CFLAGS = -I$(INCDIR) $(WARNFLAGS) -g -O0
LDFLAGS = -ljansson

# Source and object files
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC))

# Targets
MYPROG = $(BUILDDIR)/myprog
TDL = tdl

# Default target: debug-style build
all: $(BUILDDIR) $(MYPROG) $(TDL)

# Release target: optimized, no debug, disables assert()
release: CFLAGS := -I$(INCDIR) $(WARNFLAGS) -O3 -DNDEBUG
release: clean $(BUILDDIR) $(MYPROG) $(TDL)

# Build myprog
$(MYPROG): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# Build tdl (copy to root)
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
	@echo "Debug build ready: $(MYPROG), root copy: $(TDL)"

.PHONY: all release clean debug

