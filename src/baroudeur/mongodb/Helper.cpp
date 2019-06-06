#include "../../../include/baroudeur/mongodb/Helper.hpp"
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <fstream>

namespace baroudeur {
namespace mongodb {

struct Helper::Impl final {
    static constexpr const char * const db_name = "baroudeur";
    Impl();
    virtual ~Impl();
    void ModifyCollectionWithFileContent(const std::string & collection_name, const std::string & path_to_file);

private: 
    std::string CreateStringFromFileContent(const std::string & path_to_file) const;
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
    mongocxx::instance instance{}; 
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);    
    db_ = client[db_name];
}

Helper::Impl::~Impl() {}

Helper::Helper()
: pimpl_(new Impl()) {}

Helper::~Helper() {}

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
    std::string file_content = CreateStringFromFileContent(path_to_file);
    auto value = bsoncxx::from_json(file_content);

    mongocxx::collection coll = db_[collection_name];
    coll.insert_one(std::move(value));
}


}
}
