#pragma once

#ifdef MYJSON_EXPORTS
#define MYJSON_API __declspec(dllexport)
#else
#define MYJSON_API __declspec(dllimport)
#endif