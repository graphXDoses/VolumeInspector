#pragma once

#include <imgui_internal.h>

namespace ImGui {

	namespace VolumeEngineUI
	{

		//ImU32 GetHeatmapColor(float value);

		void IntensityLegend(const char* str_id, bool* colorSwitch);

		void ColorMapSwitch(bool* mappingMode);

	}

}