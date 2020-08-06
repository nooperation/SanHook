#pragma once

#include <cstdio>
#include <string>
#include <vector>

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


class GameWorld
{
public:
    static void OnUpdateRuntimeInventorySettings(PacketReader &reader)
    {
        auto spawnSource = reader.ReadUint8();
        auto spawnLifetimePolicy = reader.ReadUint8();
        auto totalSpawnLimit = reader.ReadUint16();
        auto perUserSpawnLimit = reader.ReadUint16();
        auto spawnTimeout = reader.ReadUint32();
    }

    static void OnScriptCameraMessage(PacketReader &reader)
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto controlMode = reader.ReadBits(4);
    }

    static void OnRiggedMeshScaleChanged(PacketReader &reader)
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto scale = reader.ReadFloat();
    }

    static void OnRiggedMeshFlagsChange(PacketReader &reader)
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto flags = reader.ReadUint8();
    }

    static void OnStaticMeshScaleChanged(PacketReader &reader)
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto scale = reader.ReadFloat();
    }

    static void OnStaticMeshFlagsChanged(PacketReader &reader)
    {
        auto componentid = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto flags = reader.ReadUint8();
    }

    static void OnChangeMaterial(PacketReader &reader)
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

    static void OnChangeMaterialFloatParam(PacketReader &reader)
    {
        auto parameter = reader.ReadUint8();
        auto start = reader.ReadFloat();
        auto end = reader.ReadFloat();
    }

    static void OnChangeMaterialVectorParam(PacketReader &reader)
    {
        auto parameter = reader.ReadUint8();
        auto start = reader.ReadVectorF(3);
        auto end = reader.ReadVectorF(3);
    }

    static void OnMoveEntity(PacketReader &reader)
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

        printf("GameWorldMessages::MoveEntity:\n  startFrame = %llu\n  componentId = %llu\n startPosition = <%f, %f, %f>\n  targetPosition = <%f, %f, %f>\n  time = %u\n",
            startFrame,
            componentId,
            startPosition[0], startPosition[1], startPosition[2],
            targetPosition[0], targetPosition[1], targetPosition[2],
            time
        );
    }

    static void OnTimestamp(PacketReader &reader)
    {
        auto nanoseconds = reader.ReadUint64();
        auto frame = reader.ReadUint64();
    }
};
