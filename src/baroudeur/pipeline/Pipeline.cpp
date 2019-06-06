#include "../../../include/baroudeur/pipeline/Pipeline.hpp"
#include "../../../include/baroudeur/mongodb/Helper.hpp"


namespace baroudeur {
namespace pipeline {

struct Pipeline::Impl final {
    using OsmId = unsigned long long;
    Impl();
    virtual ~Impl();
  void Process(const std::string & country_core, const std::string & country_places_fp, const std::vector<std::string> & road_data_fps);
  std::vector<OsmId> GetBusStopsFromTo(const std::string & From, const std::string & To);

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

    //tmp bus data just first for now 
    if(!road_data_fps.empty())
        db_helper_->ModifyCollectionWithFileContent(std::string (country_core + "_roads_data"), road_data_fps.front());
    
}



std::vector<OsmId> Pipeline::Impl::GetBusStopsFromTo(const std::string & From, const std::string & To) {
    //algo
    std::vector<OsmId> bus_stops;

    const OsmId bus_stop_from = db_helper_->GetClosestBusStop(From);
    const OsmId bus_stop_to = db_helper_->GetClosestBusStop(To);

    const OsmId bus_route_from = db_helper_->GetBusRouteIdOfBusStop(bus_stop_from);
    const OsmId bus_route_to = db_helper_->GetBusRouteIdOfBusStop(bus_stop_to);

    if(bus_route_from == bus_route_to) {
        bus_stops.push_back(bus_stop_from);
        bus_stops.push_back(bus_stop_to);
        return bus_stops;

    } else {
        //get intersection
        std::vector<OsmId> bus_stops;
        std::vector<OsmId> bus_route_of_from = db_helper_->GetBusRouteOf(bus_stop_from);
        std::vector<OsmId> bus_route_of_to = db_helper_->GetBusRouteOf(bus_stop_to);
        std::sort(bus_route_of_from.begin(), bus_route_of_from.end());
        std::sort(bus_route_of_to.begin(), bus_route_of_to.end());
        auto it  = std::set_intersection(bus_route_of_from.begin(), bus_route_of_from.end(), bus_route_of_to.begin(), bus_route_of_to.end(), bus_stops.begin()));
        bus_stops.resize(it-bus_stops.begin()); 
        // check if empty, if so just push the 2 non connecting bus stops

        // todo: bus stop class
        return bus_stops;
    }


    return bus_stops;
}




}
}