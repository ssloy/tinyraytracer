# Homework assignment

Add environment maps:
![](https://raw.githubusercontent.com/ssloy/tinyraytracer/homework_assignment/out-envmap.jpg)

... and triangulated meshes:
![](https://raw.githubusercontent.com/ssloy/tinyraytracer/homework_assignment/out-envmap-duck.jpg)

All the boring code (image/obj files serializer etc) is already there; to obtain the above image it suffices to add 10-20 lines of code.


## compilation
```sh
git clone --recurse-submodules https://github.com/ssloy/tinyraytracer.git
cd tinyraytracer
git checkout homework_assignment
git submodule update --init
mkdir build
cd build
cmake ..  
make
```
You can open the project in Gitpod, a free online dev evironment for GitHub:

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/ssloy/tinyraytracer/tree/homework_assignment)

On open, the editor will compile & run the program as well as open the resulting image in the editor's preview.
Just change the code in the editor and rerun the script (use the terminal's history) to see updated images.
