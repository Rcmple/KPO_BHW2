#include <iostream>
#include <crow.h>

#include "Uploading/uploading_endpoints.h"

int main() {
    crow::SimpleApp app;

    UploadingGateway(app);

    app.port(5001).multithreaded().run();
    return 0;
}
