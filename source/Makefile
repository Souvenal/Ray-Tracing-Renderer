# 获取所有的 .cpp 文件
SRC := $(wildcard *.cpp)
# 定义目标文件目录为父级目录下的 obj 目录
OBJDIR := ../obj
# 根据 .cpp 文件生成对应的 .o 文件列表，并将路径设置到父级 obj 目录下
OBJ := $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRC))
# 生成对应的 .d 依赖文件列表
DEPS := $(patsubst %.cpp, $(OBJDIR)/%.d, $(SRC))
# 最终可执行文件的名称
TARGET := render
# 编译器
CXX := g++
# 编译选项
CXXFLAGS := -Wall -g -std=c++20 -O3

# 创建目标文件目录（如果不存在）
$(shell mkdir -p $(OBJDIR))

# 主目标：链接所有目标文件生成可执行文件
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o ../$@ $^

# 编译规则：将 .cpp 文件编译成 .o 文件，并生成 .d 依赖文件
$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# 包含所有生成的依赖文件
-include $(DEPS)

# 清理规则：删除目标文件目录和可执行文件
clean:
	rm -rf $(OBJDIR) $(TARGET)