Prototype OS
============

[![Build Status](https://travis-ci.org/pelallemant/prototype-os.png?branch=master)](https://travis-ci.org/pelallemant/prototype-os)

This is a prototype of Operating System. Based on PepinOS tutorials for the first steps.

To test it, you can use:

- qemu
- TODO: bochs (configuration file to write)
- TODO: virtualbox (correct iso to generate)


Requirements: 
=============

Ubuntu
------

`sudo apt-get install libc6-dev-i386`

Compilation
===========

cd build && make

Objectives
==========

- Continuous-Integration with Travis-CI (expects the OS to be stored in an iso, and it's the problem: first try with grub2 failed. A test with LILO may be done).

