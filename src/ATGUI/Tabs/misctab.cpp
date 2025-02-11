#include "misctab.h"

#include <sys/stat.h>
#include <sstream>

#include "../../config.h"
#include "../../interfaces.h"
#include "../../Kali.h"
#include "../../settings.h"
#include "../../Utils/xorstring.h"
#include "../imgui.h"
#include "../../ImGUI/imgui_internal.h"
#include "../atgui.h"
#include "../../Features/namechanger.h"
#include "../../Features/profilechanger.h"
#include "../../Features/namechanger.h"
#include "../../Features/namestealer.h"
#include "../../Features/grenadehelper.h"
#include "../../Features/clantagchanger.h"
#include "../../Features/valvedscheck.h"
#include "../../Features/resolver.h"
#include "../Windows/configs.h"
#include "../../Utils/entity.h"
#include <stdint.h>
#pragma GCC diagnostic ignored "-Wformat-security"

static char nickname[127] = "";

void MiscCustomizations::RenderMainMenu(ImVec2 &pos, ImDrawList *draw, int sideTabIndex)
{
	const char *mainWeapons[] = {"None", "Auto", "AWP", "SSG 08"};
	const char *secondaryWeapons[] = {"None", "Dual Berettas", "Deagle/Revolver"};
	const char *altwep[] = {"Grenades", "Armor", "Taser", "Defuser"};
	const char *strafeTypes[] = {"Forwards", "Backwards", "Left", "Right", "Rage", "Directional"};
	const char *animationTypes[] = {"Static", "Marquee", "Words", "Letters", "Rotate", "Underline"};
	const char *musicType[] = {"CSGO", "CSGO2", "Crimson_Assault", "Sharpened", "Insurgency", "ADB", "High_Moon", "Deaths_Head_Demolition", "Desert_Fire", "LNOE", "Metal", "All_I_Want_for_Christmas", "IsoRhythm", "For_No_Mankind", "Hotline_Miami", "Total_Domination", "The_Talos_Principle", "Battlepack", "MOLOTOV", "Uber_Blasto_Phone", "Hazardous_Environments", "II-Headshot", "The_8-Bit_Kit", "I_Am", "Diamonds", "Invasion!", "Lions_Mouth", "Sponge_Fingerz", "Disgusting", "Java_Havana_Funkaloo", "Moments_CSGO", "Aggressive", "The_Good_Youth", "FREE", "Lifes_Not_Out_to_Get_You", "Backbone", "GLA", "III-Arena", "EZ4ENCE"};
	const char *spammerTypes[] = {"None", "Normal", "Positions"};
	const char *teams[] = {"Allies", "Enemies", "Both"};
	const char *grenadeTypes[] = {"FLASH", "SMOKE", "MOLOTOV", "HEGRENADE"};
	const char *throwTypes[] = {"NORMAL", "RUN", "JUMP", "WALK"};

	ImGui::BeginGroup();
	{
		ImGui::Columns(2, nullptr, false);
		{
			ImGui::BeginChild(XORSTR("Child1"), ImVec2(0, 781), true);
			{
				ImGui::Separator();
				ImGui::Text(XORSTR("Movement"));
				ImGui::Separator();

				ImGui::Checkbox(XORSTR("Bunny Hop"), &Settings::BHop::enabled);

				ImGui::Columns(2, nullptr, false);
				{
					ImGui::Checkbox(XORSTR("Hop Chance"), &Settings::BHop::Chance::enabled);
					ImGui::Checkbox(XORSTR("Min Hops"), &Settings::BHop::Hops::enabledMin);
					ImGui::Checkbox(XORSTR("Max Hops"), &Settings::BHop::Hops::enabledMax);
					ImGui::Checkbox(XORSTR("Auto Strafe"), &Settings::AutoStrafe::enabled);
					ImGui::Checkbox(XORSTR("Edge Jump"), &Settings::EdgeJump::enabled);
				}
				ImGui::NextColumn();
				{
					ImGui::PushItemWidth(-1);
					ImGui::SliderInt(XORSTR("##BHOPCHANCE"), &Settings::BHop::Chance::value, 0, 100);
					ImGui::SliderInt(XORSTR("##BHOPMIN"), &Settings::BHop::Hops::Min, 0, 20);
					ImGui::SliderInt(XORSTR("##BHOPMAX"), &Settings::BHop::Hops::Max, 0, 20);
					ImGui::PushItemWidth(Settings::AutoStrafe::type == AutostrafeType::AS_RAGE ? ImGui::CalcItemWidth() : -1);
					ImGui::Combo(XORSTR("##STRAFETYPE"), (int *)&Settings::AutoStrafe::type, strafeTypes, IM_ARRAYSIZE(strafeTypes));

					if (Settings::AutoStrafe::type == AutostrafeType::AS_RAGE)
					{
						ImGui::SameLine();
						ImGui::Checkbox(XORSTR("Silent"), &Settings::AutoStrafe::silent);
					}
					ImGui::PopItemWidth();
					UI::KeyBindButton(&Settings::EdgeJump::key);
				}
				ImGui::EndColumns();
				ImGui::Separator();
				ImGui::Text(XORSTR("Spammer"));
				ImGui::Separator();

				ImGui::Columns(3, nullptr, false);
				{
					ImGui::Checkbox(XORSTR("Kill Messages"), &Settings::Spammer::KillSpammer::enabled);
				}
				ImGui::NextColumn();
				{
					ImGui::Checkbox(XORSTR("Team Chat###SAY_TEAM1"), &Settings::Spammer::KillSpammer::sayTeam);
				}
				ImGui::NextColumn();
				{
					if (ImGui::Button(XORSTR("Options###KILL"), ImVec2(-1, 0)))
						ImGui::OpenPopup(XORSTR("options_kill"));

					ImGui::SetNextWindowSize(ImVec2(565, 268), ImGuiCond_Always);
					if (ImGui::BeginPopup(XORSTR("options_kill")))
					{
						static int killSpammerMessageCurrent = -1;
						static char killSpammerMessageBuf[126];

						ImGui::PushItemWidth(445);
						ImGui::InputText(XORSTR("###SPAMMERMESSAGE"), killSpammerMessageBuf, IM_ARRAYSIZE(killSpammerMessageBuf));
						ImGui::PopItemWidth();
						ImGui::SameLine();

						if (ImGui::Button(XORSTR("Add")))
						{
							if (strlen(killSpammerMessageBuf) > 0)
								Settings::Spammer::KillSpammer::messages.push_back(std::string(killSpammerMessageBuf));

							strcpy(killSpammerMessageBuf, "");
						}
						ImGui::SameLine();

						if (ImGui::Button(XORSTR("Remove")))
							if (killSpammerMessageCurrent > -1 && (int)Settings::Spammer::KillSpammer::messages.size() > killSpammerMessageCurrent)
								Settings::Spammer::KillSpammer::messages.erase(Settings::Spammer::KillSpammer::messages.begin() + killSpammerMessageCurrent);

						ImGui::PushItemWidth(550);
						ImGui::ListBox("", &killSpammerMessageCurrent, Settings::Spammer::KillSpammer::messages, 10);
						ImGui::PopItemWidth();

						ImGui::EndPopup();
					}
				}

				ImGui::Columns(1);
				ImGui::Checkbox(XORSTR("Radio Commands"), &Settings::Spammer::RadioSpammer::enabled);

				ImGui::Columns(Settings::Spammer::type != SpammerType::SPAMMER_NONE ? 3 : 2, nullptr, false);
				{
					ImGui::PushItemWidth(-1);
					ImGui::Combo(XORSTR("###SPAMMERYPE"), (int *)&Settings::Spammer::type, spammerTypes, IM_ARRAYSIZE(spammerTypes));
					ImGui::PopItemWidth();
				}
				ImGui::NextColumn();
				{
					ImGui::Checkbox(XORSTR("Team Chat###SAY_TEAM2"), &Settings::Spammer::say_team);
				}
				if (Settings::Spammer::type != SpammerType::SPAMMER_NONE)
				{
					ImGui::NextColumn();
					{
						if (ImGui::Button(XORSTR("Options###SPAMMER"), ImVec2(-1, 0)))
							ImGui::OpenPopup(XORSTR("options_spammer"));

						if (Settings::Spammer::type == SpammerType::SPAMMER_NORMAL)
							ImGui::SetNextWindowSize(ImVec2(565, 268), ImGuiCond_Always);
						else if (Settings::Spammer::type == SpammerType::SPAMMER_POSITIONS)
							ImGui::SetNextWindowSize(ImVec2(200, 240), ImGuiCond_Always);

						if (ImGui::BeginPopup(XORSTR("options_spammer")))
						{
							if (Settings::Spammer::type == SpammerType::SPAMMER_NORMAL)
							{
								static int spammerMessageCurrent = -1;
								static char spammerMessageBuf[126];

								ImGui::PushItemWidth(445);
								ImGui::InputText(XORSTR("###SPAMMERMESSAGE"), spammerMessageBuf, IM_ARRAYSIZE(spammerMessageBuf));
								ImGui::PopItemWidth();
								ImGui::SameLine();

								if (ImGui::Button(XORSTR("Add")))
								{
									if (strlen(spammerMessageBuf) > 0)
										Settings::Spammer::NormalSpammer::messages.push_back(std::string(spammerMessageBuf));

									strcpy(spammerMessageBuf, "");
								}
								ImGui::SameLine();

								if (ImGui::Button(XORSTR("Remove")))
									if (spammerMessageCurrent > -1 && (int)Settings::Spammer::NormalSpammer::messages.size() > spammerMessageCurrent)
										Settings::Spammer::NormalSpammer::messages.erase(Settings::Spammer::NormalSpammer::messages.begin() + spammerMessageCurrent);

								ImGui::PushItemWidth(550);
								ImGui::ListBox("", &spammerMessageCurrent, Settings::Spammer::NormalSpammer::messages, 10);
								ImGui::PopItemWidth();
							}
							else if (Settings::Spammer::type == SpammerType::SPAMMER_POSITIONS)
							{
								ImGui::PushItemWidth(185);
								ImGui::Combo(XORSTR("###POSITIONSTEAM"), &Settings::Spammer::PositionSpammer::team, teams, IM_ARRAYSIZE(teams));
								ImGui::PopItemWidth();
								ImGui::Separator();
								ImGui::Checkbox(XORSTR("Show Name"), &Settings::Spammer::PositionSpammer::showName);
								ImGui::Checkbox(XORSTR("Show Weapon"), &Settings::Spammer::PositionSpammer::showWeapon);
								ImGui::Checkbox(XORSTR("Show Rank"), &Settings::Spammer::PositionSpammer::showRank);
								ImGui::Checkbox(XORSTR("Show Wins"), &Settings::Spammer::PositionSpammer::showWins);
								ImGui::Checkbox(XORSTR("Show Health"), &Settings::Spammer::PositionSpammer::showHealth);
								ImGui::Checkbox(XORSTR("Show Money"), &Settings::Spammer::PositionSpammer::showMoney);
								ImGui::Checkbox(XORSTR("Show Last Place"), &Settings::Spammer::PositionSpammer::showLastplace);
							}
							ImGui::EndPopup();
						}
					}
				}
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text(XORSTR("FOV"));
				ImGui::Separator();
				ImGui::Columns(2, nullptr, false);
				{
					ImGui::Checkbox(XORSTR("FOV"), &Settings::FOVChanger::enabled);
					ImGui::Checkbox(XORSTR("Viewmodel FOV"), &Settings::FOVChanger::viewmodelEnabled);
					ImGui::Checkbox(XORSTR("Ignore Scope"), &Settings::FOVChanger::ignoreScope);
				}
				ImGui::NextColumn();
				{
					ImGui::PushItemWidth(-1);
					ImGui::SliderFloat(XORSTR("##FOVAMOUNT"), &Settings::FOVChanger::value, 0, 180);
					ImGui::SliderFloat(XORSTR("##MODELFOVAMOUNT"), &Settings::FOVChanger::viewmodelValue, 0, 360);
					ImGui::PopItemWidth();
				}
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Checkbox(XORSTR("Enable Third Person"), &Settings::ThirdPerson::enabled);
				// ImGui::SameLine(0.0f, -1.0f);{ImGui::Text(XORSTR("Third Person"));}
				if (Settings::ThirdPerson::enabled)
				{
					ImGui::Separator();
					ImGui::Columns(1, nullptr, false);
					{
						ImGui::PushItemWidth(-1);
						ImGui::SliderFloat(XORSTR("Camera Offset"), &Settings::ThirdPerson::distance, 0.f, 500.f, XORSTR("Camera Offset: %0.f"));
						UI::KeyBindButton(&Settings::ThirdPerson::toggleThirdPerson);
						ImGui::PopItemWidth();
					}
				}

				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text(XORSTR("Grenade Helper"));
				ImGui::Separator();
				ImGui::Columns(2, nullptr, false);
				{
					ImGui::Checkbox(XORSTR("Enabled ###ghenabled"), &Settings::GrenadeHelper::enabled);
					if (ImGui::Button(XORSTR("Aimassist"), ImVec2(-1, 0)))
						ImGui::OpenPopup(XORSTR("optionAimAssist"));
					ImGui::SetNextWindowSize(ImVec2(200, 120), ImGuiCond_Always);
					if (ImGui::BeginPopup(XORSTR("optionAimAssist")))
					{
						ImGui::PushItemWidth(-1);
						ImGui::Checkbox(XORSTR("Enabled"), &Settings::GrenadeHelper::aimAssist);
						ImGui::SliderFloat(XORSTR("###aimstep"), &Settings::GrenadeHelper::aimStep, 0, 10, "Speed: %0.3f");
						ImGui::SliderFloat(XORSTR("###aimfov"), &Settings::GrenadeHelper::aimFov, 0, 180, "Fov: %0.2f");
						ImGui::SliderFloat(XORSTR("###aimdistance"), &Settings::GrenadeHelper::aimDistance, 0, 100, "Distance: %0.2f");
						ImGui::PopItemWidth();
						ImGui::EndPopup();
					}
				}
				ImGui::NextColumn();
				{
					ImGui::Checkbox(XORSTR("Only matching ###match"), &Settings::GrenadeHelper::onlyMatchingInfos);
					if (ImGui::Button(XORSTR("Add Info"), ImVec2(-1, 0)))
						ImGui::OpenPopup(XORSTR("addinfo_throw"));

					ImGui::SetNextWindowSize(ImVec2(565, 268), ImGuiCond_Always);
					if (ImGui::BeginPopup(XORSTR("addinfo_throw")))
					{
						static int throwMessageCurrent = -1;
						static char inputName[40];
						static int tType = (int)ThrowType::NORMAL;
						static int gType = (int)GrenadeType::SMOKE;

						ImGui::Columns(1);
						ImGui::PushItemWidth(500);
						ImGui::InputText("", inputName, sizeof(inputName));
						ImGui::PopItemWidth();
						ImGui::SameLine();
						if (ImGui::Button(XORSTR("Add")) && engine->IsInGame() && Settings::GrenadeHelper::actMapName.length() > 0)
						{
							C_BasePlayer *localPlayer = (C_BasePlayer *)entityList->GetClientEntity(engine->GetLocalPlayer());
							if (strlen(inputName) > 0)
							{
								GrenadeInfo n = GrenadeInfo((GrenadeType)gType, localPlayer->GetEyePosition(), *localPlayer->GetVAngles(), (ThrowType)tType, inputName);
								Settings::GrenadeHelper::grenadeInfos.push_back(n);
								std::ostringstream path;
								path << GetGhConfigDirectory() << Settings::GrenadeHelper::actMapName;
								if (!DoesFileExist(path.str().c_str()))
									mkdir(path.str().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
								path << XORSTR("/config.json");
								Settings::SaveGrenadeInfo(path.str());
							}
							strcpy(inputName, "");
						}
						ImGui::Columns(2);
						ImGui::Combo(XORSTR("###Throwtype"), &tType, throwTypes, IM_ARRAYSIZE(throwTypes));
						ImGui::NextColumn();
						ImGui::Combo(XORSTR("###Grenadetype"), &gType, grenadeTypes, IM_ARRAYSIZE(grenadeTypes));
						ImGui::Columns(1);
						ImGui::Separator();
						ImGui::PushItemWidth(550);
						auto lambda = [](void *data, int idx, const char **out_text)
						{
							*out_text = Settings::GrenadeHelper::grenadeInfos.at(idx).name.c_str();
							return *out_text != nullptr;
						};
						ImGui::ListBox("", &throwMessageCurrent, lambda, nullptr, Settings::GrenadeHelper::grenadeInfos.size(), 7);
						ImGui::PopItemWidth();
						ImGui::Columns(1);
						if (ImGui::Button(XORSTR("Remove"), ImVec2(ImGui::GetWindowWidth(), 30)))
							if (throwMessageCurrent > -1 && (int)Settings::GrenadeHelper::grenadeInfos.size() > throwMessageCurrent)
							{
								Settings::GrenadeHelper::grenadeInfos.erase(Settings::GrenadeHelper::grenadeInfos.begin() + throwMessageCurrent);
								std::ostringstream path;
								path << GetGhConfigDirectory() << Settings::GrenadeHelper::actMapName;
								if (!DoesFileExist(path.str().c_str()))
									mkdir(path.str().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
								path << XORSTR("/config.json");
								Settings::SaveGrenadeInfo(path.str());
							}
						ImGui::EndPopup();
					}
				}
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Checkbox(XORSTR("Force cvars"), &Settings::CVarsOverride::enabled);
				if (Settings::CVarsOverride::enabled)
				{
					ImGui::Checkbox(XORSTR("Ragdoll gravity"), &Settings::CVarsOverride::ragdoll);
					ImGui::Checkbox(XORSTR("Fake Ping"), &Settings::CVarsOverride::fakeLatency);
					ImGui::Checkbox(XORSTR("Fullbright"), &Settings::CVarsOverride::fullbright);
					ImGui::Checkbox(XORSTR("Impacts"), &Settings::CVarsOverride::showImpacts);
				}

				ImGui::Separator();
				ImGui::Checkbox(XORSTR("AutoBuy"), &Settings::AutoBuy::enabled);
				if (Settings::AutoBuy::enabled)
				{
					ImGui::Combo(XORSTR("###MAINWAEP"), (int *)&Settings::AutoBuy::main, mainWeapons, IM_ARRAYSIZE(mainWeapons));
					ImGui::Combo(XORSTR("###SECWAEP"), (int *)&Settings::AutoBuy::secondary, secondaryWeapons, IM_ARRAYSIZE(secondaryWeapons));
					if (ImGui::BeginCombo(XORSTR("##FilterOptions"), XORSTR("Other")))
					{
						ImGui::Selectable(XORSTR("Grenades"), &Settings::AutoBuy::nades, ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable(XORSTR("Armor"), &Settings::AutoBuy::armor, ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable(XORSTR("Taser"), &Settings::AutoBuy::taser, ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable(XORSTR("Defuser"), &Settings::AutoBuy::defuser, ImGuiSelectableFlags_DontClosePopups);
						ImGui::EndCombo();
					}
				}
				ImGui::Separator();
				if (ImGui::Button(XORSTR("Configs")))
					Configs::showWindow = !Configs::showWindow;

				ImGui::EndChild();
			}
		}
		ImGui::NextColumn();
		{
			ImGui::BeginChild(XORSTR("Child2"), ImVec2(0, 781), true);
			{
				ImGui::Separator();
				ImGui::Text(XORSTR("Clantag"));
				ImGui::Separator();
				ImGui::Columns(2, nullptr, false);
				{
					ImGui::PushItemWidth(-1);
					ImGui::Checkbox(XORSTR("Enabled"), &Settings::ClanTagChanger::enabled);
					ImGui::InputText(XORSTR("##CLANTAG"), Settings::ClanTagChanger::value, 30);
					ImGui::PopItemWidth();

					// ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
					// ImGui::Text(XORSTR("Animation Delay(ms)"));
				}
				ImGui::NextColumn();
				{
					ImGui::PushItemWidth(-1);
					if (ImGui::Combo(XORSTR("##ANIMATIONTYPE"), (int *)&Settings::ClanTagChanger::type, animationTypes, IM_ARRAYSIZE(animationTypes)))
						ClanTagChanger::UpdateClanTagCallback();
					if (ImGui::Button(XORSTR("Update Clantag"), ImVec2(-1, 0)))
						ClanTagChanger::UpdateClanTagCallback();
					ImGui::PopItemWidth();
				}
				ImGui::Columns(1);
				ImGui::PushItemWidth(-1);
				if (ImGui::SliderInt(XORSTR("##ANIMATIONSPEED"), &Settings::ClanTagChanger::animationSpeed, 500, 2000, XORSTR("Animation Speed(ms): %0.f")))
					ClanTagChanger::UpdateClanTagCallback();
				ImGui::PopItemWidth();
				ImGui::Separator();
				ImGui::Text(XORSTR("Nickname"));
				ImGui::Separator();

				ImGui::InputText(XORSTR("##NICKNAME"), nickname, 127);

				ImGui::SameLine();
				if (ImGui::Button(XORSTR("Set Nickname"), ImVec2(-1, 0)))
					NameChanger::changeName(false, std::string(nickname).c_str(), 5.0f);

				if (ImGui::Button(XORSTR("Glitch Name")))
					NameChanger::SetName("\n\xAD\xAD\xAD");
				ImGui::SameLine();
				if (ImGui::Button(XORSTR("No Name")))
				{
					NameChanger::changes = 0;
					NameChanger::type = NameChanger::NC_Type::NC_NORMAL;
				}

				ImGui::SameLine();
				if (ImGui::Button(XORSTR("Rainbow Name")))
					NameChanger::InitColorChange(NameChanger::NC_Type::NC_RAINBOW);

				ImGui::SameLine();
				if (ImGui::Button(XORSTR("Colorize Name"), ImVec2(-1, 0)))
					ImGui::OpenPopup(XORSTR("optionColorizeName"));
				ImGui::SetNextWindowSize(ImVec2(150, 300), ImGuiCond_Always);
				if (ImGui::BeginPopup(XORSTR("optionColorizeName")))
				{
					ImGui::PushItemWidth(-1);
					for (auto &it : NameChanger::colors)
					{
						if (ImGui::Button(it.second, ImVec2(-1, 0)))
							NameChanger::InitColorChange(NameChanger::NC_Type::NC_SOLID, it.first);
					}
					ImGui::PopItemWidth();

					ImGui::EndPopup();
				}
				if (ImGui::Button(XORSTR("Set Banned-Name"), ImVec2(-1, 0)))
				{
					std::string banText{nickname};
					banText += " has been permanently banned from official CS:GO servers.";
					//	NameChanger::SetName(std::string("\x1\xB7").append(std::string(nickname)).append(banText).c_str());
					std::string res = " \x01\x0B";
					res += (char)(NameChanger::Colors::LIGHT_RED);
					res.append(banText);
					res.append("\230");

					//	    NameChanger::changeName(false, std::string{ "\x1\xB" }.append(std::string{ static_cast<char>(7) }).append(banText).append("\x1").c_str(), 5.0f);
					NameChanger::changeName(false, res.c_str(), 5.0f);
				}
				ImGui::Columns(2, nullptr, false);
				{
					ImGui::Checkbox(XORSTR("Name Stealer"), &Settings::NameStealer::enabled);
				}
				ImGui::NextColumn();
				{
					ImGui::Combo("", &Settings::NameStealer::team, teams, IM_ARRAYSIZE(teams));
				}
				ImGui::EndColumns();

				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text(XORSTR("Other"));
				ImGui::Separator();
				ImGui::Columns(2, nullptr, false);
				{
					ImGui::Checkbox(XORSTR("AirStuck"), &Settings::Airstuck::enabled);
					ImGui::Checkbox(XORSTR("Autoblock"), &Settings::Autoblock::enabled);
					ImGui::Checkbox(XORSTR("Jump Throw"), &Settings::JumpThrow::enabled);
					ImGui::Checkbox(XORSTR("Auto Accept"), &Settings::AutoAccept::enabled);
					ImGui::Checkbox(XORSTR("Auto Defuse"), &Settings::AutoDefuse::enabled);
					ImGui::Checkbox(XORSTR("Sniper Crosshair"), &Settings::SniperCrosshair::enabled);
					ImGui::Checkbox(XORSTR("No Duck Cooldown"), &Settings::NoDuckCooldown::enabled);
					ImGui::Checkbox(XORSTR("Fast Walk"), &Settings::SilentWalk::enabled);
					ImGui::Checkbox(XORSTR("Disable setting cvars"), &Settings::DisableSettingCvars::enabled);
					ImGui::Checkbox(XORSTR("Disable post-processing"), &Settings::DisablePostProcessing::enabled);
				}
				ImGui::NextColumn();
				{
					UI::KeyBindButton(&Settings::Airstuck::key);
					UI::KeyBindButton(&Settings::Autoblock::key);
					UI::KeyBindButton(&Settings::JumpThrow::key);
					ImGui::Checkbox(XORSTR("Show Ranks"), &Settings::ShowRanks::enabled);
					ImGui::Checkbox(XORSTR("Show Votes"), &Settings::VoteRevealer::enabled);
					ImGui::Checkbox(XORSTR("Attempt NoFall"), &Settings::NoFall::enabled);
					ImGui::Checkbox(XORSTR("Show Spectator list"), &Settings::ShowSpectators::enabled);
					ImGui::Checkbox(XORSTR("AWP Quick Switch"), &Settings::QuickSwitch::enabled);
					ImGui::Checkbox(XORSTR("Disable Anti Untrusted"), &ValveDSCheck::forceUT);
					ImGui::Checkbox(XORSTR("Force rare animations"), &Settings::Skinchanger::forcerareanim);
				}
				ImGui::Columns(1);
				ImGui::Separator();
				if (ImGui::Button(XORSTR("Unload")))
				{
					Kali::SelfShutdown();
				}
				if (ImGui::Button(XORSTR("Reset resolver misses")))
				{
					C_BasePlayer *localplayer = (C_BasePlayer *)entityList->GetClientEntity(engine->GetLocalPlayer());
					int maxClient = engine->GetMaxClients();
					for (int i = 1; i < maxClient; ++i)
					{
						// indx = i;
						C_BasePlayer *player = (C_BasePlayer *)entityList->GetClientEntity(i);

						if (!player || player == localplayer || Entity::IsTeamMate(player, localplayer))
							continue;
						Resolver::players[player->GetIndex()].MissedCount = 0;
					}
				}
				ImGui::Separator();
				ImGui::Text(XORSTR("Profile Changer"));
				ImGui::Separator();
				ImGui::Columns(1, nullptr, false);
				{
					ImGui::PushItemWidth(-1);
					ImGui::Combo(XORSTR("##MUSICTYPE"), (int *)&Settings::ProfileChanger::type, musicType, IM_ARRAYSIZE(musicType));
					ImGui::InputInt(XORSTR("COIN##ID"), &Settings::ProfileChanger::coinID);
					ImGui::InputInt(XORSTR("COMP RANK##ID"), &Settings::ProfileChanger::compRank);
					ImGui::PopItemWidth();
					if (ImGui::Button(XORSTR("Update profile"), ImVec2(-1, 0)))
						ProfileChanger::UpdateProfile();
				}
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(210, 85));
				if (ImGui::BeginPopupModal(XORSTR("Error###UNTRUSTED_FEATURE")))
				{
					ImGui::Text(XORSTR("You cannot use this feature on a VALVE server."));

					ImGui::Checkbox(XORSTR("This is not a VALVE server"), &ValveDSCheck::forceUT);

					if (ImGui::Button(XORSTR("OK")))
						ImGui::CloseCurrentPopup();

					ImGui::EndPopup();
				}
				ImGui::PopStyleVar();
				ImGui::EndChild();
			}
		}
	}
	ImGui::EndGroup();
}