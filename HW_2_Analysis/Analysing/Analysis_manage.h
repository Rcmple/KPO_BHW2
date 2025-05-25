//
// Created by imaginelipa on 5/25/25.
//

#ifndef ANALYSIS_MANAGE_H
#define ANALYSIS_MANAGE_H
#include <pqxx/pqxx>
#include <iostream>
#include <regex>

bool find_analysis(int id) {
    try {
        pqxx::connection conn("dbname=reportdb user=reportuser password=hagsfdha host=postgres");
        pqxx::work txn(conn);

        pqxx::result r = txn.exec_params(
            "SELECT 1 FROM file_analysis WHERE file_id = $1 LIMIT 1",
            id
        );

        return !r.empty();

    } catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
        return false;
    }
}

void add_analysis(int id) {
    try {
        pqxx::connection conn("dbname=reportdb user=reportuser password=hagsfdha host=postgres");
        pqxx::work txn(conn);

        pqxx::result r = txn.exec_params("SELECT path FROM files WHERE id = $1", id);
        if (r.empty()) {
            std::cerr << "File not found with ID: " << id << std::endl;
            return;
        }

        std::string path = "/app/" + r[0]["path"].as<std::string>();
        std::cout << "Analyzing file: " << path << std::endl;

        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Cannot open file: " << path << std::endl;
            return;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();

        int characters = static_cast<int>(content.size());
        size_t pos = 0;
        int paragraphs = 1;
        while ((pos = content.find("\n\n", pos)) != std::string::npos) {
            ++paragraphs;
            pos += 2;
        }

        std::regex word_regex(R"(\b\w+\b)");
        int words = std::distance(
            std::sregex_iterator(content.begin(), content.end(), word_regex),
            std::sregex_iterator()
        );

        txn.exec_params(
            "INSERT INTO file_analysis (file_id, paragraphs, words, characters) VALUES ($1, $2, $3, $4)",
            id, paragraphs, words, characters
        );

        txn.commit();

    } catch (const std::exception &e) {
        std::cerr << "Database or file error: " << e.what() << std::endl;
    }
}
#endif //ANALYSIS_MANAGE_H
