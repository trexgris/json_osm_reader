#pragma once
#include <memory>
#include <string>
#include "../osm/Utility.hpp"

namespace baroudeur {
namespace mongodb {

class Helper final {
public:
  using Ptr = std::shared_ptr<Helper>;
  static std::shared_ptr<Helper> Make();
  Helper();
  ~Helper();
  osm::OsmId GetClosestBusStop(const std::string & country, const std::string & place);

  void ModifyCollectionWithFileContent(const std::string & collection_name, const std::string & jsondata);
private:
  Helper(const Helper&) = delete;
  Helper(Helper&&) = delete;
  Helper& operator=(const Helper&) = delete;
  Helper& operator=(Helper&&) = delete;

private: 
    struct Impl;
    std::unique_ptr<Impl> pimpl_;        
};    
}
}