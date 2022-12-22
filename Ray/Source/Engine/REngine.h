#pragma once

class RAY_API REngine;

namespace Engine {

	enum EngineMode : INT {
		NONE,
		DEBUG,		// extra info, slower but more checks
		RELEASE,	// No extra debug info, optimized for runtime
		EDITOR,		// Like game editor
		SERVER		// No graphics like a server
	};

	extern REngine g_RayEngine;

	VOID RAY_API SetMode(EngineMode mode);
	EngineMode RAY_API GetMode();

	WSTRING RAY_API EngineModeToString();

}

using namespace Engine;
class RAY_API REngine {
	
public:
	REngine();
	~REngine();

private:
	EngineMode m_EngineMode;

public:
	EngineMode GetEngineMode();
	VOID SetEngineMode(EngineMode mode);
};