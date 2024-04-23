# Compiler flags
CFLAGS := -Wall -Wextra -O3 -std=c17
#-fsanitize=address,undefined
LDFLAGS :=
#-fsanitize=address,undefined

OBJ_DIR := obj
BIN_DIR := bin
ASM_DIR := asm
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
ASMS := $(patsubst %.c,$(ASM_DIR)/%.s,$(SRCS))
HEADERS := $(wildcard *.h)

TARGET := $(BIN_DIR)/main

.PHONY: all $(TARGET) clean run
.PRECIOUS: $(ASMS)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

all: clean $(TARGET)

$(ASM_DIR)/%.s: %.c $(HEADERS)
	$(CC) $(CFLAGS) -S $< -o $@

$(OBJ_DIR)/%.o: $(ASM_DIR)/%.s
	$(AS) $< -o $@

clean:
	$(RM) $(OBJS) $(ASMS) $(TARGET)

run: $(TARGET)
	sudo ./$(TARGET)
