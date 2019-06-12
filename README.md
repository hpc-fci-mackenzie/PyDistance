# PyDistance
![Build Status](https://travis-ci.org/hpc-fci-mackenzie/edcalc.svg?branch=master)

PyDistance is a set of distance measure implementations optimized for a variety of processors with SIMD support written in C, able to be used from Python.

## Requirements

`CMake` >= 3.8

## Supported AVX Instruction Sets
- AVX-128
- AVX-256
- AVX-512

## Building

The setting `REGISTER_WIDTH` may be specified to take full advantage of the target architecture.

``` bash
$ mkdir build 
$ cd build
$ cmake .. [-D REGISTER_WIDTH=<auto|128|256|512>] # optionally set
$ make
$ cd lib
```

## Usage

``` bash
$ cd lib
$ python main.py
```

## Contributing
