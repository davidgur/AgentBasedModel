//
// Created by david on 27/03/19.
//

#include "../include/configure_agents.h"
#include "../include/json.hpp"


#include <random>
#include <fstream>


template<typename I>
I random_element(I begin, I end) {
    auto n = (std::distance(begin, end));
    long divisor = (RAND_MAX + 1) / n;

    unsigned long k;
    do { k = std::rand() / divisor; } while (k >= n);

    std::advance(begin, k);
    return begin;
}

std::vector<Agent> place_students_in_classes(std::vector<Agent> agent_vector, int grade) {
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

    // NOTE: ALL GRADE 9'S HAVE EITHER PERIOD 3 OR PERIOD 4 LUNCH
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution_bool(0, 1);
    std::uniform_int_distribution<int> distribution_range(1, 5);

    std::map<std::string, int> period_1_class_sizes;
    std::map<std::string, int> period_2_class_sizes;
    std::map<std::string, int> period_3_class_sizes;
    std::map<std::string, int> period_4_class_sizes;
    std::map<std::string, int> period_5_class_sizes;

    std::vector<std::map<std::string, int>> class_sizes = {
            period_1_class_sizes,
            period_2_class_sizes,
            period_3_class_sizes,
            period_4_class_sizes,
            period_5_class_sizes
    };
    std::vector<json> periods = {period1_json, period2_json, period3_json, period4_json, period5_json};
    auto assigner = [&periods, &class_sizes](int grade, int period) -> std::string {
        std::string random_class = "LUNCH";
        std::string json_index;
        if (grade == 0) json_index = "grade9";
        else if (grade == 1) json_index = "grade10";
        else if (grade == 2) json_index = "grade11";
        else if (grade == 3) json_index = "grade12";


        while (true) {
            random_class = *random_element(periods[period - 1][json_index].begin(),
                                           periods[period - 1][json_index].end());
            bool all_classes_over = std::all_of(class_sizes[period - 1].begin(), class_sizes[period - 1].end(),
                                                [](auto it) { return it.second >= 27; });
            if (class_sizes[period - 1][random_class] < 27 || all_classes_over)
                break;
        }
        class_sizes[period - 1][random_class]++;
        return random_class;
    };

    if (grade == 0) {
        for (auto &student : agent_vector) {
            if (distribution_bool(generator)) {
                student.p3 = "LUNCH";
                student.p4 = assigner(0, 4);
            } else {
                student.p3 = assigner(0, 3);
                student.p4 = "LUNCH";
            }

            student.p1 = assigner(0, 1);
            student.p2 = assigner(0, 2);
            student.p5 = assigner(0, 5);
        }
    } else if (grade == 1) {
        for (auto &student : agent_vector) {
            if (distribution_bool(generator)) {
                student.p3 = "LUNCH";
                student.p4 = assigner(1, 4);
            } else {
                student.p3 = assigner(1, 3);
                student.p4 = "LUNCH";
            }

            student.p1 = assigner(1, 1);
            student.p2 = assigner(1, 2);
            student.p5 = assigner(1, 5);
        }
    } else if (grade == 2) {
        for (auto &student : agent_vector) {
            student.p1 = assigner(2, 1);
            student.p2 = assigner(2, 2);
            student.p3 = assigner(2, 3);
            student.p4 = assigner(2, 4);
            student.p5 = assigner(2, 5);

            // When's lunch?
            int lunch = distribution_range(generator);
            if (lunch == 1)
                student.p1 = "LUNCH";
            else if (lunch == 2)
                student.p2 = "LUNCH";
            else if (lunch == 3)
                student.p3 = "LUNCH";
            else if (lunch == 4)
                student.p4 = "LUNCH";
            else
                student.p5 = "LUNCH";
        }

    } else if (grade == 3) {
        for (auto &student : agent_vector) {
            student.p1 = assigner(3, 1);
            student.p2 = assigner(3, 2);
            student.p3 = assigner(3, 3);
            student.p4 = assigner(3, 4);
            student.p5 = assigner(3, 5);

            // When's lunch?
            int lunch = distribution_range(generator);
            if (lunch == 1)
                student.p1 = "LUNCH";
            else if (lunch == 2)
                student.p2 = "LUNCH";
            else if (lunch == 3)
                student.p3 = "LUNCH";
            else if (lunch == 4)
                student.p4 = "LUNCH";
            else
                student.p5 = "LUNCH";
        }

    }

    return agent_vector;
}


std::vector<Agent> create_fast_track(std::vector<Agent> agent_vector, int grade) {
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

    int number_of_fast_trackers = 12;

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<unsigned long> student_chooser(0, agent_vector.size());
    std::uniform_int_distribution<int> period_chooser(1, 5);

    unsigned long student_index = 0;

    for (int i = 0; i < number_of_fast_trackers; i++) {
        while (true) {
            student_index = student_chooser(generator);
            if (!agent_vector[student_index].fast_tracking) {
                break;
            }
        }
        // Identify student's lunch period
        int lunch = 1;
        if (agent_vector[student_index].p1 == "LUNCH") lunch = 1;
        else if (agent_vector[student_index].p2 == "LUNCH") lunch = 2;
        else if (agent_vector[student_index].p3 == "LUNCH") lunch = 3;
        else if (agent_vector[student_index].p4 == "LUNCH") lunch = 4;
        else if (agent_vector[student_index].p5 == "LUNCH") lunch = 5;

        // Pick which period to replace
        int period_to_replace;
        while (true) {
            period_to_replace = period_chooser(generator);
            if (period_to_replace != lunch)
                break;
        }

        if (grade == 1) {
            if (period_to_replace == 1)
                agent_vector[student_index].p1 = *random_element(period1_json["grade11"].begin(),
                                                                 period1_json["grade11"].end());
            else if (period_to_replace == 2)
                agent_vector[student_index].p2 = *random_element(period2_json["grade11"].begin(),
                                                                 period2_json["grade11"].end());
            else if (period_to_replace == 3)
                agent_vector[student_index].p3 = *random_element(period3_json["grade11"].begin(),
                                                                 period3_json["grade11"].end());
            else if (period_to_replace == 4)
                agent_vector[student_index].p4 = *random_element(period4_json["grade11"].begin(),
                                                                 period4_json["grade11"].end());
            else if (period_to_replace == 5)
                agent_vector[student_index].p5 = *random_element(period5_json["grade11"].begin(),
                                                                 period5_json["grade11"].end());

        } else if (grade == 2) {
            if (period_to_replace == 1)
                agent_vector[student_index].p1 = *random_element(period1_json["grade12"].begin(),
                                                                 period1_json["grade12"].end());
            else if (period_to_replace == 2)
                agent_vector[student_index].p2 = *random_element(period2_json["grade12"].begin(),
                                                                 period2_json["grade12"].end());
            else if (period_to_replace == 3)
                agent_vector[student_index].p3 = *random_element(period3_json["grade12"].begin(),
                                                                 period3_json["grade12"].end());
            else if (period_to_replace == 4)
                agent_vector[student_index].p4 = *random_element(period4_json["grade12"].begin(),
                                                                 period4_json["grade12"].end());
            else if (period_to_replace == 5)
                agent_vector[student_index].p5 = *random_element(period5_json["grade12"].begin(),
                                                                 period5_json["grade12"].end());
        }

        agent_vector[student_index].fast_tracking = true;
    }
    return agent_vector;
}