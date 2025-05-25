//
// Created by imaginelipa on 5/25/25.
//

#ifndef FIND_FILE_H
#define FIND_FILE_H
#include <pqxx/pqxx>
#include <string>

bool find_file(const std::string& hash) {
    try {
        pqxx::connection conn("dbname=reportdb user=reportuser password=hagsfdha host=postgres");
        pqxx::work txn(conn);

        pqxx::result r = txn.exec_params("SELECT 1 FROM files WHERE hash = $1 LIMIT 1", hash);

        return !r.empty();
    } catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
        return false;
    }
}

void add_file(const std::string& hash, const std::string& path) {
    try {
        pqxx::connection conn("dbname=reportdb user=reportuser password=hagsfdha host=postgres");
        pqxx::work txn(conn);

        std::filesystem::path source = path;
        std::filesystem::path dest_dir = "uploaded_files";
        std::filesystem::path destination = dest_dir / source.filename();

        std::string filename = dest_dir.filename();

        txn.exec_params(
            "INSERT INTO files (name, path, hash) VALUES ($1, $2, $3)",
            filename,
            destination.string(),
            hash
        );

        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Database insert error: " << e.what() << std::endl;
    }
}

std::string send_file(const int id) {
    try {
        pqxx::connection conn("dbname=reportdb user=reportuser password=hagsfdha host=postgres");
        pqxx::work txn(conn);

        pqxx::result r = txn.exec_params("SELECT path FROM files WHERE id = $1", id);
        if (r.empty()) {
            std::cerr << "File not found with ID: " << id << std::endl;
            return "";
        }

        std::string file_path = r[0]["path"].as<std::string>();

        std::ifstream file(file_path, std::ios::binary);
        if (!file) {
            std::cerr << "Cannot open file: " << file_path << std::endl;
            return "";
        }
        return file_path;

    } catch (const std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
        return "";
    }
}

#endif //FIND_FILE_H
