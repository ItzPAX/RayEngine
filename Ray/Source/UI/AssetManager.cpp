#include "Ray.h"

AssetManager::AssetManager()
	: m_Drives("")
{
	
}

AssetManager::~AssetManager()
{

}

void AssetManager::Initialize(std::string name, AssetManagerFlags flags)
{
	m_Name = name;

	// init path to our working dir
	char cCurrentPath[FILENAME_MAX];
	if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
		m_Path = "C:\\";
	else
		m_Path = cCurrentPath;

	// set bools according to flags etc.


	// get all drives on the pc
	GetLogicalDriveStringsA(MAX_NAME_STRING, m_Drives);

	char* szSingleDrive = m_Drives;

	while (*szSingleDrive)
	{
		m_DrivesVec.push_back(szSingleDrive);

		// get the next drive
		szSingleDrive += strlen(szSingleDrive) + 1;
	}

	GetFilesInPath();
}

void AssetManager::Render()
{
	if (!m_AssetManager)
		return;

	ImGui::Begin(m_Name.c_str(), &m_AssetManager);
	{
		ImGui::TextWrapped(m_Path.c_str());
		if (ImGui::Combo("Drive", &m_SelectedDrive, &m_DrivesVec[0], (int)m_DrivesVec.size()))
		{
			m_Path = m_DrivesVec.at(m_SelectedDrive);
			GetFilesInPath();
		}

		ImGui::Columns(glm::max(GetButtonsInWindow(), 1), 0, 0);

		for (auto asset : m_Assets)
		{
			// menu screen for our asset
			if (m_AssetActive[asset.m_Path])
			{
				std::string label = asset.m_Stem + "##" + asset.m_Path;
				ImGui::SetNextWindowPos(ImGui::GetItemRectMin(), ImGuiCond_Appearing);
				ImGui::SetNextWindowSize(ImVec2(128, 192), ImGuiCond_Always);
				ImGui::Begin(label.c_str(), &m_AssetActive[asset.m_Path], ImGuiWindowFlags_NoResize);
				{
					RenderFileMenu(asset);
				}
				ImGui::End();
			}

			std::string label = asset.m_Dir ? "[D]##" : asset.m_Extension + "##" + asset.m_Path;

			ImGui::Button(label.c_str(), ImVec2(64, 64));
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				// dir
				if (asset.m_Dir && asset.m_Path != m_Path)
				{
					m_Path = asset.m_Path;
					GetFilesInPath();
				}
				// go back
				else if (asset.m_Path == m_Path)
				{
					std::stringstream test(m_Path);
					std::string segment;
					std::vector<std::string> seglist;

					while (std::getline(test, segment, '\\'))
					{
						seglist.push_back(segment);
					}

					// make new string, but dont use the last segment
					std::string newpath;
					int tocopy = (int)seglist.size() == 1 ? 1 : (int)seglist.size() - 1;

					for (int i = 0; i < tocopy; i++)
					{
						newpath.append(seglist.at(i));
						newpath.append("\\");
					}

					m_Path = newpath;
					GetFilesInPath();
				}

				// file
				else
				{
					m_AssetActive.clear();
					m_AssetActive[asset.m_Path] = !m_AssetActive[asset.m_Path];
				}
			}

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && !asset.m_Dir)
			{
				m_AssetActive.clear();
				m_AssetActive[asset.m_Path] = !m_AssetActive[asset.m_Path];
			}

			if (asset.m_Path == m_Path)
				ImGui::TextWrapped("..");
			else
			{
				std::string str = asset.m_Stem + asset.m_Extension;
				ImGui::TextWrapped(str.c_str());
			}

			ImGui::NextColumn();
		}

		ImGui::Columns(1);
	}
	ImGui::End();
}

void AssetManager::GetFilesInPath()
{
	m_Assets.clear();

	std::filesystem::directory_entry entry(m_Path);
	Asset asset{ entry, entry.is_directory(), entry.path().extension().string(), entry.path().string(), entry.path().stem().string() };
	m_Assets.push_back(asset);

	for (const auto& entry : std::filesystem::directory_iterator(m_Path))
	{
		DWORD attributes = GetFileAttributes(entry.path().wstring().c_str());	
		if (attributes & FILE_ATTRIBUTE_SYSTEM || attributes & FILE_ATTRIBUTE_ENCRYPTED || attributes & FILE_ATTRIBUTE_HIDDEN)
			continue;

		Asset asset{ entry, entry.is_directory(), entry.path().extension().string(), entry.path().string(), entry.path().stem().string() };
		m_Assets.push_back(asset);
	}
}

void AssetManager::RenderFileMenu(Asset asset)
{
	if (ImGui::Button("Delete", ImVec2(-1, 0)))
	{
		remove(asset.m_Path.c_str());
		GetFilesInPath();
	}
	if (ImGui::Button("Load Model", ImVec2(-1, 0)))
	{
		// TEMPORARY SOLUTION
		Graphics::Instance()->CreateModel(asset.m_Path.c_str(), { "C:/Users/Deniz/Desktop/RayEngine/Build/Release/Content/Engine/Shaders/BasicShader.vert", "C:/Users/Deniz/Desktop/RayEngine/Build/Release/Content/Engine/Shaders/BasicShader.frag" });
		GetFilesInPath();
	}
}

int AssetManager::GetButtonsInWindow()
{
	int buttonspossible = 0;

	// Manually wrapping (we should eventually provide this as an automatic layout feature, but for now you can do it manually)
	ImVec2 button_sz(64, 64);
	int buttons_count = (int)m_Assets.size();
	float window_visible_x2 = ImGui::GetWindowContentRegionMax().x;
	for (int n = 0; n < buttons_count; n++)
	{
		float next_button_x2 = (64 * n) + button_sz.x; // Expected position if next button was on same line
		if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
			buttonspossible++;
	}

	return buttonspossible;
}