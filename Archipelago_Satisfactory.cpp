#include "Archipelago.h"
#include "Archipelago_Satisfactory.h"

#include <set>
#include <iostream>

//imports from apcpp
extern int ap_player_team;
extern std::set<int> teams_set;
extern std::map<int, AP_NetworkPlayer> map_players;

extern std::string getItemName(std::string game, int64_t id);
//

std::vector<int64_t> all_locations;

void satisfactory_Shutdown() {
    all_locations.clear();
}

int AP_GetCurrentPlayerTeam() {
    return ap_player_team;
}

std::string AP_GetItemName(std::string game, int64_t id) {
    return getItemName(game, id);
}

std::vector<int64_t> AP_GetAllLocations() {
    return all_locations;
}

std::vector<std::pair<int,std::string>> AP_GetAllPlayers() {
    std::vector<std::pair<int,std::string>> allPlayers;

    for (int team : teams_set) {
        for (std::pair<int, AP_NetworkPlayer> player : map_players) {
            std::pair<int,std::string> teamPlayer = std::pair<int,std::string>(team, player.second.alias);

            allPlayers.push_back(teamPlayer);
        }
    }

    return allPlayers;
}

void satisfactory_parse_response(Json::Value& packet, std::string command){
    if (cmd == "Connected") {
        unsigned int checked_size = packet["checked_locations"].size();

        all_locations.resize(checked_size + packet["missing_locations"].size())

        for (unsigned int i = 0; i < packet["checked_locations"].size(); i++)
            all_locations[i] = packet["checked_locations"][i].asInt64();
        for (unsigned int i = 0; i < packet["missing_locations"].size(); i++)
            all_locations[checked_size + i] = packet["missing_locations"][i].asInt64();
    }
}

void (*log_external)(std::string) = nullptr;
void AP_SetLoggingCallback(void (*f_log)(std::string)) {
    log_external = f_log;
}
void log(std::string message) {
    if (log_external != nullptr)
        log_external(message);
}
