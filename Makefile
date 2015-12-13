COMPILESTRING = mkdir -p build;cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make;

all:
	$(COMPILESTRING)

tests: $(all)
	$(COMPILESTRING) make test;

testsX: $(all)
	$(COMPILESTRING) ctest -V;

run:
	$(COMPILESTRING) ./Bubba3D;

clean:
	rm -rf build;

install:
	sudo apt-get install g++ software-properties-common freeglut3-dev libfreeimage3 libfreeimage-dev libassimp-dev libglw1-mesa-dev libglew-dev libxmu-dev libxi-dev libopenal1 libalut0 libalut-dev libopenal-dev libvorbis-dev; sudo add-apt-repository ppa:george-edison55/cmake-3.x; sudo apt-get update; sudo apt-get install cmake;

release: all
	rm -rf Release/executable; rm -rf Release/scenes; rm -rf Release/shaders; mkdir Release/executable; cp build/Bubba3D Release/executable/; cp -r scenes Release/; cp -r shaders Release/;