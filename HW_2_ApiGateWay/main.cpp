#include <crow.h>
#include "ApiGateway/FilesStoringService/StoringGateWay.h"
#include "ApiGateway/FileAnalysisService/AnalysisGateway.h"

int main() {
    crow::SimpleApp app;


    StoringGateway(app);
    AnalysisGateway(app);

    app.port(5000).multithreaded().run();
    return 0;
}
