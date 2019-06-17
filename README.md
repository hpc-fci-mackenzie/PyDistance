# PyDistance 

| Branch |Status|
|--------|------------|
| [Master](https://github.com/hpc-fci-mackenzie/PyDistance/) |[![master Branch Build Status](https://travis-ci.org/hpc-fci-mackenzie/PyDistance.svg?branch=master)](https://travis-ci.org/hpc-fci-mackenzie/PyDistance)|
| [Dev](https://github.com/hpc-fci-mackenzie/PyDistance/tree/dev)    |[![dev Branch Build Status](https://travis-ci.org/hpc-fci-mackenzie/PyDistance.svg?branch=dev)](https://travis-ci.org/hpc-fci-mackenzie/PyDistance)|

PyDistance is a set of distance measure implementations optimized for a variety of processors with SIMD support written in C++, able to be used from Python.

## Requirements

`CMake` >= 3.8

## Supported AVX Instruction Sets

| AVX Instruction Set |REGISTER_WIDTH option|
|--------|------------|
|Auto Vectorization |auto|
|AVX-128 |128|
|AVX-256 |256|
|AVX-512 |512|

## Building

The setting `REGISTER_WIDTH` may be specified to take full advantage of the target architecture, as follows:

``` bash
$ mkdir build 
$ cd build
$ cmake .. [-D REGISTER_WIDTH=<auto|128|256|512>]
$ make
$ cd lib
```

<!-- ## Features -->

## Usage

``` python
>>> import PyDistance as pd
>>> import numpy as np
>>> a=np.random.randint(0, 10, 8)
>>> a
array([4, 3, 5, 1, 4, 5, 7, 7])
>>> b=np.random.randint(0, 10, 8)
>>> b
array([1, 3, 3, 4, 1, 4, 3, 6])
>>> pd.euclidean(a,b)
7.0
```

## Platforms
|![alt text](https://img.icons8.com/ios/24/000000/linux.png)|
|-|
|![alt text](https://img.icons8.com/material/24/000000/mac-os.png)|
|![alt text](https://img.icons8.com/material/24/000000/windows-logo.png)|

## Contributing
PyDistance is a research project and for now, we are a small team! 
We actively encourage and support contributions. 
The source code is released under the GNU License.
