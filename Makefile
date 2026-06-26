CC := clang
CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O0 -g -MMD -MP -D_CRT_SECURE_NO_WARNINGS --target=x86_64-pc-windows-msvc -Wno-language-extension-token -Wno-gnu-zero-variadic-macro-arguments
INCLUDES := -Iengine2d -Ivendor/glfw/include -Ivendor/glad/include -Ivendor/imgui -Ivendor/imgui/backends -Ivendor/stb
LDFLAGS := -Lvendor/glfw/lib -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -fuse-ld=lld-link -Xlinker /NODEFAULTLIB:libcmt

# Автоматический поиск всех .cpp в engine2d и app2d
SRCS := $(wildcard engine2d/*.cpp engine2d/*/*.cpp engine2d/*/*/*.cpp) \
        $(wildcard app2d/*.cpp) \
        vendor/imgui/imgui.cpp vendor/imgui/imgui_draw.cpp vendor/imgui/imgui_widgets.cpp \
        vendor/imgui/imgui_tables.cpp vendor/imgui/backends/imgui_impl_glfw.cpp vendor/imgui/backends/imgui_impl_opengl3.cpp

OBJS := $(patsubst %.cpp, build/obj/%.o, $(SRCS))

GLAD_SRC := vendor/glad/src/glad.c
GLAD_OBJ := build/obj/vendor/glad/src/glad.o

OBJS += $(GLAD_OBJ)

DEPS := $(OBJS:.o=.d)
TARGET := build\2DGame.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@if not exist build md build
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

build/obj/%.o: %.cpp
	@if not exist $(subst /,\,$(dir $@)) md $(subst /,\,$(dir $@))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(GLAD_OBJ): $(GLAD_SRC)
	@if not exist $(subst /,\,$(dir $@)) md $(subst /,\,$(dir $@))
	$(CC) --target=x86_64-pc-windows-msvc -Ivendor/glad/include -c $< -o $@

-include $(DEPS)

clean:
	@if exist build rmdir /s /q build