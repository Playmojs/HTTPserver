#include <iomanip>
#include <iostream>
#include<string>
#include<iomanip>
#include"json.hpp"
#include <filesystem>


// How can I make sure ids are unique?
struct UserData{
    public:
    int id;
    std::string username;
    time_t time_of_creation;

    UserData(int id, std::string username);
    UserData(std::string json_input);
    UserData(){}
    std::string to_string();
};

template<> struct nlohmann::adl_serializer<UserData>
{
    static void to_json(json& j, const UserData& user)
    {
        j = json{{"id", user.id}, {"username", user.username}, {"time_of_creation", user.time_of_creation}};
    }
    static void from_json(const json& j, UserData& user)
    {
        j.at("id").get_to(user.id);
        j.at("username").get_to(user.username);
        j.at("time_of_creation").get_to(user.time_of_creation);
    }
};

struct User{
    UserData userdata;
    std::filesystem::path path = "/db/users.txt";
};