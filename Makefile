# Compiler flags
CXXFLAGS := -Wall -Wextra -O3 -std=c++20

CXXFLAGS_DBG := -Wall -Wextra -Og -ggdb -std=c++20 -fsanitize=address,undefined

LDFLAGS :=
LDFLAGS_DBG := -fsanitize=address,undefined

OBJ_DIR := obj
BIN_DIR := bin
ASM_DIR := asm
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))
ASMS := $(patsubst %.cpp,$(ASM_DIR)/%.s,$(SRCS))
OBJS_DBG := $(patsubst %.cpp,$(OBJ_DIR)/%.dbg.o,$(SRCS))
ASMS_DBG := $(patsubst %.cpp,$(ASM_DIR)/%.dbg.s,$(SRCS))
HEADERS := $(wildcard *.h)

TARGET := $(BIN_DIR)/main
TARGET_DBG := $(BIN_DIR)/main.dbg

.PHONY: all $(TARGET) $(TARGET_DBG) clean run
.PRECIOUS: $(ASMS)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(TARGET_DBG): $(OBJS_DBG)
	$(CXX) $(LDFLAGS_DBG) $^ -o $@

all: $(TARGET) $(TARGET_DBG)

$(ASM_DIR)/%.s: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -S $< -o $@

$(ASM_DIR)/%.dbg.s: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS_DBG) -S $< -o $@

$(OBJ_DIR)/%.o: $(ASM_DIR)/%.s
	$(AS) $< -o $@

$(OBJ_DIR)/%.dbg.o: $(ASM_DIR)/%.dbg.s
	$(AS) $< -o $@

clean:
	$(RM) $(OBJS) $(OBJS_DBG) $(TARGET) $(TARGET_DBG)

run: $(TARGET)
	sudo ./$(TARGET)
