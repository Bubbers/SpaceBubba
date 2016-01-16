COMPILESTRING = mkdir -p build;cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make -j16;

all:
	$(COMPILESTRING)

clean:
	rm -rf build;

run:
	$(COMPILESTRING) ./Super-Bubba-Awesome-Space;

install:
	cd Bubba-3D; make install


release: all
	rm -rf Release/executable; rm -rf Release/scenes; rm -rf Release/shaders; mkdir Release/executable; cp build/Bubba3D Release/executable/; cp -r scenes Release/; cp -r shaders Release/;
