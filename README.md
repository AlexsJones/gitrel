#gitrel

A simple tool that shows in the commandline the status of your branches against remote branches etc.

I often find myself having to try and keep track of what's going on and this simplifies things

I will add utility as I develop this repository and try to have a 1 press command that shows most of the information a developer needs without having to perform git gymnastics.

example : http://showterm.io/2aecd85216ae1803a685e

##Deps
- OSX/Linux system
- CMake && GCC/LLVM
- libgit2 (Usually will be installed on your system when you add git)

```Shell
./make_deps //This installs submodules
cmake .
make
sudo make install
```
