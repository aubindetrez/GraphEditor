
LDFLAGS = -Lsrc

INCLUDE_DIR = -Isrc

DEFINES = -DDEBUG

MAIN_SOURCES = src/main.cpp

# Optional: -std=c99
CPPFLAGS = $(INCLUDE_DIR) $(DEFINES) -Wunused-result -s -O1 -Wall -Werror -Wpedantic

OBJECTS += main.o

# SFML
SFML_CPPFLAGS += $(shell pkg-config --cflags sfml-all)
SFML_LDFLAGS += $(shell pkg-config --libs sfml-all)

.PHONY: all
all: de sdl

de: $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(TARGET_ARCH) $(SFML_LDFLAGS)

main.o: $(MAIN_SOURCES)
	$(CXX) -c -o $@ $< $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(SFML_CPPFLAGS)

################################################################################
SDL2_LIBS += -lSDL2 -lSDL2_ttf
SDL2_INCL += -I/usr/include/SLD2 -I/usr/include
SDL2_DEF += -D_REENTRANT
################################################################################

sdl: src/main_sdl2.cpp
	$(CXX) -o $@ $< $(SDL2_LIBS) $(SDL2_INCL) $(SDL2_DEF) $(TARGET_ARCH) $(LDFLAGS) $(CFLAGS) $(CPPFLAGS)

.PHONY: clean
clean:
	rm -f de *.o sdl
