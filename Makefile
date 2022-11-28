
LDFLAGS = -L. -L/data/projects/DiagramEditor/raylib/src -L/data/projects/DiagramEditor/raylib/src -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -latomic

INCLUDE_DIR = -I. -I/data/projects/DiagramEditor/raylib/src -I/data/projects/DiagramEditor/raylib/src/external -I/usr/local/include

DEFINES = -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP

# Optional: -std=c99
CPPFLAGS = $(INCLUDE_DIR) $(DEFINES) -Wno-missing-braces -Wunused-result -s -O1 -Wall

.PHONY: all
all: de

de: main.cpp
	$(CC) -o $@ $^ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)

.PHONY: clean
clean:
	rm -f de
