#include "Ray.h"
#include <fstream>
#include <ShlObj.h>
#include <cstdio>
#include <tlhelp32.h>

Logger* Logger::inst;

Logger::Logger()
{
	inst = this;
}

Logger::~Logger()
{

}

// Print to log file with optional arguments
VOID Logger::PrintLog(const WCHAR* fmt, ...) 
{
	WCHAR buf[4096];
	va_list args;

	va_start(args, fmt);
	vswprintf_s(buf, fmt, args);
	va_end(args);

	OutputDebugString(buf);

	// Example File and Location = %AppData%/BlankProject/Log/BlankProject03052021194056.log
	
	std::wfstream outfile;
	outfile.open(WSTRING(LogDirectory() + L"\\" + LogFile()), std::ios_base::app);

	if (outfile.is_open()) {
		WSTRING s = buf;
		outfile << L"[" << Time::GetDateTimeString() << L"]  " << s;
		outfile.close();
		OutputDebugString(s.c_str());
	}
	else {
		MessageBox(NULL, L"Unable to open log file...", L"Log Error", MB_OK);
	}
}

VOID Logger::PrintOGL3DError(const WCHAR* msg)
{
	std::wstringstream m;
	m << "OGL3D Error: " << msg << std::endl;
	PrintLog(m.str().c_str());

	_bstr_t b(m.str().c_str());
	throw std::runtime_error(b);
}

VOID Logger::PrintOGL3DWarning(const WCHAR* msg)
{
	std::wstringstream m;
	m << "OGL3D Warning: " << msg << std::endl;
	PrintLog(m.str().c_str());
}

VOID Logger::PrintOGL3DInfo(const WCHAR* msg)
{
	std::wstringstream m;
	m << "OGL3D Info: " << msg << std::endl;
	PrintLog(m.str().c_str());
}

// Get and Create Log Directory in AppData/$ProjectName/Log
WSTRING Logger::LogDirectory()
{
	WCHAR Path[1024];
	WCHAR* AppDataLocal;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &AppDataLocal);
	wcscpy_s(Path, AppDataLocal);
	wcscat_s(Path, L"\\");
	wcscat_s(Path, PerGameSettings::GameName());
	CreateDirectory(Path, NULL);
	wcscat_s(Path, L"\\Log");
	CreateDirectory(Path, NULL);
	return Path;
}

WSTRING Logger::LogFile()
{
	WCHAR File[1024];
	wcscpy_s(File, PerGameSettings::GameName());
	wcscat_s(File, PerGameSettings::BootTime());
	wcscat_s(File, L".log");
	return File;
}

/* Print a seperator line without time stamp */
VOID Logger::PrintDebugSeperator()
{
	WSTRING s = L"\n------------------------------------------------------------------------------------\n\n";

#ifdef _DEBUG
	std::wfstream outfile;
	outfile.open(WSTRING(LogDirectory() + L"/" + LogFile()), std::ios_base::app);

	if (outfile.is_open()) {
		outfile << s;
		outfile.close();
	}
	else {
		MessageBox(NULL, L"Unable to open log file...", L"Log Error", MB_OK);
	}
#endif
}

/* Private class to check to see if MTail is already running - So we don't open multiple copies during debug */
BOOL Logger::IsMTailRunning()
{
	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_wcsicmp(entry.szExeFile, L"mTAIL.exe"))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}

/* Start MTail from Project or Build Directory - Depends on where ran from */
VOID Logger::StartMTail()
{
	if (IsMTailRunning()) {
		Logger::PrintLog(L"--MTail failed to start - Already Running\n");
		return;
	}

	Logger::PrintLog(L"--Starting MTail\n");
	WCHAR path[MAX_PATH] = { 0 };
	GetCurrentDirectoryW(MAX_PATH, path);
	WSTRING url = path + WSTRING(L"/mTAIL.exe");
	WSTRING params = L" \"" + LogDirectory() + L"/" + LogFile() + L"\" /start";
	ShellExecute(0, NULL, url.c_str(), params.c_str(), NULL, SW_SHOWDEFAULT);
}