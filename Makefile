
LDFLAGS = -Lsrc -Ldeps/raylib/src -Ldeps/raylib/src -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -latomic

INCLUDE_DIR = -Isrc -Ideps/raylib/src -Ideps/raylib/src/external -I/usr/local/include

DEFINES = -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP

MAIN_SOURCES = src/main.cpp

# Optional: -std=c99
CPPFLAGS = $(INCLUDE_DIR) $(DEFINES) -Wno-missing-braces -Wunused-result -s -O1 -Wall
CFLAGS += -Wall -Werror -Wpedantic

OBJECTS += main.o

ifdef XLIB_SUPPORT
CPPFLAGS += -DXLIB_SUPPORT
# Linker arguments already used by raylib
OBJECTS += xlib_deps.o
endif

ifdef FONTCONFIG_SUPPORT
CPPFLAGS += -DFONTCONFIG_SUPPORT
CPPFLAGS += $(shell pkg-config --cflags fontconfig)
LDFLAGS += $(shell pkg-config --libs fontconfig)
OBJECTS += fontconfig_deps.o
endif

.PHONY: all
all: de

.PHONY: raylib
raylib:
	$(MAKE) -C deps/raylib/src PLATFORM=PLATFORM_DESKTOP

de: $(OBJECTS) | raylib
	$(CC) -o $@ $^ $(LDFLAGS) $(TARGET_ARCH)

main.o: $(MAIN_SOURCES)
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH)

xlib_deps.o: src/xlib_deps.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH)

fontconfig_deps.o: src/fontconfig_deps.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH)

.PHONY: clean
clean:
	rm -f de *.o
	$(MAKE) -C deps/raylib/src clean
