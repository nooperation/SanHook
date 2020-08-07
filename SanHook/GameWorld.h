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
    bool OnMessage(uint32_t messageId, PacketReader &reader)
    {
        switch (messageId)
        {
            case GameWorldMessages::Timestamp:  // 16E9DC0
            {
                GameWorld::OnTimestamp(reader);
                break;
            }
            case GameWorldMessages::MoveEntity: // 16E9E30
            {
                GameWorld::OnMoveEntity(reader);
                break;
            }
            case GameWorldMessages::ChangeMaterialVectorParam:  // 16E9EA0
            {
                GameWorld::OnChangeMaterialVectorParam(reader);
                break;
            }
            case GameWorldMessages::ChangeMaterialFloatParam:  // 16E9F10
            {
                GameWorld::OnChangeMaterialFloatParam(reader);
                break;
            }
            case GameWorldMessages::ChangeMaterial:  // 16E9F80
            {
                GameWorld::OnChangeMaterial(reader);
                break;
            }
            case GameWorldMessages::StaticMeshFlagsChanged:  // 16E9FF0
            {
                GameWorld::OnStaticMeshFlagsChanged(reader);
                break;
            }
            case GameWorldMessages::StaticMeshScaleChanged:  // 16EA060
            {
                GameWorld::OnStaticMeshScaleChanged(reader);
                break;
            }
            case GameWorldMessages::RiggedMeshFlagsChange:  // 16EA0D0
            {
                GameWorld::OnRiggedMeshFlagsChange(reader);
                break;
            }
            case GameWorldMessages::RiggedMeshScaleChanged:  // 16EA140
            {
                GameWorld::OnRiggedMeshScaleChanged(reader);
                break;
            }
            case GameWorldMessages::ScriptCameraMessage:  // 16EA1B0
            {
                GameWorld::OnScriptCameraMessage(reader);
                break;
            }
            case GameWorldMessages::UpdateRuntimeInventorySettings:  // 16EA220
            {
                GameWorld::OnUpdateRuntimeInventorySettings(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    static void OnTimestamp(PacketReader &reader) // 16E9DC0
    {
        auto nanoseconds = reader.ReadUint64();
        auto frame = reader.ReadUint64();
    }

    static void OnMoveEntity(PacketReader &reader)// 16E9E30
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

    static void OnChangeMaterialVectorParam(PacketReader &reader)// 16E9EA0
    {
        auto parameter = reader.ReadUint8();
        auto start = reader.ReadVectorF(3);
        auto end = reader.ReadVectorF(3);
    }

    static void OnChangeMaterialFloatParam(PacketReader &reader)   // 16E9F10
    {
        auto parameter = reader.ReadUint8();
        auto start = reader.ReadFloat();
        auto end = reader.ReadFloat();
    }

    static void OnChangeMaterial(PacketReader &reader) // 16E9F80
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

    static void OnStaticMeshFlagsChanged(PacketReader &reader) // 16E9FF0
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto flags = reader.ReadUint8();
    }

    static void OnStaticMeshScaleChanged(PacketReader &reader)  // 16EA060
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto scale = reader.ReadFloat();
    }

    static void OnRiggedMeshFlagsChange(PacketReader &reader) // 16EA0D0
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto flags = reader.ReadUint8();
    }

    static void OnRiggedMeshScaleChanged(PacketReader &reader) // 16EA140
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto scale = reader.ReadFloat();
    }

    static void OnScriptCameraMessage(PacketReader &reader) // 16EA1B0
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto controlMode = reader.ReadBits(4);
    }

    static void OnUpdateRuntimeInventorySettings(PacketReader &reader) // 16EA220
    {
        auto spawnSource = reader.ReadUint8();
        auto spawnLifetimePolicy = reader.ReadUint8();
        auto totalSpawnLimit = reader.ReadUint16();
        auto perUserSpawnLimit = reader.ReadUint16();
        auto spawnTimeout = reader.ReadUint32();
    }
};
