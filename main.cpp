#include <iostream>
#include "data/Repository.hpp"
#include "business/FireSafetyService.hpp"
#include "presentation/ConsoleUI.hpp"

using namespace std;

int main() {
    InMemoryRepository repository;
    FireSafetyService service(repository);
    ConsoleUI ui(service);
    ui.run();
    return 0;
}