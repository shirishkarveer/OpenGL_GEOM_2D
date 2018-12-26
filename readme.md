Introduction
============

This is a learning project. It's not immediately intended to solve any particular problem.

Stack
=====
The programming language will be C++.

The target operating system will be Linux (probably Ubuntu flavours).

The initial demo will use OpenGL libraries to do some drawing functions.

Development Environment
=======================

- OS: Ubuntu or Ubuntu variant
- IDE: VSCode

Set `/etc/apt/sources.list` to:

```
deb     https://muug.ca/mirror/ubuntu cosmic           main restricted universe multiverse
deb-src https://muug.ca/mirror/ubuntu cosmic           main restricted universe multiverse
deb     https://muug.ca/mirror/ubuntu cosmic-updates   main restricted universe multiverse
deb-src https://muug.ca/mirror/ubuntu cosmic-updates   main restricted universe multiverse
deb     https://muug.ca/mirror/ubuntu cosmic-security  main restricted universe multiverse
deb-src https://muug.ca/mirror/ubuntu cosmic-security  main restricted universe multiverse
deb     https://muug.ca/mirror/ubuntu cosmic-backports main restricted universe multiverse
deb-src https://muug.ca/mirror/ubuntu cosmic-backports main restricted universe multiverse
```

You will need to run the following package installations:

```bash
sudo apt install make g++ git \
                 libglew-dev libglfw3-dev libglm-dev

# VSCode
wget -O vscode.deb https://go.microsoft.com/fwlink/?LinkID=760868
sudo dpkg -i vscode.deb
```
