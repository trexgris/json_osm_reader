#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <jsoncpp/json/json.h>
#include "../../include/baroudeur/mongodb/Helper.hpp"

int main() {
    std::ifstream ifs("osm_response.json");
    std::ifstream ifs2("places.json");
    auto test = baroudeur::mongodb::Helper::Make();
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);     // Reader can also read strings
    const Json::Value& chars = obj["features"];
    std::set<std::string> unique_cities;

    



    return 1;
}
