#pragma once
#pragma warning(disable:4251)
#include "spdlog/spdlog.h"

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

#define CHILLI_INFO(...)   spdlog::info(__VA_ARGS__)
#define CHILLI_WARN(...)   spdlog::warn(__VA_ARGS__)
#define CHILLI_ERROR(...)  spdlog::error(__VA_ARGS__)
