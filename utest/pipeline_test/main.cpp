#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <jsoncpp/json/json.h>
#include "../../include/baroudeur/mongodb/Helper.hpp"
#include "../../include/baroudeur/pipeline/Pipeline.hpp"
#include "../../include/baroudeur/osm/Utility.hpp"


int main() {
  /* std::ifstream ifs("osm_response.json");
    std::ifstream ifs2("places.json");
    auto test = baroudeur::mongodb::Helper::Make();
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);     // Reader can also read strings
    const Json::Value& chars = obj["features"];
    std::set<std::string> unique_cities;

    */
    auto pipe = baroudeur::pipeline::Pipeline::Make();
        std::cout << "SEGMENTATION CHECK B" << std::endl;

    std::vector<std::string> roaddata;
    roaddata.emplace_back("bus_stops.json");
    roaddata.emplace_back("bus_routes.json");
      std::cout << "SEGMENTATION CHECK C" << std::endl;

    pipe->Process("Nicaragua", "places.json", roaddata);
        std::cout << "SEGMENTATION CHECK D" << std::endl;

    auto bus_stops_id = pipe->GetBusStopsFromTo("Managua", "Léon");




    return 0;
}