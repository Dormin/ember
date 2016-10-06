#pragma once

#ifdef _WIN32
#ifdef EMBER_API_EXPORTS
#define EMBER_API __declspec(dllexport)
#else
#define EMBER_API __declspec(dllimport)
#endif // EMBER_API_EXPORTS
#else
#define EMBER_API
#endif // _WIN32
