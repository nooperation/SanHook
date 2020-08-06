#pragma once


#include <cstdio>
#include <string>
#include <vector>

#include "PacketReader.hpp"
#include "Utils.hpp"

// void OnRenderLightStateChanged(PacketReader &reader);

class Render
{
public:
    static void OnLightStateChanged(PacketReader &reader)  // 1611170
    {
        auto componentId = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto rgb = reader.ReadVectorF(3);
        auto range = reader.ReadFloat();
        auto shadowPriority = reader.ReadUint32();
        auto spotSinHalfAngle = reader.ReadFloat();
        auto spotAngularFalloff = reader.ReadFloat();
        auto spotNearClip = reader.ReadFloat();

        printf("RenderMessages::OnLightStateChanged:\n  componentId = %llu\n  frame = %llu\n  rgb = <%f, %f, %f>\n  range = %f\n  shadowPriority = %u\n  spotSinHalfAngle = %f\n  spotAngularFalloff = %f\n  spotNearClip = %f\n",
            componentId,
            frame,
            rgb[0], rgb[1], rgb[2],
            range,
            shadowPriority,
            spotSinHalfAngle,
            spotAngularFalloff,
            spotNearClip
        );
    }
};
