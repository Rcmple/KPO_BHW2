//
// Created by imaginelipa on 5/25/25.
//

#ifndef ANALYSING_H
#define ANALYSING_H
#include <crow.h>
#include <cpr/cpr.h>
#include <filesystem>

#include "Analysis_manage.h"

void AnalysingGateway(crow::SimpleApp &app) {
    CROW_ROUTE(app, "/get_analysis/<int>/").methods(crow::HTTPMethod::GET)
    ([](const crow::request &req, int id) {
        if (!find_analysis(id)) {
            add_analysis(id);
        }

        try {
            pqxx::connection conn("dbname=reportdb user=reportuser password=hagsfdha host=postgres");
            pqxx::work txn(conn);

            pqxx::result r = txn.exec_params(
                "SELECT paragraphs, words, characters FROM file_analysis WHERE file_id = $1",
                id
            );

            if (r.empty()) {
                return crow::response(500, "Analysis record missing");
            }

            int paragraphs = r[0]["paragraphs"].as<int>();
            int words = r[0]["words"].as<int>();
            int characters = r[0]["characters"].as<int>();

            std::ostringstream json;
            json << "{"
                 << "\"paragraphs\": " << paragraphs << ", "
                 << "\"words\": " << words << ", "
                 << "\"characters\": " << characters
                 << "}";

            return crow::response(200, json.str());

        } catch (const std::exception& e) {
            return crow::response(500, std::string("Database error: ") + e.what());
        }
    });
}

#endif //ANALYSING_H
