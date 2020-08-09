#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "MessageHandler.h"
#include "PacketReader.hpp"
#include "Utils.hpp"

//void OnWorldStateCreateWorld(PacketReader &reader);
//void OnWorldStateDestroyWorld(PacketReader &reader);
//void OnWorldStateRigidBodyComponentInitialState(PacketReader &reader);
//void OnWorldStateAnimationComponentInitialState(PacketReader &reader);
//void OnWorldStateLoadClusterDefinition(PacketReader &reader);
//void OnWorldStateComponentRelativeTransform(PacketReader &reader);
//void OnWorldStateInitiateCluster(PacketReader &reader);
//void OnWorldStateCreateClusterViaDefinition(PacketReader &reader);
//void OnWorldStateDestroyCluster(PacketReader &reader);
//void OnWorldStateDestroyObject(PacketReader &reader);
//void OnWorldStateDestroySourceIdSpace(PacketReader &reader);
//void OnWorldStateCreateCharacterNode(PacketReader &reader);
//void OnWorldStateCreateAgentController(PacketReader &reader);
//void OnWorldStateDestroyAgentController(PacketReader &reader);

class WorldState : public MessageHandler
{
public:
    bool OnMessage(uint32_t messageId, PacketReader &reader)
    {
        switch (messageId)
        {
            case WorldStateMessages::CreateWorld: // TAG: 1BB5960
            {
                this->OnCreateWorld(reader);
                break;
            }
            case WorldStateMessages::DestroyWorld: // TAG: 1BB59D0
            {
                this->OnDestroyWorld(reader);
                break;
            }
            case WorldStateMessages::RigidBodyComponentInitialState: // TAG: 1BB5B90
            {
                this->OnRigidBodyComponentInitialState(reader);
                break;
            }
            case WorldStateMessages::AnimationComponentInitialState: // TAG: 1BB5C00
            {
                this->OnAnimationComponentInitialState(reader);
                break;
            }
            case WorldStateMessages::LoadClusterDefinition: // TAG: 1BB5C70
            {
                this->OnLoadClusterDefinition(reader);
                break;
            }
            case WorldStateMessages::ComponentRelativeTransform: // TAG: 1BB5CE0
            {
                this->OnComponentRelativeTransform(reader);
                break;
            }
            case WorldStateMessages::InitiateCluster: // TAG: 1BB5D50
            {
                this->OnInitiateCluster(reader);
                break;
            }
            case WorldStateMessages::CreateClusterViaDefinition: // TAG: 1BB5DC0
            {
                this->OnCreateClusterViaDefinition(reader);
                break;
            }
            case WorldStateMessages::DestroyCluster: // TAG: 1BB5E30
            {
                this->OnDestroyCluster(reader);
                break;
            }
            case WorldStateMessages::DestroyObject: // TAG: 1BB5EA0
            {
                this->OnDestroyObject(reader);
                break;
            }
            case WorldStateMessages::DestroySourceIdSpace: // TAG: 1BB6060
            {
                this->OnDestroySourceIdSpace(reader);
                break;
            }
            case WorldStateMessages::CreateCharacterNode: // TAG: 1BB6220
            {
                this->OnCreateCharacterNode(reader);
                break;
            }
            case WorldStateMessages::CreateAgentController: // TAG: 1BB6290
            {
                this->OnCreateAgentController(reader);
                break;
            }
            case WorldStateMessages::DestroyAgentController: // TAG: 1BB6300
            {
                this->OnDestroyAgentController(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;;
    }

    void OnCreateWorld(PacketReader &reader) // TAG: 1BB5960
    {
        auto worldDefinition = reader.ReadUUID();
        auto startingClusterId = reader.ReadUint32();
        auto startingObjectId = reader.ReadUint32();

        printf("[%s] OnCreateWorld:\n  worldDefinition = %s\n  unk1 = %d\n  startingObjectId = %d\n",
            _isSender ? "OUT" : "IN",
            worldDefinition.c_str(),
            startingClusterId,
            startingObjectId
        );
    }

    void OnDestroyWorld(PacketReader &reader) // TAG: 1BB59D0
    {
        auto worldId = reader.ReadUint32();
    }

    void OnRigidBodyComponentInitialState(PacketReader &reader) // TAG: 1BB5B90
    {
        auto relativeComponentId = reader.ReadUint32();
        auto linearVelocity = reader.ReadVectorF(3);
        auto angularVelocity = reader.ReadVectorF(3);
    }

    void OnAnimationComponentInitialState(PacketReader &reader) // TAG: 1BB5C00
    {
        auto relativeComponentId = reader.ReadUint32();
        auto velocity = reader.ReadVectorF(3);
        auto behaviorState = reader.ReadArray();
    }

    void OnLoadClusterDefinition(PacketReader &reader) // TAG: 1BB5C70
    {
        auto resourceId = reader.ReadUUID();
        auto clusterId = reader.ReadUint32();

        //printf("[%s] OnLoadClusterDefinition:\n  resourceId = %s\n  clusterId = %d\n",
        // _isSender ? "OUT" : "IN",
        //    resourceId.c_str(),
        //    clusterId
        //);
    }

    void OnComponentRelativeTransform(PacketReader &reader) // TAG: 1BB5CE0
    {
        auto relativePosition = reader.ReadVectorF(3);
        auto relativeRotation = reader.ReadVectorF(4);
        auto componentId = reader.ReadUint64();
    }

    void OnInitiateCluster(PacketReader &reader) // TAG: 1BB5D50
    {
        auto clusterId = reader.ReadUint32();
        auto frame = reader.ReadUint64();
        auto rigidBodyInitialStatesLength = reader.ReadUint32();
        for (size_t i = 0; i < rigidBodyInitialStatesLength; i++)
        {
            OnRigidBodyComponentInitialState(reader);
        }

        auto animationInitialStatesLength = reader.ReadUint32();
        for (size_t i = 0; i < animationInitialStatesLength; i++)
        {
            OnAnimationComponentInitialState(reader);
        }

        //printf("[%s] OnInitiateCluster:\n  clusterId = %d\n  frame = %llu\n  rigidBodyInitialStatesLength = %u\n  animationInitialStatesLength = %u\n",
        //_isSender ? "OUT" : "IN",
        //    clusterId,
        //    frame,
        //    rigidBodyInitialStatesLength,
        //    animationInitialStatesLength
        //);
    }

    void OnCreateClusterViaDefinition(PacketReader &reader) // TAG: 1BB5DC0
    {
        auto clusterId = reader.ReadUint32();
        auto startingObjectId = reader.ReadUint32();
        auto resourceId = reader.ReadUUID();

        auto spawnPosition = reader.ReadVectorF(3);
        auto spawnRotation = reader.ReadVectorF(4);

        //printf("[%s] OnCreateClusterViaDefinition:\n  clusterId = %u\n  startingObjectId = %u\n  resourceId = %s\n",
        //_isSender ? "OUT" : "IN",
        //    clusterId,
        //    startingObjectId,
        //    resourceId.c_str()
        //);
    }

    void OnDestroyCluster(PacketReader &reader) // TAG: 1BB5E30
    {
        auto frame = reader.ReadUint64();
        auto clusterId = reader.ReadUint32();

        //printf("[%s] WorldStateMessages::DestroyCluster\n  frame = %llu\n  clusterId = %u\n",
        //_isSender ? "OUT" : "IN",
        //    frame,
        //    clusterId
        //);
    }

    void OnDestroyObject(PacketReader &reader) // TAG: 1BB5EA0
    {
        auto objectId = reader.ReadUint32();
    }

    void OnDestroySourceIdSpace(PacketReader &reader) // TAG: 1BB6060
    {
        auto sourceIdSpace = reader.ReadUint32();
    }

    void OnCreateCharacterNode(PacketReader &reader) // TAG: 1BB6220
    {
        auto nodeType = reader.ReadUint8();
        auto controllerNodetype = reader.ReadUint8();
        auto flags = reader.ReadUint8();
    }

    void OnCreateAgentController(PacketReader &reader) // TAG: 1BB6290
    {
        auto sessionId = reader.ReadUint32();
        auto clusterId = reader.ReadUint32();
        auto agentControllerId = reader.ReadUint32();
        auto characterObjectId = reader.ReadUint32();

        auto characterNodesLength = reader.ReadUint32();
        for (size_t i = 0; i < characterNodesLength; i++)
        {
            OnCreateCharacterNode(reader);
        }

        auto frame = reader.ReadUint64();
        auto personaId = reader.ReadUUID();
        auto isRemoteAgent = reader.ReadUint8();

        printf("[%s] OnCreateAgentController:\n  sessionId = %d\n  clusterId = %u\n  agentControllerId = %u\n  characterObjectId = %u\n  characterNodesLength = %u\n  frame = %llu\n  personaId = %s\n  isRemoteAgent = %d\n",
            _isSender ? "OUT" : "IN",
            sessionId,
            clusterId,
            agentControllerId,
            characterObjectId,
            characterNodesLength,
            frame,
            personaId.c_str(),
            isRemoteAgent
        );
    }

    void OnDestroyAgentController(PacketReader &reader) // TAG: 1BB6300
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();

        printf("[%s] WorldStateMessages::DestroyAgentController:\n  frame = %llu\n  agentControllerId = %u\n",
            _isSender ? "OUT" : "IN",
            frame,
            agentControllerId
        );
    }
};
