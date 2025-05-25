//
// Created by imaginelipa on 5/25/25.
//

#ifndef UPLOADING_H
#define UPLOADING_H
#include <crow.h>
#include <cpr/cpr.h>
#include <filesystem>
#include <random>
#include "hash_file.h"
#include "file_manage.h"

void UploadingGateway(crow::SimpleApp &app) {
    CROW_ROUTE(app, "/store").methods(crow::HTTPMethod::POST)
    ([](const crow::request &req) {
        std::string filename;
        while (true) {
            std::mt19937 mt{std::random_device{}()};
            std::uniform_int_distribution<int> dist(1, 1000000);
            filename = "temporary_files/file_" + std::to_string(dist(mt)) + ".txt";
            if (!std::filesystem::exists(filename)) {
                std::filesystem::create_directories("temporary_files");
                break;
            }
        }


        std::istringstream stream(req.body);
        std::vector<std::string> lines;
        std::string line;

        while (std::getline(stream, line)) {
            lines.push_back(line);
        }

        if (lines.size() <= 5) {
            return crow::response(400, "Not enough lines in body");
        }

        std::ofstream out(filename, std::ios::binary);
        if (!out.is_open()) {
            return crow::response(500, "Cannot open file for writing");
        }

        for (size_t i = 4; i < lines.size() - 1; ++i) {
            out << lines[i] << "\n";
        }

        out.close();

        std::string hash_of_file = sha256_file(filename);
        std::cout << "Hash of file: " << hash_of_file << std::endl;

        if (!find_file(hash_of_file)) {
            add_file(hash_of_file, filename);
            std::filesystem::path source = filename;
            std::filesystem::path dest_dir = "uploaded_files";
            std::filesystem::path destination = dest_dir / source.filename();

			std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing);
			std::filesystem::remove(source);
        } else {
            std::filesystem::remove(filename);
            return crow::response(409, "File already exists with the same hash");
        }

        return crow::response(200, "File uploaded successfully: " + filename);
    });

    CROW_ROUTE(app, "/get/<int>/").methods(crow::HTTPMethod::GET)
    ([](const crow::request &req, int id) {
        std::string file_path = send_file(id);
        if (file_path.empty()) {
            return crow::response(404, "File not found");
        }

        std::ifstream file(file_path, std::ios::binary);
        if (!file) {
            return crow::response(500, "Cannot open file for reading");
        }
        std::cout << "File opened successfully: " << file_path << std::endl;

        std::ostringstream oss;
        oss << file.rdbuf();
        std::cout << "File content: " << oss.str() << std::endl;
        return crow::response(200, oss.str());
    });

	CROW_ROUTE(app, "/test").methods(crow::HTTPMethod::GET)([](){
    	return "Test OK";
	});
}
#endif //UPLOADING_H
