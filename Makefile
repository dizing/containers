# MAKEFILE WRAPS CMAKE
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
BROWSER_OPENER = @open
endif
ifeq ($(UNAME), Linux)
BROWSER_OPENER = @x-www-browser
endif
GNU_COMPILER = -D CMAKE_CXX_COMPILER=g++ -D CMAKE_C_COMPILER=gcc
.PHONY: clean test gcov_report

all: clean test

clean:
	@rm -rf buildRelease 2>/dev/null || true
	@rm -rf buildDebug 2>/dev/null || true
	@rm *.tar.gz 2>/dev/null || true
	@rm *.a 2>/dev/null || true
	@rm *.h 2>/dev/null || true

test: buildRelease
	@cmake --build buildRelease --target test
	@echo "\033[0;32m-----------TESTS------------:\033[0m"
	@./buildRelease/test
	@echo "\033[0;32m----------------------------:\033[0m"
	@echo "\033[0;32m------CHECK FORMATTING------:\033[0m"
	@cmake --build buildRelease --target format-check
	@echo "\033[0;32m----------------------------:\033[0m"

gcov_report: buildDebug
	@cmake --build buildDebug --target test_coverage
	${BROWSER_OPENER} buildDebug/test_coverage/index.html
	
buildRelease:
	@cmake -S . -B buildRelease -D CMAKE_BUILD_TYPE=Release
buildDebug:
	@cmake -S . -B buildDebug $(GNU_COMPILER) -D CMAKE_BUILD_TYPE=Debug 
