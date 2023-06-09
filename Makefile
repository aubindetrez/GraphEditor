LDFLAGS = -Lsrc
INCLUDE_DIR = -Isrc

DEFINES = -DDEBUG
# Enables you to inspect the assembly
# with: objdump -d -M intel -S <BINARY> | less
DEBUGFLAG = -g -gdwarf-2
OPTIMIZATION = -Ofast

CPPFLAGS = $(INCLUDE_DIR) $(DEFINES) -Wunused-result $(OPTIMIZATION) -Wall -Werror -Wpedantic $(DEBUGFLAG)

PRODUCTS = # Products to clean

.PHONY: all
all: de.bin sdl.bin sdl_static.bin lua.bin fops.bin

################################################################################
#							  SFML System Install                              #
################################################################################
MAIN_SOURCES = src/main.cpp
OBJECTS += main.o
SFML_CPPFLAGS += $(shell pkg-config --cflags sfml-all)
SFML_LDFLAGS += $(shell pkg-config --libs sfml-all)

de.bin: $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(TARGET_ARCH) $(SFML_LDFLAGS)
PRODUCTS += de.bin

main.o: $(MAIN_SOURCES)
	$(CXX) -c -o $@ $< $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(SFML_CPPFLAGS)
PRODUCTS += main.o

################################################################################
#							  SDL2 System Install                              #
################################################################################
SDL2_LIBS += -lSDL2 -lSDL2_ttf
SDL2_INCL += -I/usr/include/SLD2 -I/usr/include
SDL2_DEF += -D_REENTRANT

sdl.bin: src/main_sdl2.cpp
	$(CXX) -o $@ $< $(SDL2_LIBS) $(SDL2_INCL) $(SDL2_DEF) $(TARGET_ARCH) $(LDFLAGS) $(CFLAGS) $(CPPFLAGS)
PRODUCTS += sdl.bin

################################################################################
#								   SDL2 Local                                  #
################################################################################
LOCAL_SDL2_ROOT=deps/SDL/
# Compile local SDL
$(LOCAL_SDL2_ROOT)/Makefile: $(LOCAL_SDL2_ROOT)/CMakeLists.txt .gitmodules
	cd $(LOCAL_SDL2_ROOT) && mkdir -p install && ./configure --prefix=`realpath install/`
$(LOCAL_SDL2_ROOT)/build/SDL.o: $(LOCAL_SDL2_ROOT)/Makefile
		$(MAKE) -C $(LOCAL_SDL2_ROOT)
$(LOCAL_SDL2_ROOT)/install/lib/libSDL2.a: $(LOCAL_SDL2_ROOT)/build/SDL.o
		$(MAKE) -C $(LOCAL_SDL2_ROOT) install
		touch $@ # Making sure to update the date
.PHONY: local_sdl
local_sdl: $(LOCAL_SDL2_ROOT)/install/lib/libSDL2.a
LOCAL_SDL2_LIBS += $(shell $(LOCAL_SDL2_ROOT)/install/bin/sdl2-config --static-libs)
LOCAL_SDL2_INCL += $(shell $(LOCAL_SDL2_ROOT)/install/bin/sdl2-config --cflags)

# Compile local SDL_ttf
LOCAL_SDL2_TTF_ROOT=deps/SDL_ttf/
$(LOCAL_SDL2_TTF_ROOT)/Makefile: $(LOCAL_SDL2_TTF_ROOT)/CMakeLists.txt $(LOCAL_SDL2_ROOT)/install/lib/libSDL2.a .gitmodules
	mkdir -p $(LOCAL_SDL2_TTF_ROOT)/build/ $(LOCAL_SDL2_TTF_ROOT)/install/ && \
	cmake -S $(LOCAL_SDL2_TTF_ROOT) -B $(LOCAL_SDL2_TTF_ROOT)/build/ \
	-DCMAKE_INSTALL_PREFIX=$(LOCAL_SDL2_TTF_ROOT)/install/ \
	-DCMAKE_PREFIX_PATH=$(LOCAL_SDL2_TTF_ROOT)/../SDL/install \
	-DCMAKE_LIBRARY_PATH=$(LOCAL_SDL2_TTF_ROOT)/../SDL/install \
	-DBUILD_SHARED_LIBS=OFF \
	-DSDL2TTF_BUILD_SHARED_LIBS=OFF
$(LOCAL_SDL2_TTF_ROOT)/build/libSDL2_ttf.a: $(LOCAL_SDL2_TTF_ROOT)/Makefile
	$(MAKE) -C $(LOCAL_SDL2_TTF_ROOT)/build/
$(LOCAL_SDL2_TTF_ROOT)/install/lib64/libSDL2_ttf.a: $(LOCAL_SDL2_TTF_ROOT)/build/libSDL2_ttf.a
	$(MAKE) -C $(LOCAL_SDL2_TTF_ROOT)/build/ install
	touch $@ # Making sure to update the date
.PHONY: local_sdl_ttf
local_sdl_ttf: $(LOCAL_SDL2_TTF_ROOT)/install/lib64/libSDL2_ttf.a
LOCAL_SDL2_TTF_INCL += -I$(LOCAL_SDL2_TTF_ROOT)/install/include/SDL2 -D_REENTRANT
LOCAL_SDL2_TTF_LIBS += $(LOCAL_SDL2_TTF_ROOT)/install/lib64/libSDL2_ttf.a

sdl_static.bin: src/main_sdl2.cpp local_sdl local_sdl_ttf
	$(CXX) -o $@ $< $(LOCAL_SDL2_LIBS) $(LOCAL_SDL2_INCL) $(LOCAL_SDL2_TTF_LIBS) \
		$(LOCAL_SDL2_TTF_INCL) $(TARGET_ARCH) $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) \
		$(shell pkg-config --cflags freetype2) \
		$(shell pkg-config --libs freetype2)
PRODUCTS += sdl_static.bin

################################################################################
#								      LUA                                      #
################################################################################
lua.bin: src/main_lua.cpp
	$(CXX) $^ $(shell pkg-config --libs lua) $(shell pkg-config --cflags lua) -o $@
PRODUCTS += lua.bin

################################################################################
#						      File operations                                  #
################################################################################
fops.bin: src/main_fops.cpp
	$(CXX) $^ -o $@
PRODUCTS += fops.bin

################################################################################
#								    Cleanup                                    #
################################################################################

.PHONY: clean_all
clean_all: clean clean_sld2 clean_sld2_ttf

.PHONY: clean
clean:
	rm -f $(PRODUCTS)

.PHONY: clean_sld2
clean_sld2:
	cd $(LOCAL_SDL2_ROOT) && git clean -ffdx

.PHONY: clean_sld2_ttf
clean_sld2_ttf:
	cd $(LOCAL_SDL2_TTF_ROOT) && git clean -ffdx
