#pragma once

#ifdef CHILLI_ENGINE
	#define CHILLI_CLASS _declspec(dllexport)
#else
	#define CHILLI_CLASS _declspec(dllimport)
#endif

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
	#define NOMINMAX
#endif