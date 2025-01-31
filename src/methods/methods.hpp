#pragma once
#include <variant>
#include "../main.cpp"

using ResourceType = std::variant<User>;

struct GET{
    std::string execute(ResourceType& resource, const std::string& json){
        return " ";
    }
};

struct POST {
    std::string execute(ResourceType& resource, const std::string& json){
        // Validate JSON -> Convert to ResourceType -> Perform Action

        std::visit([&](auto& resource){
            resource.data.from_string(json);
            resource.populate_incomplete_data();
            resource.save_current_to_file();
            }, resource);



        /* Steps: 
            Check if it already exists ?? (how should I do this - incomplete data, etc? I guess the resource should decide?)
            Insert into database
            Get confirmation. If it worked, return a complete object in stringformat

        */

        // if (!resource) {
        //     return "400 Bad Request: Invalid JSON for resource";
        // }

        // // Simulate storing the resource
        // std::cout << "Storing resource: " << resource->to_string() << std::endl;
        // return "201 Created";
        return "";
    }
};

struct HEAD{
    std::string execute(ResourceType& resource, const std::string& json){
        return " ";
    }
};
struct PUT{
    std::string execute(ResourceType& resource, const std::string& json){
        return " ";
    }
};
struct DELETE{
    std::string execute(ResourceType& resource, const std::string& json){
        return " ";
    }
};