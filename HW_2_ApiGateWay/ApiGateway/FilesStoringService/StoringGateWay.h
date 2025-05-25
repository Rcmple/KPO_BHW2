//
// Created by imaginelipa on 5/24/25.
//

#ifndef STORING_GATEWAY_H
#define STORING_GATEWAY_H
#include <crow.h>
#include <cpr/cpr.h>

void StoringGateway(crow::SimpleApp &app) {
    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::POST)
    ([](const crow::request &req) {
        auto res = cpr::Post(
            cpr::Url{"http://storing:5001/store"},
            cpr::Body{req.body},
            cpr::Header{{"Content-Type", req.get_header_value("Content-Type")}}
        );

        return crow::response(res.status_code, res.text);
    });

    CROW_ROUTE(app, "/get/<int>/").methods(crow::HTTPMethod::GET)
    ([](const crow::request &req, int id) {
        auto res = cpr::Get(
            cpr::Url{"http://storing:5001/get/" + std::to_string(id)},
            cpr::Header{{"Content-Type", req.get_header_value("Content-Type")}}
        );
        if (res.status_code == 200) {
            return crow::response(res.status_code, res.text);
        }
        return crow::response(res.status_code, "File not found");
    });
}
#endif //STORING_GATEWAY_H
