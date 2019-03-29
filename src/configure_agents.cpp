//
// Created by david on 27/03/19.
//

#include "../include/configure_agents.h"
#include "../include/json.hpp"


#include <random>
#include <fstream>

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator &g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter random_element(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

std::vector<nlohmann::json> load_classes() {
    using json = nlohmann::json;

    // Period 1 JSON
    std::ifstream period1_file("../assets/period1.json");
    json period1_json;
    period1_file >> period1_json;

    // Period 2 JSON
    std::ifstream period2_file("../assets/period2.json");
    json period2_json;
    period2_file >> period2_json;

    // Period 3 JSON
    std::ifstream period3_file("../assets/period3.json");
    json period3_json;
    period3_file >> period3_json;

    // Period 4 JSON
    std::ifstream period4_file("../assets/period4.json");
    json period4_json;
    period4_file >> period4_json;

    // Period 5 JSON
    std::ifstream period5_file("../assets/period5.json");
    json period5_json;
    period5_file >> period5_json;

    return std::vector<json>{period1_json, period2_json, period3_json, period4_json, period5_json};
}

void place_student_in_class(std::string *student_period, int grade, int period) {
    using json = nlohmann::json;

    std::vector<json> classes = load_classes();
    std::string grade_id;

    switch (grade) {
        case 0:
            grade_id = "grade9";
            break;
        case 1:
            grade_id = "grade10";
            break;
        case 2:
            grade_id = "grade11";
            break;
        case 3:
            grade_id = "grade12";
            break;
        default:
            grade_id = "grade9";
            break;
    }

    *student_period = *random_element(classes[period - 1][grade_id].begin(), classes[period - 1][grade_id].end());
}
