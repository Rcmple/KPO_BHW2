//
// Created by imaginelipa on 5/24/25.
//
#ifndef ANALYSIS_GATEWAY_H
#define ANALYSIS_GATEWAY_H
#include <crow.h>

void AnalysisGateway(crow::SimpleApp &app) {
    CROW_ROUTE(app, "/get_analysis/<int>/").methods(crow::HTTPMethod::GET)
    ([](const crow::request &req, int id) {
        auto res = cpr::Get(
            cpr::Url{"http://analysis:5002/get_analysis/" + std::to_string(id)},
            cpr::Header{{"Content-Type", req.get_header_value("Content-Type")}}
        );
        if (res.status_code == 200) {
            return crow::response(res.status_code, res.text);
        }
        return crow::response(res.status_code, "Something went wrong");
    });
}

#endif //ANALYSIS_GATEWAY_H

