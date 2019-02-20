# Understandable RayTracing in 256 lines of bare C++

This repository is a support code for my computer graphics lectures. It is not meant to be the ultimate rendering code or even physically realistic. It is meant to be **simple**. This project is distributed under the [DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE](https://en.wikipedia.org/wiki/WTFPL).

**Check [the wiki](https://github.com/ssloy/tinyraytracer/wiki) that accompanies the source code. The second raytracing chapter is available [in the tinykaboom repository](https://github.com/ssloy/tinykaboom/wiki). If you are looking for a software rasterizer, check the [other part of the lectures](https://github.com/ssloy/tinyrenderer/wiki).**

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

You can open the project in Gitpod, a free online dev evironment for GitHub:

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/ssloy/tinyraytracer)

On open, the editor will compile & run the program as well as open the resulting image in the editor's preview.
Just change the code in the editor and rerun the script (use the terminal's history) to see updated images.
