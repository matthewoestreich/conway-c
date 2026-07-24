.PHONY: setup build clean

setup:
	cmake -B build -S .

build:
	cmake --build build

.SILENT:

clean:
	rm -rf build
