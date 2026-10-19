# --- Configuration ---
SRC = ./t_vis_color.c
ROOT_DIR = ./../../../
MXE_PATH = /home/tilo/res/mxe
SUFFIX = _x86_64
# --- Default Target Logic ---
# Usage:
#   make              (Builds Linux)
#   make TARGET=WIN64 (Builds Windows 64-bit via MXE)
#   make TARGET=XP    (Builds Windows 32-bit via MXE)
# Requires MXE to be installed and properly setup locally!!!

ifeq ($(TARGET), WIN64)
    ARCH = x86_64-w64-mingw32.static
    CC = $(MXE_PATH)/usr/bin/$(ARCH)-gcc
    PKG_CONFIG = $(MXE_PATH)/usr/bin/$(ARCH)-pkg-config
    OUT = $(ROOT_DIR)3d_track_visualizer$(SUFFIX).exe
    # Windows-specific flags
    CFLAGS = `$(PKG_CONFIG) --cflags glut glew`
    LIBS = `$(PKG_CONFIG) --libs --static glut glew` -lopengl32 -lglu32 -lm
    LDFLAGS = -static -Wl,--subsystem,windows

else ifeq ($(TARGET), XP)
    SUFFIX = _x86_32
    # Note: Ensure you have the i686 target installed in MXE
    ARCH = i686-w64-mingw32.static
    CC = $(MXE_PATH)/usr/bin/$(ARCH)-gcc
    PKG_CONFIG = $(MXE_PATH)/usr/bin/$(ARCH)-pkg-config
    OUT = $(ROOT_DIR)3d_track_visualizer$(SUFFIX).exe
    CFLAGS = `$(PKG_CONFIG) --cflags glut glew`
    LIBS = `$(PKG_CONFIG) --libs --static glut glew` -lopengl32 -lglu32 -lm
    LDFLAGS = -static -Wl,--subsystem,windows

else
    # Default: Linux (Mint)
    CC = g++
    OUT = $(ROOT_DIR)3d_track_visualizer$(SUFFIX).bin
    CFLAGS = -I/home/tilo/res/freeglut/include -I/home/tilo/res/glew/include
    LIBS = -L/home/tilo/res/freeglut/build/lib -L/home/tilo/res/glew/lib \
           -lglut -lGLEW -lGL -lGLU -lm
    LDFLAGS =
endif

# --- Rules ---

all: $(OUT)


cm/color_mapper.tab.c: cm/color_mapper.y
	cd cm && bison -d color_mapper.y

cm/lex.yy.c: cm/cm_lex.l cm/color_mapper.tab.c
	cd cm && flex cm_lex.l

shaders_embedded.h: vertex.glsl fragment.glsl embed_shaders.py
	python3 embed_shaders.py

$(OUT): $(SRC) cm/color_mapper.tab.c cm/cm.h cm/lex.yy.c shaders_embedded.h
	@echo "Building for Target: $(TARGET) [$(CC)]"
	$(CC) $(SRC) cm/lex.yy.c shaders_embedded.h -o $(OUT) $(CFLAGS) $(LDFLAGS) $(LIBS)
	@echo "Compilation successful: $(OUT)"

clean:
	rm -f *.o *.output *.tab.* *.yy.* cm/*.tab.* cm/*.yy.* cm/*.output
	@echo "Cleanup complete (Executable preserved)."

.PHONY: all clean
