.PHONY: setup build clean

setup:
	cmake -B build -S .

build:
	cmake --build build

clean:
	rm -rf build
