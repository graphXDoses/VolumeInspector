#include "ngnpch.h"
#include "CustomWidgets.h"

#include <format>

constexpr int NUM_OF_LEGEND_LVLS = 5;

namespace ImGui
{
	namespace VolumeEngineUI
	{
		static const char* map[4] = { "GRAYSCALE", "HEATMAP", "Grayscale", "Heatmap" };
		static const char* layout[2] = { "EXTENDED", "BOUNDARY" };

		void IntensityLegend(const char* str_id, bool* colorSwitch)
		{
			ImVec2 p1 = ImGui::GetCursorScreenPos();
			ImVec2 pWinMin = ImGui::GetWindowContentRegionMin();
			//ImVec2 pWinMax = ImGui::GetWindowContentRegionMax();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			ImVec2 wSize = ImGui::GetWindowSize();

			ImGui::SetCursorScreenPos(ImVec2( p1.x + std::max(pWinMin.x, 10.0f), p1.y - (wSize.y - (pWinMin.y + pWinMin.x) )));
			ImVec2 p2 = ImGui::GetCursorScreenPos();

			float width = ImGui::GetFrameHeightWithSpacing() * 4.0f;
			float height = (width * 8.0f) / 4.0f;

			float TLH = ImGui::GetTextLineHeight(), TLW = TLH / 2.0;
			ImGui::GetFrameHeight();
			p2.y += TLH * 5.0f;

#if 1
			ImGui::BeginColumns("LegendDescriptionCols", 2, ImGuiOldColumnFlags_NoBorder);
			float colWidth = ImGui::GetColumnWidth();
			ImGui::Text("Describing: ");
			ImGui::Text("Mapping: ");
			ImGui::Text("Layout: ");
			ImGui::SetColumnWidth(0, TLW * 13.0f);
			ImGui::NextColumn();
			ImGui::Text("Intensity levels");
			ImGui::Text("%s", map[*colorSwitch]);
			ImGui::Text("%s", layout[0]);
			ImGui::SetColumnWidth(1, TLW * 17.0f);
			ImGui::EndColumns();
#else
			ImGui::Text("Describing:  Intensity levels\nMapping: \t\%s\nLayout:  \tNONE", map[*colorSwitch]);
#endif
			
			ImGui::InvisibleButton(str_id, ImVec2(width, height));

			// Grayscale colors
			ImU32 white = ImGui::GetColorU32(ImVec4({ 1.0f, 1.0f, 1.0f, 1.0f }));
			ImU32 black = ImGui::GetColorU32(ImVec4({ 0.0f, 0.0f, 0.0f, 1.0f }));

			if (!*colorSwitch)
			{
				draw_list->AddRectFilledMultiColor(ImVec2(p2.x + width * 0.75f, p2.y), ImVec2(p2.x + width, p2.y + height), white, white, black, black);
			}
			else {

				draw_list->AddRectFilledMultiColor(ImVec2(p2.x + width * 0.75f, p2.y + (height * 1.00f) ), ImVec2(p2.x + width, p2.y + (height * 0.75f) ),
					ImU32(0xFFFF0000), ImU32(0xFFFF0000), ImU32(0xFFFFFF00), ImU32(0xFFFFFF00)); // Blue   | Cyan
				draw_list->AddRectFilledMultiColor(ImVec2(p2.x + width * 0.75f, p2.y + (height * 0.75f) ), ImVec2(p2.x + width, p2.y + (height * 0.50f) ),
					ImU32(0xFFFFFF00), ImU32(0xFFFFFF00), ImU32(0xFF00FF00), ImU32(0xFF00FF00)); // Cyan   | Green
				draw_list->AddRectFilledMultiColor(ImVec2(p2.x + width * 0.75f, p2.y + (height * 0.50f)), ImVec2(p2.x + width, p2.y + (height * 0.25f)),
					ImU32(0xFF00FF00), ImU32(0xFF00FF00), ImU32(0xFF00FFFF), ImU32(0xFF00FFFF)); // Green  | Yellow
				draw_list->AddRectFilledMultiColor(ImVec2(p2.x + width * 0.75f, p2.y + (height * 0.25f)), ImVec2(p2.x + width, p2.y + (height * 0.00f)),
					ImU32(0xFF00FFFF), ImU32(0xFF00FFFF), ImU32(0xFF0000FF), ImU32(0xFF0000FF)); // Yellow | Red
			}
			draw_list->AddRect(ImVec2(p2.x + width * 0.75f, p2.y - .5f), ImVec2(p2.x + width, p2.y + 1.0f + height), white, 0, 1.0f);

			char buff[8];
			for(int i = 0, perc = 100; i< NUM_OF_LEGEND_LVLS; i++, perc -= 100 / (NUM_OF_LEGEND_LVLS-1))
			{
				snprintf(buff, sizeof(buff), "%.2f%%", (float)perc);
				draw_list->AddLine(ImVec2(p2.x + (width * 0.75f)- TLW, p2.y + (i * (height / ( NUM_OF_LEGEND_LVLS-1 ) ) ) ),
					ImVec2(p2.x + (width * 0.75f), p2.y + (i * (height/ (NUM_OF_LEGEND_LVLS-1) ) ) ), white, 1.5f);
				draw_list->AddText(ImVec2(p2.x, p2.y + (i * (height / (NUM_OF_LEGEND_LVLS - 1))) - ImGui::GetTextLineHeight() * 0.5f), white, buff);
			}
		}
		

		void ColorMapSwitch(bool* mappingMode)
		{
			//ImGuiStyle& style = ImGui::GetStyle();
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

			float sz = ImGui::GetWindowContentRegionWidth() / 2.0f;

			ImGui::PushID("ColorMapSwitch_ColumnSet");

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, sz);

			if (*mappingMode == false)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_SliderGrab));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_CheckMark));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
			}

			if (ImGui::Button(map[2], { sz, 24.0f }))
				*mappingMode = false;

			if (ImGui::IsItemHovered())
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

			ImGui::PopStyleColor(2);
			ImGui::NextColumn();

			if (*mappingMode == true)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_SliderGrab));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_CheckMark));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
			}

			if (ImGui::Button(map[3], { sz, 24.0f }))
				*mappingMode = true;

			if (ImGui::IsItemHovered())
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

			ImGui::PopStyleColor(2);

			ImGui::PopID();
			ImGui::PopStyleVar();
		}

	}

}