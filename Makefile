DIR_BUILD = build
DIR_ROOT = $(shell pwd)
DIR_RESOURCES = $(DIR_ROOT)/resources
DIR_APP = rogue-craft

DIR_INSTALLED_RESOURCES=/usr/local/share/$(DIR_APP)
DIR_INSTALLED_ENV=/etc/$(DIR_APP)
DIR_INSTALLED_CACHE=/var/cache/$(DIR_APP)
DIR_INSTALLED_BIN=/usr/local/games
INSTALLED_LOG_FILE=/var/log/rogue-craft.log

CC = gcc
#Debian/Ubuntu: Ncurses: To compile: ncursesw-dev, ncursesw5-dev, Run: ncursesw
#VLC: Compile: libvlccore-dev libvlc-dev Run: vlc
LIBS = -l ncursesw -l panelw -l menuw -l m -lvlc -ldl -l:libcursed_tools.so.0.0
DEFINITIONS = -DDIR_APP_RELATIVE=\"$(DIR_APP)\" $(VERSION_DEFINITIONS) -D NCURSES_WIDECHAR=1 -D_GNU_SOURCE

#This way we can avoid nasty includes like #include "../../../config/config.h"
NAMESPACES = -I config -I src
INCLUDES = $(NAMESPACES) -I lib/mem-pool/src -I lib/collection/src -I lib/tinydir -I lib/parson -I lib/quadtree/src -I lib/dotenv/src -I lib/rimraf/src -I lib/notifier/include -I lib
CFLAGS = -std=gnu11 -g -Wall -Wextra -ftrapv -Wshadow -Wundef -Wcast-align -Wunreachable-code -Wno-unused-result -fstack-protector
CFLAGS += -O2 -Os

TARGET = rogue-craft
TEST_TARGET = $(TARGET)-test

VERSION_MAJOR=0
VERSION_MINOR=2
VERSION_PATCH=0
VERSION_FULL=$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

VERSION_DEFINITIONS=-DVERSION_MAJOR=$(VERSION_MAJOR) -DVERSION_MINOR=$(VERSION_MINOR) -DVERSION_PATCH=$(VERSION_PATCH) -DVERSION_STRING=\"$(VERSION_FULL)\"


.PHONY: default all clean $(TARGET) $(TEST_TARGET) test


LIB_SOURCES = $(shell find lib -name "*.c" | grep -E -v "test|samples|dev|benchmark|examples|notifier|cursed_tools")
COMMON_SOURCES = $(LIB_SOURCES) $(shell find src config -name "*.c")
SOURCES = $(COMMON_SOURCES) main.c
TEST_SOURCES = $(COMMON_SOURCES) $(shell find test -name "*.c")

OBJECTS = $(patsubst %.c, $(DIR_BUILD)/%.o, $(SOURCES))
TEST_OBJECTS = $(patsubst %.c, $(DIR_BUILD)/%.o, $(TEST_SOURCES))


default: $(TARGET)


NOTIFIER_ROOT = ./lib/notifier

include ./lib/dev/build/build.mk
include ./lib/notifier/Makefile


-include $(shell find $(DIR_BUILD) -name "*.d")


C_GREEN = \033[0;32m
C_RESET = \033[0m

# Progress display
# Source: https://stackoverflow.com/questions/451413/make-makefile-progress-indication
ifndef PROGRESS
T := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory \
	-nrRf $(firstword $(MAKEFILE_LIST)) \
	PROGRESS="COUNTTHIS" | grep -c "COUNTTHIS")

N := x
C = $(words $N)$(eval N := x $N)
PROGRESS = echo -ne "\r[$(C_GREEN)`expr $C '*' 100 / $T`%$(C_RESET)]"
endif

$(DIR_BUILD)/%.o: %.c
	@mkdir -p $(shell dirname $@)
	@$(CC) -MMD $(CFLAGS) $(INCLUDES) $(DEFINITIONS) -c $< -o $@
	@$(PROGRESS) "CC $@"


.PRECIOUS: $(TARGET) $(OBJECTS)


install-dependencies:
	cd ./lib/cursed_tools && make install

$(TARGET): $(OBJECTS) install-dependencies
	@$(CC) $(OBJECTS) $(CFLAGS) $(LIBS) -o $@
	@$(PROGRESS) "Linking $@"

$(TEST_TARGET): $(TEST_OBJECTS) install-dependencies
	@$(CC) $(TEST_OBJECTS) $(CFLAGS) $(LIBS) -o $@
	$(PROGRESS) "Linking $a"

prepare-test:
	rm -f test/fixture/cache/*.cache
	rm -rf test/fixture/saved_games/*

run-test: prepare-test $(TEST_TARGET)
	./$(TEST_TARGET) --env=test

run-test-valgrind: prepare-test $(TEST_TARGET)
	valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./$(TEST_TARGET) --env=test

run-debug: $(TARGET)
	./$(TARGET) --env=dev

install-environments:
	mkdir -p $(DIR_INSTALLED_ENV)
	cp ./config/environments/.env.* $(DIR_INSTALLED_ENV)
	chmod -R 775 $(DIR_INSTALLED_ENV)


install: install-environments
	# previous wrong location should be removed
	rm -f /usr/bin/$(TARGET)
	mkdir -p $(DIR_INSTALLED_CACHE)
	chmod -R 777 $(DIR_INSTALLED_CACHE)
	mkdir -p $(DIR_INSTALLED_RESOURCES)
	cp -r $(DIR_RESOURCES)/* $(DIR_INSTALLED_RESOURCES)
	chmod 775 -R $(DIR_INSTALLED_RESOURCES)
	cp $(TARGET) $(DIR_INSTALLED_BIN)
	touch $(INSTALLED_LOG_FILE)
	chmod 777 $(INSTALLED_LOG_FILE)
	rm -rf $(DIR_INSTALLED_CACHE)/*.cache

uninstall: uninstall-notifier
	rm -rf $(DIR_INSTALLED_CACHE)
	rm -rf $(DIR_INSTALLED_RESOURCES)
	rm -rf $(DIR_INSTALLED_ENV)
	rm -f $(DIR_INSTALLED_BIN)/$(TARGET)

clean-cache:
	rm -rf ./cache/*.cache

clean:
	rm -rf $(DIR_ROOT)/$(DIR_BUILD)/*
	make clean-cache

