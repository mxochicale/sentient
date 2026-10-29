#include <iostream>
#include <string_view>
#include <optional>
#include <variant>
#include <vector>
#include <numeric>

int main() {
    std::string_view name = "GCC 7";
    std::optional<int> answer = 42;
    std::variant<int, std::string> v = std::string("variant");

    std::vector<int> nums{1, 2, 3, 4, 5};
    int sum = std::accumulate(nums.begin(), nums.end(), 0);

    std::cout << "compiler: " << __VERSION__ << "\n";
    std::cout << "C++ standard: " << __cplusplus << "\n";
    std::cout << "name = " << name << "\n";
    std::cout << "answer = " << *answer << "\n";
    std::cout << "variant holds: " << std::get<std::string>(v) << "\n";
    std::cout << "sum = " << sum << "\n";
    return 0;
}
