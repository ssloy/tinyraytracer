# Understandable RayTracing in 256 lines of bare C++

This repository is a support code for my computer graphics lectures. It is not meant to be the ultimate rendering code or even physically realistic. It is meant to be **simple**. This project is distributed under the [DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE](https://en.wikipedia.org/wiki/WTFPL).

Check [the article](https://github.com/ssloy/tinyraytracer/wiki) that accompanies the source code.
If you are looking for a software rasterizer, check the [other part of the lectures](https://github.com/ssloy/tinyrenderer/wiki).  

In my lectures I tend to avoid third party libraries as long as it is reasonable, because it forces to understand what is happening under the hood. So, the raytracing 256 lines of plain C++ give us this result:  
![](https://raw.githubusercontent.com/ssloy/tinyraytracer/master/out.jpg)

## compilation
```sh
git clone https://github.com/ssloy/tinyraytracer.git  
cd tinyraytracer  
mkdir build  
cd build  
cmake ..  
make
```
# Homework assignment
[homework_assignment branch](https://github.com/ssloy/tinyraytracer/tree/homework_assignment) contains all necessary stuff to easily add environment maps:
![](https://raw.githubusercontent.com/ssloy/tinyraytracer/homework_assignment/out-envmap.jpg)

... and triangulated meshes:
![](https://raw.githubusercontent.com/ssloy/tinyraytracer/homework_assignment/out-envmap-duck.jpg)

All boring code (image/obj files serializer etc) is already added; to obtain the above image ot suffices to add 10-20 lines of code.
