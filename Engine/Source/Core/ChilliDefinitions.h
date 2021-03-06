#pragma once

#ifdef CHILLI_ENGINE
	#define CHILLI_CLASS class _declspec(dllexport)
	#define CHILLI_FUNC _declspec(dllexport)
#else
	#define CHILLI_CLASS class _declspec(dllimport)
	#define CHILLI_FUNC  _declspec(dllimport)
#endif

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
	#define NOMINMAX
#endif