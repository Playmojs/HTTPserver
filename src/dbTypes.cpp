#pragma once
#include<string>
#include<chrono>
#include"dbTypes.hpp"
#include<iostream>
#include<filesystem>

using json = nlohmann::json;

namespace chrono = std::chrono;

User::User(int id, std::string username) : id(id), username(username){
        time_of_creation = time(0);
}

std::string User::to_string(){
    json j = *this;
    return j.dump();
    
}

User::User(std::string jsoninput){
    json j = json::parse(jsoninput);
    std::cout << j << "\n";
    id = j["id"].template get<int>();
    username = j["username"].template get<std::string>();
    time_of_creation = j["time_of_creation"].template get<time_t>();
}