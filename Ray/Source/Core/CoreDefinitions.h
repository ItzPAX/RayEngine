#pragma once

// DLL Im/Export
#ifdef BUILD_DLL
	#define RAY_API __declspec(dllexport)
#else
	#define RAY_API __declspec(dllimport)
#endif

// defines
#define MAX_NAME_STRING 256
#define HInstance() GetModuleHandle(NULL)

typedef std::wstring WSTRING;
typedef std::string  STRING;