.PHONY: setup setup-prod build clean run

EXECUTABLE_NAME := conway

setup:
	cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug

setup-prod:
	cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

build:
	cmake --build build

run: 
	./build/$(EXECUTABLE_NAME)

.SILENT:

clean:
	rm -rf build
