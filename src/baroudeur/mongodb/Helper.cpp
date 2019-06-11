#include "../../../include/baroudeur/mongodb/Helper.hpp"
#include "../../../include/baroudeur/mongodb/Utility.hpp"

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/exception/exception.hpp>

#include <fstream>
#include <iostream>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

namespace baroudeur {
namespace mongodb {

struct Helper::Impl final {
    static constexpr const char * const db_name = "baroudeur";
    static constexpr const char * const latkey = "lat";
    static constexpr const char * const lonkey = "lon";
    static constexpr const char * const tagskey = "tags";
    static constexpr const char * const placenamekey = "name";


    Impl();
    virtual ~Impl();
    void ModifyCollectionWithFileContent(const std::string & collection_name, const std::string & path_to_file);
    osm::OsmId GetClosestBusStop(const std::string & country, const std::string & place);



private: 
    std::string CreateStringFromFileContent(const std::string & path_to_file) const;
    bool CollectionExists(const std::string & collection_name);
private:
    Impl(const Impl&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(const Impl&) = delete;
    Impl& operator=(Impl&&) = delete;
private:  
    mongocxx::database db_;
};

std::shared_ptr<Helper> Helper::Make() {
    return std::make_shared<Helper>();
}

Helper::Impl::Impl() {
  mongocxx::instance instance{}; // This should be done only once.
  mongocxx::client client{mongocxx::uri{}};   
  db_ = client[db_name];
}

Helper::Impl::~Impl() {}

Helper::Helper()
: pimpl_(new Impl()) {}

Helper::~Helper() {}


osm::OsmId Helper::GetClosestBusStop(const std::string & country, const std::string & place) {
    if(!pimpl_) throw std::runtime_error("Pimpl is null.");
    pimpl_->GetClosestBusStop(country, place);
}


bool Helper::Impl::CollectionExists(const std::string & collection_name) {
    auto coll_name_list = db_.list_collection_names();
    auto it = std::find(coll_name_list.begin(), coll_name_list.end(), collection_name);
    return (it != coll_name_list.end());
}

osm::OsmId Helper::Impl::GetClosestBusStop(const std::string & country, const std::string & place) {
    // scan places
    // find node lon lat of place
    auto coll_name = std::string(country) + std::string(osm::PLACES_SUFFIX);
    if(!CollectionExists(coll_name))
        return osm::OsmId(0ull); // std optional
    mongocxx::collection coll = db_[coll_name];
    mongocxx::cursor cursor = coll.find({
    }); 
    for(auto && doc : cursor) {
       auto el =  doc.find(std::string(tagskey));
       if(el != doc.end()) {
        //auto place_name = el.find(std::string(placenamekey));
       }
    }   
}


std::string Helper::Impl::CreateStringFromFileContent(const std::string & path_to_file) const {
    std::ifstream f(path_to_file.c_str());
    if(!f.good()) throw std::runtime_error("File does not exist.");

    std::string file_content;
    f.seekg(0, std::ios::end);   
    file_content.reserve(f.tellg());
    f.seekg(0, std::ios::beg);
    file_content.assign((std::istreambuf_iterator<char>(f)),
                std::istreambuf_iterator<char>());
    
    return file_content;
}


void Helper::ModifyCollectionWithFileContent(const std::string & collection_name, const std::string & path_to_file) {
    if(!pimpl_) throw std::runtime_error("Pimpl is null.");
    pimpl_->ModifyCollectionWithFileContent(std::move(collection_name), path_to_file);
}

void Helper::Impl::ModifyCollectionWithFileContent(const std::string & collection_name, const std::string & path_to_file) {

    try {
    std::string file_content = CreateStringFromFileContent(path_to_file);
    auto value = bsoncxx::from_json(file_content);
            std::cout << collection_name << std::endl;


    mongocxx::collection coll = db_[collection_name];
                std::cout << "INSERTZZ" << std::endl;


                /*
                auto builder = bsoncxx::builder::stream::document{};
bsoncxx::document::value doc_value = builder
  << "name" << "MongoDB"
  << "type" << "database"
  << "count" << 1
  << "versions" << bsoncxx::builder::stream::open_array
    << "v3.2" << "v3.0" << "v2.6"
  << bsoncxx::builder::stream::close_array
  << "info" << bsoncxx::builder::stream::open_document
    << "x" << 203
    << "y" << 102
  << bsoncxx::builder::stream::close_document
  << bsoncxx::builder::stream::finalize;

coll.insert_one(std::move(doc_value));

                
                
                 */
                auto builder = bsoncxx::builder::stream::document{};
bsoncxx::document::value doc_value = builder
  << "name" << "MongoDB"
  << "type" << "database"
  << "count" << 1
  << "versions" << bsoncxx::builder::stream::open_array
    << "v3.2" << "v3.0" << "v2.6"
  << bsoncxx::builder::stream::close_array
  << "info" << bsoncxx::builder::stream::open_document
    << "x" << 203
    << "y" << 102
  << bsoncxx::builder::stream::close_document
  << bsoncxx::builder::stream::finalize;

coll.insert_one(std::move(doc_value));


    coll.insert_one(std::move(doc_value));
    }
    catch(bsoncxx::exception) {
                    std::cout << "FAILED" << std::endl;

    }

}


}
}
