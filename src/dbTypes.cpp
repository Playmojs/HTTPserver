#pragma once
#include<string>
#include<chrono>
#include"dbTypes.hpp"
#include<iostream>
#include<filesystem>

using json = nlohmann::json;

namespace chrono = std::chrono;

UserData::UserData(int id, std::string username) : id(id), username(username){
        time_of_creation = time(0);
}

std::string UserData::to_string(){
    json j = *this;
    return j.dump();
    
}

UserData::UserData(std::string jsoninput){
    json j = json::parse(jsoninput);
    try{
        id = j["id"].template get<int>();
    } catch(std::exception ex){
        std::cout << "Wrong id \n";
    }
    username = j["username"].template get<std::string>();
    time_of_creation = j["time_of_creation"].template get<time_t>();
}