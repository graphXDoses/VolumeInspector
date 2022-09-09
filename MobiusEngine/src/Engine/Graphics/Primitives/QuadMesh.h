#pragma once

namespace MobiusEngine
{

	namespace Meshes
	{

        float quadVertices[4*5] = {
            // pos(vec3) | uv(vec2)
            -0.500, -0.500, 0.000, 0.000, 0.000,
            0.500, -0.500, 0.000, 1.000, 0.000,
            0.500, 0.500, 0.000, 1.000, 1.000,
            -0.500, 0.500, 0.000, 0.000, 1.000
        };

        uint32_t quadIndices[6] = {
            0,1,2,2,3,0
        };

	}

}
