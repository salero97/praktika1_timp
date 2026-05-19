#pragma once
#include "business/FireSafetyService.hpp"

using namespace std;

class ConsoleUI {
public:
    explicit ConsoleUI(FireSafetyService& service);
    void run();

private:
    void showMenu() const;
    bool authorize();

    FireSafetyService& service;
};