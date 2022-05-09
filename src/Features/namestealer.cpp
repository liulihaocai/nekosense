#include "namestealer.h"

#include "namechanger.h"
#include "../Utils/util.h"
#include "../Utils/xorstring.h"
#include "../settings.h"
#include "../interfaces.h"
#include "../Utils/entity.h"

void NameStealer::CreateMove(CUserCmd *cmd)
{
	if (!Settings::NameStealer::enabled)
		return;

	if (!engine->IsInGame())
		return;

	C_BasePlayer *localplayer = (C_BasePlayer *)entityList->GetClientEntity(engine->GetLocalPlayer());

	if (!localplayer)
		return;

	static std::vector<int> stolenIds;

	for (int i = engine->GetMaxClients(); i > 0; i--)
	{
		C_BasePlayer *player = (C_BasePlayer *)entityList->GetClientEntity(i);

		if (!player || player == localplayer)
			continue;

		if (Settings::NameStealer::team == 0 && !Entity::IsTeamMate(player, localplayer))
			continue;

		if (Settings::NameStealer::team == 1 && Entity::IsTeamMate(player, localplayer))
			continue;

		IEngineClient::player_info_t entityInformation;
		engine->GetPlayerInfo(i, &entityInformation);

		if (entityInformation.fakeplayer)
			continue;

		if (std::find(stolenIds.begin(), stolenIds.end(), i) != stolenIds.end())
			continue;

		if (NameChanger::changeName(false, Util::PadStringRight(entityInformation.name, strlen(entityInformation.name) + 1), 1.f))
		{
			stolenIds.push_back(i);
		}
		return;
	}
	stolenIds.clear();
}
