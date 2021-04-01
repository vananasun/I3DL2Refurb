#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#define SAFE_DELETE_PTR(ptr) {if(ptr){delete ptr;}}
#define MIN(a,b) ((((a)>(b))?(b):(a)))
#define MAX(a,b) ((((a)>(b))?(a):(b)))
