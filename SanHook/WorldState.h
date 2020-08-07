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
                WorldState::OnCreateWorld(reader);
                break;
            }
            case WorldStateMessages::DestroyWorld: // TAG: 1BB59D0
            {
                WorldState::OnDestroyWorld(reader);
                break;
            }
            case WorldStateMessages::RigidBodyComponentInitialState: // TAG: 1BB5B90
            {
                WorldState::OnRigidBodyComponentInitialState(reader);
                break;
            }
            case WorldStateMessages::AnimationComponentInitialState: // TAG: 1BB5C00
            {
                WorldState::OnAnimationComponentInitialState(reader);
                break;
            }
            case WorldStateMessages::LoadClusterDefinition: // TAG: 1BB5C70
            {
                WorldState::OnLoadClusterDefinition(reader);
                break;
            }
            case WorldStateMessages::ComponentRelativeTransform: // TAG: 1BB5CE0
            {
                WorldState::OnComponentRelativeTransform(reader);
                break;
            }
            case WorldStateMessages::InitiateCluster: // TAG: 1BB5D50
            {
                WorldState::OnInitiateCluster(reader);
                break;
            }
            case WorldStateMessages::CreateClusterViaDefinition: // TAG: 1BB5DC0
            {
                WorldState::OnCreateClusterViaDefinition(reader);
                break;
            }
            case WorldStateMessages::DestroyCluster: // TAG: 1BB5E30
            {
                WorldState::OnDestroyCluster(reader);
                break;
            }
            case WorldStateMessages::DestroyObject: // TAG: 1BB5EA0
            {
                WorldState::OnDestroyObject(reader);
                break;
            }
            case WorldStateMessages::DestroySourceIdSpace: // TAG: 1BB6060
            {
                WorldState::OnDestroySourceIdSpace(reader);
                break;
            }
            case WorldStateMessages::CreateCharacterNode: // TAG: 1BB6220
            {
                WorldState::OnCreateCharacterNode(reader);
                break;
            }
            case WorldStateMessages::CreateAgentController: // TAG: 1BB6290
            {
                WorldState::OnCreateAgentController(reader);
                break;
            }
            case WorldStateMessages::DestroyAgentController: // TAG: 1BB6300
            {
                WorldState::OnDestroyAgentController(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;;
    }

    static void OnCreateWorld(PacketReader &reader) // TAG: 1BB5960
    {
        auto worldDefinition = reader.ReadUUID();
        auto startingClusterId = reader.ReadUint32();
        auto startingObjectId = reader.ReadUint32();

        printf("OnCreateWorld:\n  worldDefinition = %s\n  unk1 = %d\n  startingObjectId = %d\n",
            worldDefinition.c_str(),
            startingClusterId,
            startingObjectId
        );
    }

    static void OnDestroyWorld(PacketReader &reader) // TAG: 1BB59D0
    {
        auto worldId = reader.ReadUint32();
    }

    static void OnRigidBodyComponentInitialState(PacketReader &reader) // TAG: 1BB5B90
    {
        auto relativeComponentId = reader.ReadUint32();
        auto linearVelocity = reader.ReadVectorF(3);
        auto angularVelocity = reader.ReadVectorF(3);
    }

    static void OnAnimationComponentInitialState(PacketReader &reader) // TAG: 1BB5C00
    {
        auto relativeComponentId = reader.ReadUint32();
        auto velocity = reader.ReadVectorF(3);
        auto behaviorState = reader.ReadArray();
    }

    static void OnLoadClusterDefinition(PacketReader &reader) // TAG: 1BB5C70
    {
        auto resourceId = reader.ReadUUID();
        auto clusterId = reader.ReadUint32();

        printf("OnLoadClusterDefinition:\n  resourceId = %s\n  clusterId = %d\n",
            resourceId.c_str(),
            clusterId
        );
    }

    static void OnComponentRelativeTransform(PacketReader &reader) // TAG: 1BB5CE0
    {
        auto relativePosition = reader.ReadVectorF(3);
        auto relativeRotation = reader.ReadVectorF(4);
        auto componentId = reader.ReadUint64();
    }

    static void OnInitiateCluster(PacketReader &reader) // TAG: 1BB5D50
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

        printf("OnInitiateCluster:\n  clusterId = %d\n  frame = %llu\n  rigidBodyInitialStatesLength = %u\n  animationInitialStatesLength = %u\n",
            clusterId,
            frame,
            rigidBodyInitialStatesLength,
            animationInitialStatesLength
        );
    }

    static void OnCreateClusterViaDefinition(PacketReader &reader) // TAG: 1BB5DC0
    {
        auto clusterId = reader.ReadUint32();
        auto startingObjectId = reader.ReadUint32();
        auto resourceId = reader.ReadUUID();

        auto spawnPosition = reader.ReadVectorF(3);
        auto spawnRotation = reader.ReadVectorF(4);

        printf("OnCreateClusterViaDefinition:\n  clusterId = %u\n  startingObjectId = %u\n  resourceId = %s\n",
            clusterId,
            startingObjectId,
            resourceId.c_str()
        );
    }

    static void OnDestroyCluster(PacketReader &reader) // TAG: 1BB5E30
    {
        auto frame = reader.ReadUint64();
        auto clusterId = reader.ReadUint32();

        printf("WorldStateMessages::DestroyCluster\n  frame = %llu\n  clusterId = %u\n",
            frame,
            clusterId
        );
    }

    static void OnDestroyObject(PacketReader &reader) // TAG: 1BB5EA0
    {
        auto objectId = reader.ReadUint32();
    }

    static void OnDestroySourceIdSpace(PacketReader &reader) // TAG: 1BB6060
    {
        auto sourceIdSpace = reader.ReadUint32();
    }

    static void OnCreateCharacterNode(PacketReader &reader) // TAG: 1BB6220
    {
        auto nodeType = reader.ReadUint8();
        auto controllerNodetype = reader.ReadUint8();
        auto flags = reader.ReadUint8();
    }

    static void OnCreateAgentController(PacketReader &reader) // TAG: 1BB6290
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

        printf("OnCreateAgentController:\n  sessionId = %d\n  clusterId = %u\n  agentControllerId = %u\n  characterObjectId = %u\n  characterNodesLength = %u\n  frame = %llu\n  personaId = %s\n  isRemoteAgent = %d\n",
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

    static void OnDestroyAgentController(PacketReader &reader) // TAG: 1BB6300
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();

        printf("WorldStateMessages::DestroyAgentController:\n  frame = %llu\n  agentControllerId = %u\n",
            frame,
            agentControllerId
        );
    }
};
