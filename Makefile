all:
	cd build ; make -j12

cmake:
	mkdir -p build ; cd build ; cmake .. -DCMAKE_BUILD_MODE=Debug ; make -j8

clean:
	rm -rf build
