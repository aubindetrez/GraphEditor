
LDFLAGS = -Lsrc

INCLUDE_DIR = -Isrc

DEFINES = -DDEBUG

MAIN_SOURCES = src/main.cpp

# Optional: -std=c99
CPPFLAGS = $(INCLUDE_DIR) $(DEFINES) -Wunused-result -s -O1 -Wall -Werror -Wpedantic

OBJECTS += main.o

# SFML
CPPFLAGS += $(shell pkg-config --cflags sfml-all)
LDFLAGS += $(shell pkg-config --libs sfml-all)

.PHONY: all
all: de

de: $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(TARGET_ARCH)

main.o: $(MAIN_SOURCES)
	$(CXX) -c -o $@ $< $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH)

.PHONY: clean
clean:
	rm -f de *.o
