#include <string>
#include <iostream>
#include <optional>
class Response{
    protected:
        std::string error_code;
        std::string response_message;
        std::string error_message;

        Response(const std::string& error_code, const std::string& message, const std::string& error_msg)
            : error_code(error_code), response_message(message), error_message(error_msg) {};
        

        public:
        virtual std::optional<std::string> handle() const = 0;
        virtual ~Response() = default;

        // Getters
        std::string getErrorCode() const { return error_code; }
        std::string getResponseMessage() const { return response_message; }
        std::string getErrorMessage() const { return error_message; }
};

class OK : public Response{
    public:
    OK() : Response("200", "OK", "No Error"){};
    std::optional<std::string> handle() const override {
        return {};
    }
};

class BadRequest : public Response {
    public:
    BadRequest(std::string error_message) : Response("400", "Bad Requset", std::move(error_message)) {}; 

    std::optional<std::string> handle() const override {
        return this->error_code + " " + this->response_message + "\nError message: " + this->error_message + "\n";
    }
};