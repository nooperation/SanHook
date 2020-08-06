#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "PacketReader.hpp"
#include "Utils.hpp"

//void OnRegionRegionDynamicSubscribe(PacketReader &reader);
//void OnRegionRegionDynamicPlayback(PacketReader &reader);
//void OnRegionRegionMasterFrameSync(PacketReader &reader);
//void OnRegionRegionAgentControllerMapping(PacketReader &reader);

class RegionRegion
{
public:
    static void OnDynamicSubscribe(PacketReader &reader)
    {
        // No payload
    }

    static void OnDynamicPlayback(PacketReader &reader)
    {
        // No payload
    }

    static void OnMasterFrameSync(PacketReader &reader)
    {
        auto masterFrame = reader.ReadUint64();
    }

    static void OnAgentControllerMapping(PacketReader &reader)
    {
        auto agentControllerId = reader.ReadUint32();
        auto animationComponentId = reader.ReadUint64();
        auto clusterId = reader.ReadUint32();
    }
};
