#pragma once

#include "../SDK/Materialsystem_config.h"
#include "../../settings.h"

namespace MaterialConfig
{
	extern MaterialSystem_Config_t backupConfig;

	void OverrideConfig(MaterialSystem_Config_t *gameConfig, bool forceUpdate);

}

namespace Material
{
	void RenderAimware(ImVec2 &pose, ImDrawList *draw, int sideTabIndex);
}