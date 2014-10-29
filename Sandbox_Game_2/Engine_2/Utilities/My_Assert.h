#ifndef ENGINE_MY_ASSERT_H
#define ENGINE_MY_ASSERT_H

#ifdef DEBUG
#include <cassert>
#define MY_ASSERT(x) assert(x)
#else
#define MY_ASSERT(x) // define as nothing
#endif

#endif