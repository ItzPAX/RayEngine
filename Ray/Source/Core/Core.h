#pragma once

/*-------------------*/
/*   CORE FEATURES   */
/*-------------------*/

// Minimal Core Features
#include "CoreMinimal.h"

// imgui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>

// UI
#include "UI/UIDefinitions.h"
#include "UI/AssetManager.h"
#include "UI/UI.h"

// Graphics features
#include "Graphics/GraphicsDefinitions.h"
#include "Graphics/Camera.h"
#include "Graphics/FPS_Camera.h"
#include "Graphics/FloatingCamera.h"
#include "Graphics/UniformBuffer.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/VAO.h"
#include "Graphics/Graphics.h"