COMPILESTRING = mkdir -p build;cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make -j16;

all:
	$(COMPILESTRING)

tests: $(all)
	$(COMPILESTRING) make test;

testsX: $(all)
	$(COMPILESTRING) ctest -V;

clean:
	rm -rf build;

run:
	$(COMPILESTRING) ./Super-Bubba-Awesome-Space;

install:
	-sudo add-apt-repository ppa:ubuntu-toolchain-r/test
	-sudo apt-get update
	-sudo apt-get install gcc-4.9 g++-4.9
	-sudo apt-get install freeglut3-dev libjpeg-dev libfreetype6-dev libxrandr-dev libglew-dev libsndfile1-dev libopenal-dev libudev-dev
	-sudo apt-get install software-properties-common libfreeimage3 libfreeimage-dev libassimp-dev libglw1-mesa-dev libglew-dev libxmu-dev libxi-dev; sudo add-apt-repository ppa:george-edison55/cmake-3.x; sudo apt-get update; sudo apt-get install cmake;


release: all
	rm -rf Release/executable; rm -rf Release/scenes; rm -rf Release/shaders; mkdir Release/executable; cp build/Bubba3D Release/executable/; cp -r scenes Release/; cp -r shaders Release/;
