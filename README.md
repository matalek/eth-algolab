# Algorithms Lab 2016

## Overview
This repository consists of my solutions for course Algorithms Lab at ETH Zurich, which I attended during Autumn Semester 2016.

The solutions for the problems needed to be correct, fast and created in a limited amount of time. Therefore, the coding style for these solutions is not always perfect.

My solutions were tested using automated online judge and in most cases achieved maximal number of points.

## Compiling
For simple programs which use only STL for compilation use:
```
g++ -std=c++11 <file_name>
```
For programs using BGL we compile using:
```
g++ -std=c++11 -I /usr/local/include/ <file_name>
```
For programs using CGAL it is necessary to first create Makefile using:
```
cmake .
```
and then running it:
```
make
```