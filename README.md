#gitrel

A simple tool that shows in the commandline the status of your branches against remote branches etc.

I often find myself having to try and keep track of what's going on and this simplifies things

I will add utility as I develop this repository and try to have a 1 press command that shows most of the information a developer needs without having to perform git gymnastics.

###Example

Example terminal output: http://showterm.io/a94f295ba7ad822e2f22c#fast
```
$gitrel
```
Response should be similar to the following...
```
[REMOTES]
origin git@git.foo.com/remote.git
upstream git@git.bar.com/remote.git
[REFS]
ref:refs/heads/dynamic_method sha:[bfe48b6e079c51b5fc7a93318e559751f4a70101]
ref:refs/heads/master sha:[3cdf330cbc284f3f2c8ca7077916ca3903aa6e8f]
ref:refs/heads/shim sha:[d8c8c001c0b5ae90801f166d0b67f3e22c90a0fb]
ref:refs/heads/updating_calabash sha:[bfe48b6e079c51b5fc7a93318e559751f4a70101]
ref:refs/remotes/origin/features sha:[16c33f761818aad8038b2242a50e761fbfa7fb14]
ref:refs/remotes/origin/HEAD sha:[3cdf330cbc284f3f2c8ca7077916ca3903aa6e8f]
ref:refs/remotes/origin/master sha:[3cdf330cbc284f3f2c8ca7077916ca3903aa6e8f]
ref:refs/remotes/origin/pomv2 sha:[2687ae1cab019760906bfe57e7859f54a0775977]
ref:refs/remotes/origin/updating_calabash sha:[bfe48b6e079c51b5fc7a93318e559751f4a70101]
ref:refs/remotes/upstream/master sha:[14f4a5b96e5160cd6f38c78913897dc895f92e39]
[LOCAL BRANCHES]
0)dynamic_method [CURRENT] [ACTIVE PULL REQUEST]
1)master
2)shim
3)updating_calabash
[REMOTE BRANCHES]
0)origin/features
1)origin/HEAD
2)origin/master
3)origin/pomv2
4)origin/updating_calabash
5)upstream/master
```

##Deps
- OSX/Linux system
- CMake && GCC/LLVM
- libgit2 (Usually will be installed on your system when you add git)

```Shell
cmake .
make
sudo make install
```
