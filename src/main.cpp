#include "dbTypes.hpp"
#include<iostream>
#include <string>
#include <system_error>
#include <vector>
#include <variant>
#include "methods/methods.hpp"

enum ProtocolVersion{
    ver_one
};

enum ParseResult{
    OK,
    ERROR
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
    std::string path;
    ProtocolVersion version;
    ParseResult result = OK;

    void get_method(std::string input){
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
        } else {result = ERROR;}
    }

    void get_version(std::string input){
        if (input == "HTML/1.0"){
            version = ver_one;
        } else {
            result = ERROR;
        }
    }
    public:
    Request(){}

    Request(std::string input){
        const std::vector<std::string> parsed_input = parse_string(input, " ");
        if(parsed_input.size() != 3){
            result = ERROR;
            return;
        }
        get_method(parsed_input[0]);
        path = parsed_input[1];  
    }

    ParseResult validate(){
        return result;
    }
};

class Header{
    std::string content_type;
    std::string accept;
    int content_length;
    std::string user_agent;
    ParseResult result = OK;

    void parse_input_field(std::string input_field){
        std::vector<std::string> input_pair = parse_string(input_field, ": ");
        if(input_pair.size() < 2){result = ERROR; return;}
        if(input_pair[0] == "User-Agent"){
            user_agent = input_pair[1];
        }
        if(input_pair[0] == "Content-Length"){
            try{content_length = std::stoi(input_pair[1]);}
            catch(std::invalid_argument const& ex){
                result = ERROR;
            }
        }
    }

    public:
     Header(std::vector<std::string> input_fields){
        for(std::string input_field: input_fields){
            parse_input_field(input_field);
        }
        std::cout << user_agent << "\n";
        std::cout << content_length << "\n";
    }
    Header(){}

    ParseResult validate(){
        return result;
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

        request = Request(parsed_input[0]);

        std::vector<std::string> header_input;
        bool header_end_encountered = false;
        for (size_t i = 1; i < parsed_input.size(); i++){
            std::cout << parsed_input[i] << "\n";
            if(header_end_encountered){
                body.append(parsed_input[i]);
            }
            if(parsed_input[i] == ""){
                header_end_encountered = true;
            }
            if(!header_end_encountered){
                header_input.push_back(parsed_input[i]);
            }
        }
        header = Header(header_input);
        
    }

    ParseResult validate(){
        if (request.validate() == OK && header.validate() == OK) return OK;
        return ERROR;
    }
};

int main(){
    const std::string input = "GET /index.html HTTP/1.0\nUser-Agent: Mozilla/5.0\nContent-Length: 463\nHost: www.example.com\n\n(Optional request body with a bunch of stuff)";
    HTTPReceiver test_receiver = HTTPReceiver(input);
    User user = User(1, "TestUser");

    std::cout << user.to_string() << "\n";

    User secondaryUser = User("{\"id\":1,\"time_of_creation\":1738155433,\"username\":\"TestUser\"}");
    std::cout << secondaryUser.username << "\n";

    std::cout << test_receiver.validate() << "\n";
    return 0;
}