#include <iomanip>
#include <iostream>
#include<string>
#include<iomanip>
#include"json.hpp"

// How can I make sure ids are unique?

struct User{
    public:
    int id;
    std::string username;
    time_t time_of_creation;

    User(int id, std::string username);
    User(std::string json_input);
    std::string to_string();
};

template<> struct nlohmann::adl_serializer<User>
{
    static void to_json(json& j, const User& user)
    {
        j = json{{"id", user.id}, {"username", user.username}, {"time_of_creation", user.time_of_creation}};
    }
    static void from_json(const json& j, User& user)
    {
        j.at("id").get_to(user.id);
        j.at("username").get_to(user.username);
        j.at("time_of_creation").get_to(user.time_of_creation);
    }
};