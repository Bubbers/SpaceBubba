SpaceBubba
=========================

This is a small 3D space-shooter game made during Ludum Dare 34 in our own game-engine called Bubba-3D. Your mission is to destroy 50 of the asteroids in this system. Be aware that you do not have any shields. One hit will destroy you. Use w and S to thrust, A and D to rotate, P and L to change altitude and R to fire the laser.

Prerequisites:
* <a href="http://www.cmake.org/">CMake</a>
* <a href="http://assimp.sourceforge.net/">Assimp</a>
* <a href="http://freeglut.sourceforge.net/">Glut (Freeglut)</a>
* <a href="http://glew.sourceforge.net/">Glew</a>
* <a href="http://freeimage.sourceforge.net/">FreeImage</a><br />
* <a href="http://www.sfml-dev.org/index.php">SFML</a><br />

Install dependencies (Debian, Ubuntu):
```bash
git submodule git@github:Bubbers/Bubba-3D.git
git submodule init
git submodule update
cd Bubba-3D
git checkout develop
cd ..
make install
```

Compile:
```bash
make
```

Compile and run executable:
```bash
make run
```

Make binary:
```bash
make release
```



