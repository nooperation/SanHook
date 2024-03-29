#pragma once


#include <cstdio>
#include <string>
#include <vector>

#include "MessageHandler.h"
#include "PacketReader.hpp"
#include "Utils.hpp"

// void OnRenderLightStateChanged(PacketReader &reader);

class Render : public MessageHandler
{
public:
    bool OnMessage(uint32_t messageId, PacketReader &reader, bool isSending)
    {
        this->_isSender = isSending; // todo: get rid of this garbage

        switch (messageId)
        {
            case RenderMessages::LightStateChanged: // TAG: 1611170
            {
               // this->OnLightStateChanged(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    void OnLightStateChanged(PacketReader &reader)  // TAG: 1611170
    {
        auto componentId = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto rgb = reader.ReadVectorF(3);
        auto range = reader.ReadFloat();
        auto shadowPriority = reader.ReadUint32();
        auto spotSinHalfAngle = reader.ReadFloat();
        auto spotAngularFalloff = reader.ReadFloat();
        auto spotNearClip = reader.ReadFloat();

        //printf("RenderMessages::LightStateChanged:\n  componentId = %llu\n  frame = %llu\n  rgb = <%f, %f, %f>\n  range = %f\n  shadowPriority = %u\n  spotSinHalfAngle = %f\n  spotAngularFalloff = %f\n  spotNearClip = %f\n",
        //    componentId,
        //    frame,
        //    rgb[0], rgb[1], rgb[2],
        //    range,
        //    shadowPriority,
        //    spotSinHalfAngle,
        //    spotAngularFalloff,
        //    spotNearClip
        //);
    }
};
