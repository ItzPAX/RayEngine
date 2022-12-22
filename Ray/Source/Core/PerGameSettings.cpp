#include "Ray.h"

PerGameSettings* PerGameSettings::inst;

PerGameSettings::PerGameSettings()
{
	inst = this;

	wcscpy_s(inst->m_GameName, L"undefined");
	wcscpy_s(inst->m_ShortName, L"undefined");
	wcscpy_s(inst->m_BootTime, Time::GetDateTimeString(TRUE).c_str());
	wcscpy_s(inst->m_SplashURL, L"Content\\Engine\\Images\\RaySplash.bmp");
}

PerGameSettings::~PerGameSettings()
{
}