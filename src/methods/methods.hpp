#include <variant>

using source = std::variant<User>;

struct GET{};
struct POST{};
struct HEAD{};
struct PUT{};
struct DELETE{};