#include <iostream>
#include <fstream>

#include <jsoncpp/json/json.h>


int main() {
    std::ifstream ifs("osm_response.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);     // Reader can also read strings
    const Json::Value& chars = obj["features"];
    std::set<string> unique_cities;

    for(auto && it : chars.size())

    for(int i = 0 ; i < chars.size() ; ++i) {

    }


std::cout <<chars.size();
    return 1;
}