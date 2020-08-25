#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "MessageHandler.h"
#include "PacketReader.hpp"
#include "Utils.hpp"

//void OnGameWorldMessageUpdateRuntimeInventorySettings(PacketReader &reader);
//void OnGameWorldMessageScriptCameraMessage(PacketReader &reader);
//void OnGameWorldMessageRiggedMeshScaleChanged(PacketReader &reader);
//void OnGameWorldMessageRiggedMeshFlagsChange(PacketReader &reader);
//void OnGameWorldMessageStaticMeshScaleChanged(PacketReader &reader);
//void OnGameWorldMessageStaticMeshFlagsChanged(PacketReader &reader);
//void OnGameWorldMessageChangeMaterial(PacketReader &reader);
//void OnGameWorldMessageChangeMaterialFloatParam(PacketReader &reader);
//void OnGameWorldMessageChangeMaterialVectorParam(PacketReader &reader);
//void OnGameWorldMessageMoveEntity(PacketReader &reader);
//void OnGameWorldMessageTimestamp(PacketReader &reader);


class GameWorld : public MessageHandler
{
public:
    bool OnMessage(uint32_t messageId, PacketReader &reader, bool isSending)
    {
        this->_isSender = isSending; // todo: get rid of this garbage

        switch (messageId)
        {
            case GameWorldMessages::Timestamp:  // TAG: 16E9DC0
            {
                this->OnTimestamp(reader);
                break;
            }
            case GameWorldMessages::MoveEntity: // TAG: 16E9E30
            {
                this->OnMoveEntity(reader);
                break;
            }
            case GameWorldMessages::ChangeMaterialVectorParam:  // TAG: 16E9EA0
            {
                this->OnChangeMaterialVectorParam(reader);
                break;
            }
            case GameWorldMessages::ChangeMaterialFloatParam:  // TAG: 16E9F10
            {
                this->OnChangeMaterialFloatParam(reader);
                break;
            }
            case GameWorldMessages::ChangeMaterial:  // TAG: 16E9F80
            {
                this->OnChangeMaterial(reader);
                break;
            }
            case GameWorldMessages::StaticMeshFlagsChanged:  // TAG: 16E9FF0
            {
                this->OnStaticMeshFlagsChanged(reader);
                break;
            }
            case GameWorldMessages::StaticMeshScaleChanged:  // TAG: 16EA060
            {
                this->OnStaticMeshScaleChanged(reader);
                break;
            }
            case GameWorldMessages::RiggedMeshFlagsChange:  // TAG: 16EA0D0
            {
                this->OnRiggedMeshFlagsChange(reader);
                break;
            }
            case GameWorldMessages::RiggedMeshScaleChanged:  // TAG: 16EA140
            {
                this->OnRiggedMeshScaleChanged(reader);
                break;
            }
            case GameWorldMessages::ScriptCameraMessage:  // TAG: 16EA1B0
            {
                this->OnScriptCameraMessage(reader);
                break;
            }
            case GameWorldMessages::UpdateRuntimeInventorySettings:  // TAG: 16EA220
            {
                this->OnUpdateRuntimeInventorySettings(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    void OnTimestamp(PacketReader &reader) // TAG: 16E9DC0
    {
        auto nanoseconds = reader.ReadUint64();
        auto frame = reader.ReadUint64();
    }

    void OnMoveEntity(PacketReader &reader)// TAG: 16E9E30
    {
        auto startFrame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto startPosition = reader.ReadVectorF(3);
        auto targetPosition = reader.ReadVectorF(3);
        auto time = reader.ReadUint32();
        //auto startOrientation = reader.ReadBits(46);
        //auto targetOrientation = reader.ReadBits(46);
        //auto positionInterpMode = reader.ReadBits(4);
        //auto rotationInterpMode = reader.ReadBits(4);

        //printf("GameWorldMessages::MoveEntity:\n  startFrame = %llu\n  componentId = %llu\n startPosition = <%f, %f, %f>\n  targetPosition = <%f, %f, %f>\n  time = %u\n",
        //    startFrame,
        //    componentId,
        //    startPosition[0], startPosition[1], startPosition[2],
        //    targetPosition[0], targetPosition[1], targetPosition[2],
        //    time
        //);
    }

    void OnChangeMaterialVectorParam(PacketReader &reader)// TAG: 16E9EA0
    {
        auto parameter = reader.ReadUint8();
        auto start = reader.ReadVectorF(3);
        auto end = reader.ReadVectorF(3);
    }

    void OnChangeMaterialFloatParam(PacketReader &reader)   // TAG: 16E9F10
    {
        auto parameter = reader.ReadUint8();
        auto start = reader.ReadFloat();
        auto end = reader.ReadFloat();
    }

    void OnChangeMaterial(PacketReader &reader) // TAG: 16E9F80
    {
        auto startFrame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto materialIndex = reader.ReadUint8();
        auto time = reader.ReadUint32();
        auto interpMode = reader.ReadBits(4); // ???

        /*
        auto vectorParametersLength = reader.ReadUint32();
        for (size_t i = 0; i < vectorParametersLength; i++)
        {
            OnChangeMaterialVectorParam(reader);
        }

        auto floatParametersLength = reader.ReadUint32();
        for (size_t i = 0; i < floatParametersLength; i++)
        {
            OnChangeMaterialFloatParam(reader);
        }
        */
    }

    void OnStaticMeshFlagsChanged(PacketReader &reader) // TAG: 16E9FF0
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto flags = reader.ReadUint8();

        if (_isVerbose)
        {
            printf("StaticMeshFlagsChanged:\n  componentId = %llu\n  flags = %u\n",
                componentid,
                flags
            );
        }
    }

    void OnStaticMeshScaleChanged(PacketReader &reader)  // TAG: 16EA060
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto scale = reader.ReadFloat();
    }

    void OnRiggedMeshFlagsChange(PacketReader &reader) // TAG: 16EA0D0
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto flags = reader.ReadUint8();

        if (_isVerbose)
        {
            printf("OnRiggedMeshFlagsChange:\n  componentId = %llu\n  flags = %u\n",
                componentid,
                flags
            );
        }
    }

    void OnRiggedMeshScaleChanged(PacketReader &reader) // TAG: 16EA140
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto scale = reader.ReadFloat();
    }

    void OnScriptCameraMessage(PacketReader &reader) // TAG: 16EA1B0
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto controlMode = reader.ReadBits(4);
    }

    void OnUpdateRuntimeInventorySettings(PacketReader &reader) // TAG: 16EA220
    {
        auto spawnSource = reader.ReadUint8();
        auto spawnLifetimePolicy = reader.ReadUint8();
        auto totalSpawnLimit = reader.ReadUint16();
        auto perUserSpawnLimit = reader.ReadUint16();
        auto spawnTimeout = reader.ReadUint32();

        printf("[%s] OnUpdateRuntimeInventorySettings\n  spawnSource = %u\n  spawnLifetimePolicy = %u\n  totalSpawnLimit = %u\n  perUserSpawnLimit = %u\n  spawnTimeout = %u\n",
            _isSender ? "OUT" : "IN",
            spawnSource,
            spawnLifetimePolicy,
            totalSpawnLimit,
            perUserSpawnLimit,
            spawnTimeout
        );
    }
};
