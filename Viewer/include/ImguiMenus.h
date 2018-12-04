#pragma once

#ifndef __IMGUIMENUS_H__
#define __IMGUIMENUS_H__

#include <imgui/imgui.h>
#include "Scene.h"

void DrawImguiMenus(ImGuiIO& io, Scene* scene);
const glm::vec4& GetClearColor();

#endif // !__IMGUIMENUS_H__
