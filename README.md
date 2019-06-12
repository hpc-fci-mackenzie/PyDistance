# PyDistance

| Branch |Status|
|--------|------------|
| Master |[![master Branch Build Status](https://travis-ci.org/hpc-fci-mackenzie/PyDistance.svg?branch=master)](https://travis-ci.org/hpc-fci-mackenzie/PyDistance)|
| Dev    |[![dev Branch Build Status](https://travis-ci.org/hpc-fci-mackenzie/PyDistance.svg?branch=dev)](https://travis-ci.org/hpc-fci-mackenzie/PyDistance)|

PyDistance is a set of distance measure implementations optimized for a variety of processors with SIMD support written in C++, able to be used from Python.

## Requirements

`CMake` >= 3.8

## Supported AVX Instruction Sets & Building

The setting `REGISTER_WIDTH` may be specified to take full advantage of the target architecture, as follows:

| AVX Instruction Set |REGISTER_WIDTH option|
|--------|------------|
|Auto Vectorization |auto|
|AVX-128 |128|
|AVX-256 |256|
|AVX-512 |512|

``` bash
$ mkdir build 
$ cd build
$ cmake .. [-D REGISTER_WIDTH=<auto|128|256|512>]
$ make
$ cd lib
```

## Features

## Usage

``` bash
$ cd lib
$ python main.py
```

## Contributing
