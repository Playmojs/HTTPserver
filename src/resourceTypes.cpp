#pragma once
#include<string>
#include<chrono>
#include"resourceTypes.hpp"
#include<fstream>
#include<filesystem>

using json = nlohmann::json;

namespace chrono = std::chrono;

UserData::UserData(int id, std::string username) : _id(id), _username(username){
    _time_of_creation = time(0);
}

std::string UserData::to_string(){
    json j = *this;
    return j.dump();
}

void UserData::from_string(std::string jsoninput){
    json j = json::parse(jsoninput);
    _username = j["username"].template get<std::string>();

    if(j.count("id") != 0){
        _id= j["id"].template get<int>();
    }

    if(j.count("time_of_creation") != 0){
        _time_of_creation = j["time_of_creation"].template get<time_t>();
    }
}

UserData::UserData(std::string jsoninput){
    json j = json::parse(jsoninput);
    try{
        _id = j["id"].template get<int>();
    } catch(std::exception ex){
        std::cout << "Wrong id \n";
    }
    _username = j["username"].template get<std::string>();
    _time_of_creation = j["time_of_creation"].template get<time_t>();
}

UserData& UserData::set_id(int id){
    _id = id;
    return (*this);
}

UserData& UserData::set_time_of_creation(time_t time_of_creation){
    _time_of_creation = time_of_creation;
    return (*this);
}


void User::populate_incomplete_data(){
    data.set_id(7).set_time_of_creation(time(0));
}

void User::save_current_to_file(){
    std::ofstream file(path, std::ios::app);
    if (file.is_open()){
        file << data.to_string() << "\n";
        file.close();
    } else {
        throw std::runtime_error("Blabla test");
    }

}