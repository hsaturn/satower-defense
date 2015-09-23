SRC_ALL = main.cpp $(wildcard src/*.cpp)

SDL_OPTS=`pkg-config --cflags sdl`

OPTS=$(DEBUG_OPTS) -Wall -std=c++0x -O3 $(SDL_OPTS)

PATHS=$(sort $(dir $(SRC_ALL)))
BUILD_DIR=build
NOTDIR_FILES=$(notdir $(SRC_LIB) )
OBJS_SERVER=$(addprefix $(BUILD_DIR)/,$(NOTDIR_FILES:.cpp=.o))
DEPENDS_ALL=$(OBJS_SERVER:.o=.d)
LINKER=g++
CXX=g++ $(OPTS) -I./include $(INCLUDE)
LIBS=-lsdl
DEBUG_OPTS=
BIN_TARGETS=main


.PHONY: all
all: $(BUILD_DIR) $(BIN_TARGETS)
	echo "SRC_ALL=$(SRC_ALL)"

.PHONY: $(BIN_TARGETS)
$(BIN_TARGETS):
	@echo "  Building Tool : $@"
	@$(MAKE) -s $(BUILD_DIR)/$@.o
	@$(CXX) $(BUILD_DIR)/$@.o -lphantom -o $@


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
	@echo "OPTS=$(OPTS)"
	@echo "    Compiling $<" 
	
	@$(CXX) -c $< -o $@

.PHONY: clean
clean:
	@-rm -f $(BUILD_DIR)/*.o
	@-rm -f $(BUILD_DIR)/*.d
	@-rmdir $(BUILD_DIR) >/dev/null 2>&1
	@-rm -f $(BIN_TARGETS)

-include $(DEPENDS_ALL)



