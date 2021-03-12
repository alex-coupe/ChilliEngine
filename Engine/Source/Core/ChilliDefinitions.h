#pragma once

#ifdef CHILLI_ENGINE
	#define CHILLI_API _declspec(dllexport)
#else
	#define CHILLI_API  _declspec(dllimport)
#endif

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
	#define NOMINMAX
#endif

#ifdef DEBUG
#include <iostream>
#include <stdlib.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define ENGINE_ERROR(...) std::cout << "[ERROR] " << (__VA_ARGS__)
#define ENGINE_INFO(...) std::cout << (__VA_ARGS__)
#endif