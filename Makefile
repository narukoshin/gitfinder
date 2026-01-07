# Makefile for narukoshin/gitfinder (Ninja generator + clang-format support)
# Usage examples:
#   make                # configure + build (Release, uses Ninja)
#   make BUILD_TYPE=Debug
#   make TOOLCHAIN=toolchains/your_toolchain.cmake
#   make EXTRA_CMAKE_ARGS='-DFOO=ON'
#   make run
#   make format         # format source with clang-format
#   make check-format   # fail if formatting is required (useful in CI)
#   make clean
#
# Variables:
#   CMAKE               - cmake executable (default: cmake)
#   CMAKE_BUILD_DIR     - build directory (default: build)
#   BUILD_TYPE          - Release/Debug (default: Release)
#   TOOLCHAIN           - optional CMake toolchain file path
#   EXTRA_CMAKE_ARGS    - any extra args forwarded to cmake configure
#   CMAKE_GENERATOR     - default: Ninja (override to use another generator)
#   NPROC               - parallel jobs for the native build tool
#   TARGET              - produced executable name (default: main)
#   PREFIX              - install prefix (default: /usr/local)

CMAKE ?= cmake
CMAKE_BUILD_DIR ?= build
BUILD_TYPE ?= Release
TOOLCHAIN ?=
EXTRA_CMAKE_ARGS ?=
CMAKE_GENERATOR ?= Ninja
TARGET ?= main
NPROC ?= $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)
PREFIX ?= /usr/local

CMAKE_ARGS := -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
ifeq ($(strip $(TOOLCHAIN)),)
else
	CMAKE_ARGS += -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN)
endif
CMAKE_ARGS += $(EXTRA_CMAKE_ARGS)

# Source files to format/check (adjust path patterns if needed)
FORMAT_FILES := $(shell find src -type f \( -name '*.cpp' -o -name '*.cc' -o -name '*.c' -o -name '*.hpp' -o -name '*.h' \))

.PHONY: all configure build run install clean distclean reconfigure format check-format help

all: build

help:
	@printf "\nMakefile targets:\n"
	@printf "  make                Configure and build (default BUILD_TYPE=Release)\n"
	@printf "  make BUILD_TYPE=Debug\n"
	@printf "  make TOOLCHAIN=toolchains/your_toolchain.cmake\n"
	@printf "  make EXTRA_CMAKE_ARGS='-DFOO=ON'\n"
	@printf "  make run\n"
	@printf "  make install PREFIX=/some/path\n"
	@printf "  make format         # run clang-format -i on sources\n"
	@printf "  make check-format   # exit non-zero if files would change\n"
	@printf "  make clean\n"
	@printf "  make distclean\n\n"

configure:
	@echo "Configuring (generator: $(CMAKE_GENERATOR), build dir: $(CMAKE_BUILD_DIR), type: $(BUILD_TYPE))"
	@mkdir -p $(CMAKE_BUILD_DIR)
	@cd $(CMAKE_BUILD_DIR) && \
		if [ -n "$(CMAKE_GENERATOR)" ]; then \
			$(CMAKE) -G "$(CMAKE_GENERATOR)" $(CMAKE_ARGS) .. ; \
		else \
			$(CMAKE) $(CMAKE_ARGS) .. ; \
		fi

build: configure
	@echo "Building (parallel jobs: $(NPROC))"
	@$(CMAKE) --build $(CMAKE_BUILD_DIR) --config $(BUILD_TYPE) -- -j$(NPROC)

run: build
	@echo "Running binary..."
	@exe="$(CMAKE_BUILD_DIR)/$(TARGET)"; \
	if [ -x "$$exe" ]; then \
		exec "$$exe"; \
	else \
		exe="$(CMAKE_BUILD_DIR)/$(BUILD_TYPE)/$(TARGET)"; \
		if [ -x "$$exe" ]; then \
			exec "$$exe"; \
		else \
			echo "Executable not found in $(CMAKE_BUILD_DIR)/ or $(CMAKE_BUILD_DIR)/$(BUILD_TYPE)/"; exit 1; \
		fi \
	fi

install: build
	@echo "Installing to $(PREFIX)..."
	@mkdir -p $(PREFIX)/bin
	@exe="$(CMAKE_BUILD_DIR)/$(TARGET)"; \
	if [ -x "$$exe" ]; then \
		cp "$$exe" "$(PREFIX)/bin/gitfinder"; \
	else \
		exe="$(CMAKE_BUILD_DIR)/$(BUILD_TYPE)/$(TARGET)"; \
		if [ -x "$$exe" ]; then \
			cp "$$exe" "$(PREFIX)/bin/gitfinder"; \
		else \
			echo "Executable not found; cannot install."; exit 1; \
		fi \
	fi

clean:
	@echo "Cleaning build outputs (keeps build dir)..."
	@find $(CMAKE_BUILD_DIR) -maxdepth 1 -mindepth 1 -not -name CMakeFiles -not -name CMakeCache.txt -exec rm -rf {} + || true

distclean:
	@echo "Removing entire build directory: $(CMAKE_BUILD_DIR)"
	@rm -rf $(CMAKE_BUILD_DIR) || true

reconfigure: distclean configure

# clang-format integration
format:
	@command -v clang-format >/dev/null 2>&1 || (echo "clang-format not found. Install clang-format and try again."; exit 1)
	@if [ -z "$(FORMAT_FILES)" ]; then echo "No source files found to format."; else \
		printf "Formatting %d files...\n" $(words $(FORMAT_FILES)); \
		printf "%s\n" $(FORMAT_FILES) | xargs -r clang-format -i; \
	fi

check-format:
	@command -v clang-format >/dev/null 2>&1 || (echo "clang-format not found. Install clang-format and try again."; exit 1)
	@echo "Checking formatting..."
	@set -e; \
	if [ -z "$(FORMAT_FILES)" ]; then \
		echo "No source files found to check."; \
	else \
		for f in $(FORMAT_FILES); do \
			tmp=$$f.clangformat.tmp; \
			clang-format -style=file "$$f" > "$$tmp"; \
			if ! cmp -s "$$f" "$$tmp"; then \
				echo "Formatting required: $$f"; \
				rm -f "$$tmp"; exit 1; \
			fi; \
			rm -f "$$tmp"; \
		done; \
		echo "All files properly formatted."; \
	fi