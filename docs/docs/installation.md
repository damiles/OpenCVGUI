---
id: installation
title: Installation
permalink: docs/installation.html
next: hello-world.html
---

# Dependencies
* GLEW
* xorg-dev 

# Build
```bash
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make
sudo make install
```