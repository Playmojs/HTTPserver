#pragma once
#include <iomanip>
#include <iostream>
#include<string>
#include<iomanip>
#include"json.hpp"
#include <filesystem>
#include <variant>

// How can I make sure ids are unique?

struct UserData{
    public:
    int _id;
    std::string _username;
    time_t _time_of_creation;

    UserData(int id, std::string username);
    UserData(std::string json_input);
    UserData(){};
    std::string to_string();
    void from_string(std::string);
    UserData& set_id(int new_id);
    UserData& set_time_of_creation(time_t time);
};

template<> struct nlohmann::adl_serializer<UserData>
{
    static void to_json(json& j, const UserData& user)
    {
        j = json{{"id", user._id}, {"username", user._username}, {"time_of_creation", user._time_of_creation}};
    }
    static void from_json(const json& j, UserData& user)
    {
        j.at("id").get_to(user._id);
        j.at("username").get_to(user._username);
        j.at("time_of_creation").get_to(user._time_of_creation);
    }
};

struct User{
    UserData data;
    std::string path = "../db/users.txt";
    void populate_incomplete_data();
    void save_current_to_file();
};

// std::string to_json(ResourceType& resource){
//     std::string json;
//     std::visit([&](auto& resource){json = resource.data.to_string();}, resource);
//     return json;
// }

// std::string from_json(ResourceType& resource, std::string& json){
//     std::visit([&](auto& resource){resource.data.from_string(json);}, resource);
//     return json;
// }