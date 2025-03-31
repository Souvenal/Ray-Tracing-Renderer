# 定义源文件目录
SOURCE_DIR = source

# 定义编译命令，-j8 表示使用 8 个并行任务进行编译
COMPILE_COMMAND = cd $(SOURCE_DIR) && make -j8 && cd ..

# 定义渲染命令前缀
RENDER_COMMAND_PREFIX = ./render --name

# 定义不同清晰度的渲染参数
RENDER_TEST = $(RENDER_COMMAND_PREFIX) graph_test.ppm -x 800 -y 400 --sample-rate 100 --recursive-depth 10
RENDER_MEDIUM = $(RENDER_COMMAND_PREFIX) graph_medium.ppm -x 1920 -y 1080 --sample-rate 100 --recursive-depth 10
RENDER_2K = $(RENDER_COMMAND_PREFIX) graph_2k.ppm -x 2560 -y 1440 --sample-rate 100 --recursive-depth 10
RENDER_4K = $(RENDER_COMMAND_PREFIX) graph_4k.ppm -x 4096 -y 3112 --sample-rate 100 --recursive-depth 10

# test 清晰度渲染任务
test:
	@$(COMPILE_COMMAND)
	@time $(RENDER_TEST)
	@code graph_test.ppm

# 中等清晰度渲染任务
medium:
	@$(COMPILE_COMMAND)
	@time $(RENDER_MEDIUM)
	@code graph_medium.ppm

# 2K 清晰度渲染任务
2k:
	@$(COMPILE_COMMAND)
	@time $(RENDER_2K)
	@code graph_2k.ppm

# 4K 清晰度渲染任务
4k:
	@$(COMPILE_COMMAND)
	@time $(RENDER_4K)
	@code graph_4k.ppm

# 清理生成的文件
clean:
	@rm -f graph_test.ppm graph_medium.ppm graph_2k.ppm graph_4k.ppm
	@cd $(SOURCE_DIR) && make clean && cd ..