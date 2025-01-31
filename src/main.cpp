#pragma once
#include "resourceTypes.hpp"
#include<iostream>
#include <string>
#include <system_error>
#include <vector>
#include <variant>
#include "methods/methods.hpp"
#include "response.hpp"

using ResourceType = std::variant<User>;

enum ProtocolVersion{
    ver_one
};



std::vector<std::string> parse_string(std::string input, std::string delimiter){
    std::vector<std::string> parsed_string;
    auto pos = input.find(delimiter);
    while(pos != input.npos){
        parsed_string.push_back(input.substr(0, pos));
        input.erase(0, pos + 1);
        pos = input.find(delimiter);
    }
    parsed_string.push_back(input);
    return parsed_string;
}

class Request{
    std::variant<GET, POST, PUT, HEAD, DELETE> method;
    ResourceType resource;


    ProtocolVersion version;
    std::unique_ptr<Response> setup_response;

    void error_in_setup(std::string error_message){
        setup_response = std::move(std::make_unique<BadRequest>(std::move(error_message)));
    }

    bool populate_method(std::string input){
        if (input == "GET") {
            method = GET{};
        } else if (input == "POST"){
            method = POST{};
        } else if (input == "HEAD"){
            method = HEAD{};
        } else if (input == "PUT"){
            method = PUT{};
        } else if (input == "DELETE"){
            method = DELETE{};
        } else {error_in_setup("Invalid request: " + input);
            return false;
        }
        return true;
    }

    bool populate_source(std::string input){
        if(input == "User"){
            resource = User();
        } else {
            error_in_setup("Invalid source data requested: " + input);
            return false;
        }
        return true;
    }

    bool populate_version(std::string input){
        if (input == "HTTP/1.0"){
            version = ver_one;
        } else {
            error_in_setup("Invalid HTTP version: " + input);
            return false;
        }
        return true;
    }
    public:
    Request(): setup_response(std::make_unique<OK>())
    {}

    std::optional<std::string> setup(std::string input){
        
        const std::vector<std::string> parsed_input = parse_string(input, " ");
        if(parsed_input.size() != 3){
            error_in_setup("Incorrect number of inputs, number of inputs for " + input + " is " + std::to_string(parsed_input.size()));
            return setup_response->handle();
        }
        if(!populate_source(parsed_input[1])){return setup_response->handle();};
        if(!populate_method(parsed_input[0])){return setup_response->handle();};
        if(!populate_version(parsed_input[2])){return setup_response->handle();};
        return {};
    }

    std::optional<std::string> validate(){
        return setup_response->handle();
    }

    std::string execute(std::string& json){
        std::visit([&](auto& method){method.execute(resource, json);}, method);
        return "hi";
    }
};

class Header{
    std::string content_type;
    std::string accept;
    int content_length;
    std::string user_agent;
    std::unique_ptr<Response> setup_response;

    void error_in_setup(std::string error_message){
        setup_response = std::move(std::make_unique<BadRequest>(std::move(error_message)));
    }

    bool parse_input_field(std::string input_field){
        std::vector<std::string> input_pair = parse_string(input_field, ": ");
        if(input_pair.size() < 2){
            error_in_setup("Too few parameters for input " + input_field); 
            return false;
        }
        if(input_pair[0] == "User-Agent"){
            user_agent = input_pair[1];
        }
        else if(input_pair[0] == "Content-Length"){
            try{content_length = std::stoi(input_pair[1]);}
            catch(std::invalid_argument const& ex){
                error_in_setup("Content-length must be convertible to int, but " + input_pair[1] + " isn't");
                return false;
            }
        }
        return true;
    }

    public:
    Header(): setup_response(std::make_unique<OK>()){}
    std::optional<std::string> setup(std::vector<std::string> input_fields) {
        for(std::string input_field: input_fields){
            if(!parse_input_field(input_field))
            return setup_response->handle();
        }
        std::cout << user_agent << "\n";
        std::cout << content_length << "\n";
        return {};
    }

    std::optional<std::string> validate(){
        return setup_response->handle();
    }
};

class HTTPReceiver{
    std::vector<std::string> parsed_input;
    Request request;
    Header header;
    std::string body;

    void parse_input(std::string input){
        auto pos = input.find("\n");
        while(pos != input.npos){
            parsed_input.push_back(input.substr(0, pos));
            input.erase(0, pos + 1);
            pos = input.find("\n");
        }
        parsed_input.push_back(input);
    }

    public:
    HTTPReceiver(std::string input){
        parsed_input = parse_string(input, "\n");

        if(parsed_input.size() == 0){return;}

        if(request.setup(parsed_input[0])){
            std::cout << "Error occurred: " << request.validate().value() << "\n";
            return;
        };

        std::vector<std::string> header_input;
        bool header_end_encountered = false;
        for (size_t i = 1; i < parsed_input.size(); i++){
            std::cout << parsed_input[i] << "\n";
            if(header_end_encountered){
                body.append(parsed_input[i]);
                continue;
            }

            if(parsed_input[i] == ""){
                header_end_encountered = true;
            }

            if(!header_end_encountered){
                header_input.push_back(parsed_input[i]);
            }
        }

        if(header.setup(header_input)){
             std::cout << "Error occurred: " << header.validate().value() << "\n";
            return;
        }
        std::cout << body << "\n";

        request.execute(body);
        
    }

    void internal_validate(){

    }
};

int main(){
    // const std::string input = "POST User HTTP/1.0\nUser-Agent: Mozilla/5.0\nContent-Length: 463\nHost: www.example.com\n\n{\"id\":1,\"time_of_creation\":1738155433,\"username\":\"AnotherTestUser\", \"picture\":\"TestUser\"}";
    const std::string input = "POST User HTTP/1.0\nUser-Agent: Mozilla/5.0\nContent-Length: 463\nHost: www.example.com\n\n{\"username\":\"AnotherTestUser\"}";
    HTTPReceiver test_receiver = HTTPReceiver(input);

    return 0;
}