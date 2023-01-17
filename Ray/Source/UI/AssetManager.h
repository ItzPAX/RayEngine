#pragma once
#include <filesystem>
#include <unordered_map>

enum class AssetManagerFlags : int
{
	NONE = 0,
};

struct Asset
{
	std::filesystem::directory_entry m_Entry;

	// additional info about asset can be stored here
	bool m_Dir;
	std::string m_Extension;
	std::string m_Path;
	std::string m_Stem;
};

class RAY_API AssetManager
{
public:
	AssetManager();
	~AssetManager();

public:
	void Initialize(std::string name, AssetManagerFlags);
	void Render();

private:
	void GetFilesInPath();
	void RenderFileMenu(Asset asset);
	int GetButtonsInWindow();

private:
	std::string m_Name;
	std::string m_Path;

	int m_SelectedDrive = 0;
	char m_Drives[MAX_NAME_STRING];

	std::string m_CurrentlySelected;

	std::vector<const char*> m_DrivesVec;

	std::vector<Asset> m_Assets;
	std::unordered_map<std::string, bool> m_AssetActive;
};