# DEPENDENCIES
# sdl sdl_image  sdl_mixer sdl_ttf

SRC_ALL = \
    $(wildcard src/*.cpp)\
    $(wildcard slib/src/*.cpp)\
    $(wildcard src/weapons/*.cpp)\
    $(wildcard src/ogol/*.cpp)\
    $(wildcard src/tiled/*.cpp)\
    $(wildcard src/xml/*.cpp)\
    $(wildcard src/gui/*.cpp)\
	 $(wildcard src/ogol_editor/*.cpp)

SDL = SDL_mixer SDL_image SDL_ttf SDL_gfx
OBJS = $(SRC_ALL:.cpp=.o)

SDL_OPTS=`pkg-config --cflags $(SDL)`

OPTS=-pedantic $(DEBUG_OPTS) -Wall -std=c++17 -g  $(SDL_OPTS) -Islib/include -Isrc/ogol -Isrc/weapons -Isrc/xml -Isrc/ogol_editor -Isrc -Isrc/gui

PATHS=$(sort $(dir $(SRC_ALL)))
BUILD_DIR=build
NOTDIR_FILES=$(notdir $(SRC_ALL) )
OBJS_SERVER=$(addprefix $(BUILD_DIR)/,$(NOTDIR_FILES:.cpp=.o))
DEPENDS_ALL=$(OBJS_SERVER:.o=.d)
LINKER=g++ 
CXX=g++ $(OPTS) -I./include $(INCLUDE)
LIBS=-lsdl
DEBUG_OPTS=
STATIC_BIN:=satower-static-$(shell uname -m)
BIN_TARGETS=satower $(STATIC_BIN)

.PHONY: all
all: $(BUILD_DIR) $(BIN_TARGETS)

vars:
	@echo "LINKER      = $(LINKER)"
	@echo "OBKS_SERVER = $(OBJS_SERVER)"
	@echo "STATIC-BIN  = $(STATIC_BIN)"

satower: build/main.o $(OBJS_SERVER)
	@echo "Linking binary : $@"
	$(LINKER) $(OBJS_SERVER) `pkg-config --libs-only-l $(SDL)` -o $@
	
$(STATIC_BIN): satower
	@echo "Linking static binary : $@"
	# $(LINKER)  -static-libgcc -static $(OBJS_SERVER) -lSDL -lpthread -lSDL_image -lSDL_gfx  -lSDL_ttf  -lfreetype -lbz2 -lz  -lSDL_mixer -ldl -lSDL -o $@

.PHONY: phantom.conf
phantom.conf: $(PHANTOM_CONF)

$(PHANTOM_CONF):
	@if [ -f $(PHANTOM_CONF) ]; then \
		echo "  Fichier de configuration déja installé"; \
	else \
		echo "  Création fichier de configuration"; \
		echo "  [plugins]" > $(PHANTOM_CONF); \
	fi

.PHONY: depends
depends:
	makedepend -Y -p$(BUILD_DIR)/ -- $(INCLUDE) -- $(SRC_ALL) 2>/dev/null 
	@sed -i "s'$(BUILD_DIR)/src'$(BUILD_DIR)'g" makefile

vpath %.cpp $(PATHS)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

%.d: $(BUILD_DIR)
	@touch "$@"

$(BUILD_DIR)/%.o:%.hpp

$(BUILD_DIR)/%.o:%.cpp
	@echo $@: $< > $(patsubst %.o,%.d,$@)
	@echo "    Compiling $<" 
	
	@$(CXX) -c $< -o $@

.PHONY: clean
clean:
	@-rm -f $(BUILD_DIR)/*.o
	@-rm -f $(BUILD_DIR)/*.d
	@-rmdir $(BUILD_DIR) >/dev/null 2>&1
	@-rm -f $(BIN_TARGETS)

-include $(DEPENDS_ALL)



