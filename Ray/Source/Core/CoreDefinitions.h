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

// defines for windows
#define DEFAULT_WIDTH 1000
#define DEFAULT_HEIGHT 600

//#define DirectX // Uncomment for DirectX implementation
#define OpenGL	// Uncomment for OpenGL implementation

typedef std::wstring WSTRING;
typedef std::string  STRING;