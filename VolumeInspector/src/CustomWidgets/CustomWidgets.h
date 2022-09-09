#pragma once

#include "imgui/imgui_internal.h"

namespace MobiusEngine {

	namespace MobiusUI
	{

		//ImU32 GetHeatmapColor(float value);

		void IntensityLegend(const char* str_id, bool* colorSwitch);

		void ColorMapSwitch(bool* mappingMode);

	}

}