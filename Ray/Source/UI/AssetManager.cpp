#include "Ray.h"

AssetManager::AssetManager()
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
	_getcwd(cCurrentPath, sizeof(cCurrentPath));
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
}

void AssetManager::Render()
{
	GetFilesInPath();

	ImGui::Begin(m_Name.c_str());
	{
		ImGui::TextWrapped(m_Path.c_str());
		if (ImGui::Combo("Drive", &m_SelectedDrive, &m_DrivesVec[0], m_DrivesVec.size()))
		{
			m_Path = m_DrivesVec.at(m_SelectedDrive);
		}

		ImGui::Columns(max(GetButtonsInWindow(), 1), 0, 0);

		for (auto asset : m_Assets)
		{
			// menu screen for our asset
			if (m_AssetActive[asset.m_Entry.path().string()])
			{
				std::string label = asset.m_Entry.path().stem().string();
				label.append("##");
				label.append(asset.m_Entry.path().string());
				ImGui::SetNextWindowSize(ImVec2(250, 600), ImGuiCond_Appearing);
				ImGui::Begin(label.c_str(), &m_AssetActive[asset.m_Entry.path().string()]);
				{
					RenderFileMenu();
				}
				ImGui::End();
			}

			std::string label = asset.m_Entry.is_directory() ? "[D]##" : "[F]##";
			label.append(asset.m_Entry.path().string());

			if (ImGui::Button(label.c_str(), ImVec2(64, 64)))
			{
				// dir
				if (asset.m_Entry.is_directory() && asset.m_Entry.path().string() != m_Path)
				{
					m_Path = asset.m_Entry.path().string();
				}
				// go back
				else if (asset.m_Entry.path().string() == m_Path)
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
					int tocopy = seglist.size() == 1 ? 1 : seglist.size() - 1;

					for (int i = 0; i < tocopy; i++)
					{
						newpath.append(seglist.at(i));
						newpath.append("\\");
					}

					m_Path = newpath;
				}

				// file
				else
				{
					m_AssetActive[asset.m_Entry.path().string()] = !m_AssetActive[asset.m_Entry.path().string()];
				}
			}
			if (asset.m_Entry.path().string() == m_Path)
				ImGui::TextWrapped("..");
			else
				ImGui::TextWrapped(asset.m_Entry.path().stem().string().c_str());

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
	Asset asset{ entry };
	m_Assets.push_back(asset);

	for (const auto& entry : std::filesystem::directory_iterator(m_Path))
	{
		Asset asset{ entry };
		DWORD attributes = GetFileAttributes(entry.path().wstring().c_str());
		
		if (attributes & FILE_ATTRIBUTE_SYSTEM || attributes & FILE_ATTRIBUTE_ENCRYPTED || attributes & FILE_ATTRIBUTE_HIDDEN)
			continue;

		m_Assets.push_back(asset);
	}
}

void AssetManager::RenderFileMenu()
{
	ImGui::Button("Delete", ImVec2(128,128));
	ImGui::SameLine();
	ImGui::Text("Delete object");
}

int AssetManager::GetButtonsInWindow()
{
	int buttonspossible = 0;

	// Manually wrapping (we should eventually provide this as an automatic layout feature, but for now you can do it manually)
	ImVec2 button_sz(64, 64);
	ImGuiStyle& style = ImGui::GetStyle();
	int buttons_count = m_Assets.size();
	float window_visible_x2 = ImGui::GetWindowContentRegionMax().x;
	for (int n = 0; n < buttons_count; n++)
	{
		float next_button_x2 = (64 * n) + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
		if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
			buttonspossible++;
	}

	return buttonspossible;
}