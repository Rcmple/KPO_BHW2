#include <iostream>
#include <crow.h>
#include "Analysing/Analysing.h"
int main() {
    crow::SimpleApp app;

    AnalysingGateway(app);

    app.port(5002).multithreaded().run();
}