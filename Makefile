# ==============================================================================
# CONFIGURATION
# ==============================================================================

# Compiler and Flags
CC       := gcc
CFLAGS   := -Wall -Wextra -Werror -std=c11 -Iinclude
LDFLAGS  := 
LDLIBS   := 

# Directory Structure
SRC_DIR  := src
OBJ_DIR  := obj
BIN_DIR  := bin
INC_DIR  := include

# Target Executable Name
TARGET   := $(BIN_DIR)/tamagochi

# Find all C source files in SRC_DIR (supports subdirectories)
SRCS     := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)
# Map source files to object files in OBJ_DIR
OBJS     := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
# Map object files to dependency files
DEPS     := $(OBJS:.o=.d)

# ==============================================================================
# TARGETS & RULES
# ==============================================================================

.PHONY: all clean fclean re directories

# Default target
all: directories $(TARGET)

# Run the app entrypoint
run: $(TARGET)
	./$(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	@echo "Linking executable: $@"
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

# Compile source files into object files
# -MMD -MP automatically creates .d files for header dependencies
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling: $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Create necessary directories
directories:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR) $(INC_DIR)

# Include automatically generated dependency files (if they exist)
-include $(DEPS)

# Clean object files and dependency files
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(OBJ_DIR)

# Clean build artifacts and binary
fclean: clean
	@echo "Removing binaries..."
	@rm -rf $(BIN_DIR)

# Rebuild everything
re: fclean all
