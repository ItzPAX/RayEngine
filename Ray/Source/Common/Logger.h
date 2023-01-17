#pragma once
#include <string>

class RAY_API Logger {

	//Getters and Setters for singleton static class
private:
	static Logger* inst;
public:
	static Logger* Instance() { return inst; }

	//Constructor
public:
	Logger();
	~Logger();

	// Print to Log File
	static VOID PrintLog(const WCHAR* fmt, ...);

	// OGL3D Messsages
	static VOID PrintOGL3DError(const WCHAR* msg);
	static VOID PrintOGL3DWarning(const WCHAR* msg);
	static VOID PrintOGL3DInfo(const WCHAR* msg);

	// Get Log Directory & File
	static WSTRING LogDirectory();
	static WSTRING LogFile();

	/* Print a line of '-' char's */
	static VOID PrintDebugSeperator();

	/* Check to see if MTail is already Running */
	static BOOL IsMTailRunning();

	/* Start MTail Application */
	static VOID StartMTail();

public:
	std::vector<WSTRING> m_Logs;
};