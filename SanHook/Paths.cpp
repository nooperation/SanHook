#include <filesystem>

// No longer used
std::filesystem::path sanhookConfigImPath = "sanhook_config_im.txt"; // no longer useful
std::filesystem::path sanhookConfigPath = "sanhook_config.txt"; // this has been fixed and is no longer useful

// Input files

// binary file, first byte is enabled/disabled, remaining 16 bytes are resource uuid
//  enabled 1/0 -> Override spawned item with the following resource id
//  resourceId -> 16-byte resource id to replace spawned item with
std::filesystem::path sanhookConfigRezPath = "sanhook_config_rez.txt"; 

// 3 bytes. 
//  camrez_enabled 1/0 -> Spawn items at camera position instead of in front of you
//  emote_rez_enabled 1/0 -> Create a reaction instead of spawning items
//  emote_to_rez -> string up to 12 bytes to specify what reaction to spawn if emote_rez_enabled is set "Sansar.Heart"
std::filesystem::path sanhookConfigCamrezPath = "sanhook_config_camrez.txt";

// Single character. "1" = always broadcast, "0" = never broadcast
std::filesystem::path sanhookConfigBrodcast = "sanhook_config_broadcast.txt";

// key->name csv for local name resolution
/*
PersonaId,Handle
57e88916-0ddc-493d-a7e7-a650105e6cc8,Moondog-Sansar
dc51be25-f85e-4c80-8bce-4fa4635a8333,Binah-Sansar
effc2bd4-5748-44b1-981b-21c96394130c,Zorba-Sansar
...
*/
std::filesystem::path sanhookPersonasPath = "personas.csv";

// Output files
std::filesystem::path longlivedPath = "longlived.csv";
std::filesystem::path shortLivedPath = "shortlived.csv";
std::filesystem::path userdumpPath = "userdump.csv";
std::filesystem::path animationDumpPath = "animationdump.csv";
std::filesystem::path chatDumpPath = "chat.txt";
