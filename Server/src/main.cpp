#include "pch.h"
int main(int argc, char *argv[]) {
    auto app = Application::GetInstance();

    app->Run([]() {});
    app->ShutDown();

    return 0;
}