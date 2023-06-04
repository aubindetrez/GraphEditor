
LDFLAGS = -L. -Ldeps/raylib/src -Ldeps/raylib/src -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -latomic

INCLUDE_DIR = -I. -Ideps/raylib/src -Ideps/raylib/src/external -I/usr/local/include

DEFINES = -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP

# Optional: -std=c99
CPPFLAGS = $(INCLUDE_DIR) $(DEFINES) -Wno-missing-braces -Wunused-result -s -O1 -Wall
CFLAGS += -Wall -Werror -Wpedantic

.PHONY: all
all: de

.PHONY: deps
deps:
	$(MAKE) -C deps/raylib/src PLATFORM=PLATFORM_DESKTOP

de: src/main.cpp deps
	$(CC) -o $@ $< $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)

.PHONY: clean
clean:
	rm -f de
	$(MAKE) -C deps/raylib/src clean
