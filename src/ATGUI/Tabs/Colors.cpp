#include "Colors.hpp"

//enum
//{
	//ESP,
	//Chams,
	//Grenade,
	//Others,
//};

static void colorPicker()
{
	const char *colorNames[IM_ARRAYSIZE(Colors::colors)];
	int size = IM_ARRAYSIZE(Colors::colors);
	for (int i = 0; i < size; i++) {
		colorNames[i] = Colors::colors[i].name;
	}

	static int colorSelected = 0;

	ImGui::Columns(2, nullptr, false);
	{
		ImGui::PushItemWidth(-1);
		ImGui::ListBoxHeader(XORSTR("##ColorNames"), ImVec2(-1, -1));
		for (int i = 0; i < size; i++) {
			if (ImGui::Selectable(colorNames[i])) {
				colorSelected = i;
			}
		}
		// ImGui::ListBox(XORSTR("##COLORSELECTION"), &colorSelected, colorNames, IM_ARRAYSIZE(colorNames), size);
		ImGui::ListBoxFooter();
		ImGui::PopItemWidth();
	}
	ImGui::NextColumn();
	{
		ImGui::PushItemWidth(-1);
		if ((int) Colors::colors[colorSelected].type == (int) Colors::ColorListVar::HEALTHCOLORVAR_TYPE) {
			UI::ColorPicker4((float *) Colors::colors[colorSelected].healthColorVarPtr);
			ImGui::CheckboxFill(XORSTR("Rainbow"), &Colors::colors[colorSelected].healthColorVarPtr->rainbow);
			ImGui::SameLine();
			ImGui::CheckboxFill(XORSTR("Health-Based"), &Colors::colors[colorSelected].healthColorVarPtr->hp);
			ImGui::PushItemWidth(-1);
			ImGui::SliderFloat(XORSTR("##RAINBOWSPEED"), &Colors::colors[colorSelected].healthColorVarPtr->rainbowSpeed,
			                   0.f, 1.f, "Rainbow Speed: %0.3f");
			ImGui::PopItemWidth();
		} else {

			UI::ColorPicker4((float *) Colors::colors[colorSelected].colorVarPtr);
			ImGui::CheckboxFill(XORSTR("Rainbow"), &Colors::colors[colorSelected].colorVarPtr->rainbow);
			ImGui::SliderFloat(XORSTR("##RAINBOWSPEED"), &Colors::colors[colorSelected].colorVarPtr->rainbowSpeed, 0.f,
			                   1.f, "Rainbow Speed: %0.3f");

		}
		ImGui::PopItemWidth();
	}
	// Colors::RenderWindow();
}

//static void AimwarecolorPicker()
//{
	//const char *colorNames[IM_ARRAYSIZE(AWcolor::colors)];
	//int size = IM_ARRAYSIZE(AWcolor::colors);
	//for (int i = 0; i < size; i++) {
	//	colorNames[i] = AWcolor::colors[i].name; // ColorListVar colors [] 
	//}

	//static int colorSelected = 0;

	//ImGui::Columns(2, nullptr, false);
	//{
		//ImGui::PushItemWidth(-1);
		//ImGui::ListBoxHeader(XORSTR("##ColorNames"), ImVec2(-1, -1));
		//for (int i = 0; i < size; i++) {
			//if (ImGui::Selectable(colorNames[i])) {
			//	colorSelected = i;
			//}
		//}
		//ImGui::ListBox(XORSTR("##COLORSELECTION"), &colorSelected, colorNames, IM_ARRAYSIZE(colorNames), size);
		//ImGui::ListBoxFooter();
		//ImGui::PopItemWidth();
	//}
	//ImGui::NextColumn();
	//{
		//ImGui::PushItemWidth(-1);
		//if ((int) AWcolor::colors[colorSelected].type == (int) AWcolor::ColorListVar::HEALTHCOLORVAR_TYPE) {
			//UI::ColorPicker4((float *) AWcolor::colors[colorSelected].healthColorVarPtr);
			//ImGui::CheckboxFill(XORSTR("Rainbow"), &AWcolor::colors[colorSelected].healthColorVarPtr->rainbow);
			//ImGui::SameLine();
			//ImGui::CheckboxFill(XORSTR("Health-Based"), &AWcolor::colors[colorSelected].healthColorVarPtr->hp);
			//ImGui::PushItemWidth(-1);
			//ImGui::SliderFloat(XORSTR("##RAINBOWSPEED"), &AWcolor::colors[colorSelected].healthColorVarPtr->rainbowSpeed,
			                   //0.f, 1.f, "Rainbow Speed: %0.3f");
			//ImGui::PopItemWidth();
		//} else {

			//UI::ColorPicker4((float *) AWcolor::colors[colorSelected].colorVarPtr);
			//ImGui::CheckboxFill(XORSTR("Rainbow"), &AWcolor::colors[colorSelected].colorVarPtr->rainbow);
			//ImGui::SliderFloat(XORSTR("##RAINBOWSPEED"), &AWcolor::colors[colorSelected].colorVarPtr->rainbowSpeed, 0.f,
			                   //1.f, "Rainbow Speed: %0.3f");

		//}
		//ImGui::PopItemWidth();
	//}
	// Colors::RenderWindow();
//}

void Colors::RenderTab()
{
	ImGui::Columns();
	{
		ImGui::BeginChild(XORSTR("ColorPicker"));
		{
			colorPicker();
		}
		ImGui::EndChild();
	}
}

//void AWcolor::RenderAimware(ImVec2 &pos, ImDrawList *draw, int sideTabIndex)
//{
//	ImGui::SetCursorPos(ImVec2(180, 65));
//	if (sideTabIndex == ESP) {
//		ImGui::BeginGroup();
//		{
//			ImGui::Columns();
//			{
//				ImGui::BeginChild(XORSTR("##PlayerVisuals1"), ImVec2(0, 0), false);
//				{
//					AimwarecolorPicker();
//				}
//				ImGui::EndChild();
//
//			}
//
//		}
//		ImGui::EndGroup();
//
//	} else if (sideTabIndex == Chams) {
//		ImGui::BeginGroup();
//		{
//			ImGui::Columns();
//			{
//				ImGui::BeginChild(XORSTR("##PlayerVisuals1"), ImVec2(0, 0), false);
//				{
//					AimwarecolorPicker();
//				}
//				ImGui::EndChild();
//
//			}
//
//		}
//		ImGui::EndGroup();
//
//	} else if (sideTabIndex == Grenade) {
//		ImGui::BeginGroup();
//		{
//			ImGui::Columns();
//			{
//				ImGui::BeginChild(XORSTR("##PlayerVisuals1"), ImVec2(0, 0), false);
//				{
//					AimwarecolorPicker();
//				}
//				ImGui::EndChild();
//
//			}
//
//		}
//		ImGui::EndGroup();
//
//	} else if (sideTabIndex == Others) {
//		ImGui::BeginGroup();
//		{
//			ImGui::Columns();
//			{
//				ImGui::BeginChild(XORSTR("##PlayerVisuals1"), ImVec2(0, 0), false);
//				{
//					AimwarecolorPicker();
//				}
//			ImGui::EndChild();
//
//			}
//
//		}
//		ImGui::EndGroup();
//
//	} else {
//		ImGui::BeginGroup();
//		{
//			ImGui::Columns();
//			{
//				ImGui::BeginChild(XORSTR("##PlayerVisuals1"), ImVec2(0, 0), false);
//				{
//					AimwarecolorPicker();
//				}
//				ImGui::EndChild();
//
//			}
//
//		}
//		ImGui::EndGroup();
//	}
//
//}