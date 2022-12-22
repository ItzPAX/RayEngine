#include "Ray.h"

namespace Engine {

	REngine g_RayEngine;

	VOID SetMode(EngineMode mode)
	{
		g_RayEngine.SetEngineMode(mode);
	}


	EngineMode GetMode()
	{
		return g_RayEngine.GetEngineMode();
	}

	WSTRING RAY_API EngineModeToString()
	{
		switch (Engine::GetMode())
		{
		case EngineMode::DEBUG:		return L"Debug";
		case EngineMode::RELEASE:	return L"Release";
		case EngineMode::SERVER:	return L"Server";
		case EngineMode::EDITOR:	return L"Editor";
		default:     return L"None";
		}
	}
}

REngine::REngine()
{
#ifdef _DEBUG
	m_EngineMode = EngineMode::DEBUG;
#else
	m_EngineMode = EngineMode::RELEASE;
#endif
}

REngine::~REngine()
{
}

EngineMode REngine::GetEngineMode()
{
	return m_EngineMode;
}

VOID REngine::SetEngineMode(EngineMode mode)
{
	m_EngineMode = mode;
}
