LDFLAGS = -Lsrc
INCLUDE_DIR = -Isrc

DEFINES = -DDEBUG
# Enables you to inspect the assembly
# with: objdump -d -M intel -S <BINARY> | less
DEBUGFLAG = -g -gdwarf-2
OPTIMIZATION = -Ofast

CPPFLAGS = $(INCLUDE_DIR) $(DEFINES) -Wunused-result $(OPTIMIZATION) -Wall -Werror -Wpedantic $(DEBUGFLAG)

.PHONY: all
all: de sdl sdl_static

################################################################################
#							  SFML System Install                              #
################################################################################
MAIN_SOURCES = src/main.cpp
OBJECTS += main.o
SFML_CPPFLAGS += $(shell pkg-config --cflags sfml-all)
SFML_LDFLAGS += $(shell pkg-config --libs sfml-all)

de: $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(TARGET_ARCH) $(SFML_LDFLAGS)

main.o: $(MAIN_SOURCES)
	$(CXX) -c -o $@ $< $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(SFML_CPPFLAGS)

################################################################################
#							  SDL2 System Install                              #
################################################################################
SDL2_LIBS += -lSDL2 -lSDL2_ttf
SDL2_INCL += -I/usr/include/SLD2 -I/usr/include
SDL2_DEF += -D_REENTRANT

sdl: src/main_sdl2.cpp
	$(CXX) -o $@ $< $(SDL2_LIBS) $(SDL2_INCL) $(SDL2_DEF) $(TARGET_ARCH) $(LDFLAGS) $(CFLAGS) $(CPPFLAGS)

################################################################################
#								   SDL2 Local                                  #
################################################################################

# Compile local SDL
LOCAL_SDL2_ROOT=deps/SDL/
.PHONY: local_sdl
local_sdl:
	cd $(LOCAL_SDL2_ROOT) && mkdir -p install && ./configure --prefix=`realpath install/` && \
		$(MAKE) && \
		$(MAKE) install
LOCAL_SDL2_LIBS += $(shell $(LOCAL_SDL2_ROOT)/install/bin/sdl2-config --static-libs)
LOCAL_SDL2_INCL += $(shell $(LOCAL_SDL2_ROOT)/install/bin/sdl2-config --cflags)

# Compile local SDL_ttf
LOCAL_SDL2_TTF_ROOT=deps/SDL_ttf/
.PHONY: local_sdl_ttf
local_sdl_ttf: local_sdl
	mkdir -p $(LOCAL_SDL2_TTF_ROOT)/build/ $(LOCAL_SDL2_TTF_ROOT)/install/ && \
	cmake -S $(LOCAL_SDL2_TTF_ROOT) -B $(LOCAL_SDL2_TTF_ROOT)/build/ \
	-DCMAKE_INSTALL_PREFIX=$(LOCAL_SDL2_TTF_ROOT)/install/ \
	-DCMAKE_PREFIX_PATH=$(LOCAL_SDL2_TTF_ROOT)/../SDL/install \
	-DCMAKE_LIBRARY_PATH=$(LOCAL_SDL2_TTF_ROOT)/../SDL/install \
	-DBUILD_SHARED_LIBS=OFF \
	-DSDL2TTF_BUILD_SHARED_LIBS=OFF \
	&& \
	$(MAKE) -C $(LOCAL_SDL2_TTF_ROOT)/build/ && \
	$(MAKE) -C $(LOCAL_SDL2_TTF_ROOT)/build/ install
LOCAL_SDL2_TTF_INCL += -I$(LOCAL_SDL2_TTF_ROOT)/install/include/SDL2 -D_REENTRANT
LOCAL_SDL2_TTF_LIBS += $(LOCAL_SDL2_TTF_ROOT)/install/lib64/libSDL2_ttf.a
# -DCMAKE_FIND_LIBRARY_SUFFIXES=".a" \
# -DCMAKE_EXE_LINKER_FLAGS="-static" \


sdl_static: src/main_sdl2.cpp local_sdl local_sdl_ttf
	$(CXX) -o $@ $< $(LOCAL_SDL2_LIBS) $(LOCAL_SDL2_INCL) $(LOCAL_SDL2_TTF_LIBS) \
		$(LOCAL_SDL2_TTF_INCL) $(TARGET_ARCH) $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) \
		$(shell pkg-config --cflags freetype2) \
		$(shell pkg-config --libs freetype2)

################################################################################
#								    Cleanup                                    #
################################################################################

.PHONY: clean_all
clean_all: clean clean_sld2 clean_sld2_ttf

.PHONY: clean
clean:
	rm -f de *.o sdl sdl_static

.PHONY: clean_sld2
clean_sld2:
	cd $(LOCAL_SDL2_ROOT) && git clean -ffdx

.PHONY: clean_sld2_ttf
clean_sld2_ttf:
	cd $(LOCAL_SDL2_TTF_ROOT) && git clean -ffdx
