#pragma once
#include <memory>
#include <string>
#include <vector>
#include "../../../include/baroudeur/osm/Utility.hpp"


namespace baroudeur {
namespace pipeline {

class Pipeline final {
public:
  static std::shared_ptr<Pipeline> Make();
  Pipeline();
  ~Pipeline();
  void Process(const std::string & country_core, const std::string & country_places_fp, const std::vector<std::string> & road_data_fps);
  std::vector<osm::OsmId> GetBusStopsFromTo(const std::string & From, const std::string & To); // tmp

private:
  Pipeline(const Pipeline&) = delete;
  Pipeline(Pipeline&&) = delete;
  Pipeline& operator=(const Pipeline&) = delete;
  Pipeline& operator=(Pipeline&&) = delete;

private: 
    struct Impl;
    std::unique_ptr<Impl> pimpl_;        
};    
}
}