#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "MessageHandler.h"
#include "PacketReader.hpp"
#include "Utils.hpp"

//void OnRegionRegionDynamicSubscribe(PacketReader &reader);
//void OnRegionRegionDynamicPlayback(PacketReader &reader);
//void OnRegionRegionMasterFrameSync(PacketReader &reader);
//void OnRegionRegionAgentControllerMapping(PacketReader &reader);

class RegionRegion : public MessageHandler
{
public:
    bool OnMessage(uint32_t messageId, PacketReader &reader)
    {
        switch (messageId)
        {
            case RegionRegionMessages::DynamicSubscribe: // 1BB19B0
            {
                RegionRegion::OnDynamicSubscribe(reader);
                break;
            }
            case RegionRegionMessages::DynamicPlayback: // 1BB1B00
            {
                RegionRegion::OnDynamicPlayback(reader);
                break;
            }
            case RegionRegionMessages::MasterFrameSync: // 1BB1C50
            {
                RegionRegion::OnMasterFrameSync(reader);
                break;
            }
            case RegionRegionMessages::AgentControllerMapping: // 1BB1E10
            {
                RegionRegion::OnAgentControllerMapping(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    static void OnDynamicSubscribe(PacketReader &reader)  // 1BB19B0
    {
        // No payload
    }

    static void OnDynamicPlayback(PacketReader &reader) // 1BB1B00
    {
        // No payload
    }

    static void OnMasterFrameSync(PacketReader &reader) // 1BB1C50
    {
        auto masterFrame = reader.ReadUint64();
    }

    static void OnAgentControllerMapping(PacketReader &reader)  // 1BB1E10
    {
        auto agentControllerId = reader.ReadUint32();
        auto animationComponentId = reader.ReadUint64();
        auto clusterId = reader.ReadUint32();
    }
};
