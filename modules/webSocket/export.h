#pragma once

#ifdef MYSOCKET_EXPORTS
#define MYSOCKET_API __declspec(dllexport)
#else
#define MYSOCKET_API __declspec(dllimport)
#endif