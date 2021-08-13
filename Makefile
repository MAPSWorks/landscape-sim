.PHONY: build

TARGET_NAME = test-app


all: build run
full: clean generate build shaders
generate:
	# -DBUILD_SHARED_LIBS=YES
	cmake -G "Unix Makefiles" -S . -B build -D CMAKE_EXPORT_COMPILE_COMMANDS=1
build:
	cmake --build build --config Debug --target $(TARGET_NAME)
shaders:
	cmake --build build --target "$(TARGET_NAME)-shaders"
run:
	#Used for proper working directory
	(cd build/apps/test; ./$(TARGET_NAME))
clean:
	rm -rf build
memcheck:
	(cd build/apps/test; valgrind --leak-check=yes ./$(TARGET_NAME))
massif:
	(cd build/apps/test; valgrind --tool=massif ./$(TARGET_NAME))