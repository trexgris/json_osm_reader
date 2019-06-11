#include "../../../include/baroudeur/pipeline/Pipeline.hpp"
#include "../../../include/baroudeur/mongodb/Helper.hpp"
#include "../../../include/baroudeur/osm/Utility.hpp"
#include <algorithm>
#include <iostream>

namespace baroudeur {
namespace pipeline {

struct Pipeline::Impl final {
    Impl();
    virtual ~Impl();
    void Process(const std::string & country_core, const std::string & country_places_fp, const std::vector<std::string> & road_data_fps);
    std::vector<osm::OsmId> GetBusStopsFromTo(const std::string & From, const std::string & To);

private:
    Impl(const Impl&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(const Impl&) = delete;
    Impl& operator=(Impl&&) = delete;
private:
    mongodb::Helper::Ptr db_helper_;

};

void Pipeline::Process(const std::string & country_core,const std::string & country_places_fp, const std::vector<std::string> & road_data_fps) {
    if(!pimpl_) throw std::runtime_error("Pimpl is null.");
    pimpl_->Process(country_core, country_places_fp, road_data_fps);


}
std::shared_ptr<Pipeline> Pipeline::Make() {
    return std::make_shared<Pipeline>();
}

Pipeline::Impl::Impl() {
    db_helper_ = mongodb::Helper::Make();
     
}

Pipeline::Impl::~Impl() {}

Pipeline::Pipeline()
: pimpl_(new Impl()) {}

Pipeline::~Pipeline() {}

 void Pipeline::Impl::Process(const std::string & country_core, const std::string & country_places_fp, const std::vector<std::string> & road_data_fps) {  
    // upload places    

    db_helper_->ModifyCollectionWithFileContent(std::string (country_core + "_places"), country_places_fp);
        std::cout << "SEGMENTATION CHECK A2" << std::endl;

    //bus data, to rework
    if(road_data_fps.size() == 2) {
        db_helper_->ModifyCollectionWithFileContent(std::string (country_core + "_stops"), road_data_fps[0]);
        db_helper_->ModifyCollectionWithFileContent(std::string (country_core + "_routes"), road_data_fps[1]);
    }
    
    //loop over places
    
}

std::vector<osm::OsmId> Pipeline::GetBusStopsFromTo(const std::string & From, const std::string & To) {
    if(!pimpl_) throw std::runtime_error("Pimpl is null.");
    pimpl_->GetBusStopsFromTo(From, To);
}


std::vector<osm::OsmId> Pipeline::Impl::GetBusStopsFromTo(const std::string & From, const std::string & To) {
    //algo
    std::vector<osm::OsmId> bus_stops;
    const osm::OsmId bus_stop_from = db_helper_->GetClosestBusStop("t", From);
    const osm::OsmId bus_stop_to = db_helper_->GetClosestBusStop("t", To);
   /*const osm::OsmId bus_route_from = db_helper_->GetBusRouteIdOfBusStop(bus_stop_from);
    const osm::OsmId bus_route_to = db_helper_->GetBusRouteIdOfBusStop(bus_stop_to);
    //get intersection, which can be empty
    std::vector<osm::OsmId> bus_route_of_from = db_helper_->GetBusRouteOf(bus_stop_from);
    std::vector<osm::OsmId> bus_route_of_to = db_helper_->GetBusRouteOf(bus_stop_to);
    std::sort(bus_route_of_from.begin(), bus_route_of_from.end());
    std::sort(bus_route_of_to.begin(), bus_route_of_to.end());
    auto it  = std::set_intersection(bus_route_of_from.begin(), bus_route_of_from.end(), bus_route_of_to.begin(), bus_route_of_to.end(), std::back_inserter(bus_stops));        
    bus_stops.push_back(bus_stop_from);
    bus_stops.push_back(bus_stop_to);*/
    return bus_stops;
}

}
}