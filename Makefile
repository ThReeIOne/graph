# 编译器和编译选项
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
LDFLAGS = -lm

# 目录定义
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# 目标程序名
TARGET = $(BINDIR)/pathfinding

# 源文件和目标文件
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# 默认目标
all: directories $(TARGET)

# 创建必要的目录
directories:
	@mkdir -p $(OBJDIR) $(BINDIR)

# 链接目标程序
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build complete!"

# 编译源文件
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# 清理编译文件
clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJDIR) $(BINDIR)
	@echo "Clean complete!"

# 运行程序
run: $(TARGET)
	@echo "Running the program..."
	@./$(TARGET)

# 调试模式编译
debug: CFLAGS += -DDEBUG -g
debug: $(TARGET)

# 发布模式编译
release: CFLAGS += -O3 -DNDEBUG
release: clean $(TARGET)

# 安装程序（可选）
install: $(TARGET)
	@echo "Installing to /usr/local/bin..."
	@sudo cp $(TARGET) /usr/local/bin/pathfinding
	@echo "Installation complete!"

# 卸载程序（可选）
uninstall:
	@echo "Removing from /usr/local/bin..."
	@sudo rm -f /usr/local/bin/pathfinding
	@echo "Uninstallation complete!"

# 帮助信息
help:
	@echo "Available targets:"
	@echo "  all      - Build the program (default)"
	@echo "  clean    - Remove build files"
	@echo "  run      - Build and run the program"
	@echo "  debug    - Build with debug information"
	@echo "  release  - Build optimized release version"
	@echo "  install  - Install to system (requires sudo)"
	@echo "  uninstall- Remove from system (requires sudo)"
	@echo "  help     - Show this help message"

# 依赖关系
$(OBJDIR)/graph.o: $(SRCDIR)/graph.c $(INCDIR)/graph.h $(INCDIR)/utils.h
$(OBJDIR)/pathfinding.o: $(SRCDIR)/pathfinding.c $(INCDIR)/pathfinding.h $(INCDIR)/graph.h $(INCDIR)/utils.h
$(OBJDIR)/utils.o: $(SRCDIR)/utils.c $(INCDIR)/utils.h
$(OBJDIR)/main.o: $(SRCDIR)/main.c $(INCDIR)/graph.h $(INCDIR)/pathfinding.h $(INCDIR)/utils.h

.PHONY: all clean run debug release install uninstall help directories 