UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
  NCPU := $(shell grep -c '^processor' /proc/cpuinfo)
else ifeq ($(UNAME_S),Darwin)
  NCPU := $(shell sysctl -n hw.ncpu)
else
  $(error unsupported OS: "$(UNAME_S)")
endif

SUPERBUILD_DIR ?= .superbuild/
BUILD_DIR ?= .build/
# this has to be ext/ because it's hardcoded in the cmake files
EXT_DIR := ext/

CMAKE ?= cmake

.PHONY: build-ext
build-ext:
	mkdir -p $(SUPERBUILD_DIR) \
	  && cd $(SUPERBUILD_DIR) \
	  && $(CMAKE) -DBUILD_EXT=ON .. \
	  && make -j$(NCPU) \
	  && cd ..

CMAKE_BUILD_TYPE ?= RELEASE
.PHONY: cmake-gen
cmake-gen:
	$(CMAKE) -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) -H. -B$(BUILD_DIR) .

.PHONY: build
build:
	$(CMAKE) --build $(BUILD_DIR) --parallel $(NCPU)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: deepclean
deepclean:
	rm -rf $(BUILD_DIR) $(SUPERBUILD_DIR) $(EXT_DIR)

.PHONY: clang-format
clang-format:
	find . -type d \( -path ./.build -o -path ./.superbuild -o -path ./ext \) -prune -false -o \
	  -type f \( -iname "*.hh" -o -iname "*.h" -o -iname "*.hpp" -o -iname "*.cc" -o -iname "*.c" -o -iname "*.cpp" \) \
	  -exec clang-format -style=file -i {} \;
