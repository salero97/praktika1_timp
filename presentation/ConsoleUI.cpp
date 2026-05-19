#include "ConsoleUI.hpp"
#include <iostream>
#include <limits>
#include <set>

using namespace std;

static const set<string> ALLOWED_ZONES = {
    "Этаж 1",
    "Этаж 2",
    "Этаж 3",
    "Комната 101",
    "Комната 102",
    "Комната 201"
};

ConsoleUI::ConsoleUI(FireSafetyService& service) : service(service) {}

void ConsoleUI::showMenu() const {
    cout << "\nСистема пожарной безопасности" << endl;
    cout << "1. Зарегистрировать инцидент" << endl;
    cout << "2. Показать все инциденты" << endl;
    cout << "3. Найти инцидент по id" << endl;
    cout << "4. Изменить статус инцидента" << endl;
    cout << "5. Закрыть инцидент" << endl;
    cout << "6. Показать архив закрытых инцидентов" << endl;
    cout << "0. Выход" << endl;
}

bool ConsoleUI::authorize() {
    const string validLogin = "admin";
    const string validPassword = "admin123";

    while (true) {
        string login;
        string password;

        cout << "Авторизация" << endl;
        cout << "Введите логин (например, admin): ";
        cin >> login;

        cout << "Введите пароль (например, admin123): ";
        cin >> password;

        if (login == validLogin && password == validPassword) {
            cout << "Авторизация выполнена успешно" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return true;
        } else {
            cout << "Ошибка авторизации. Неверный логин или пароль." << endl;
            cout << "Повторите ввод." << endl;
        }
    }
}

void ConsoleUI::run() {
    if (!authorize()) {
        return;
    }

    while (true) {
        showMenu();
        cout << "Выберите действие: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            string zone;
            string description;
            double temperature;
            double smokeLevel;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            while (true) {
                cout << "Введите зону (например, Этаж 1, Этаж 2, Этаж 3, Комната 101): ";
                getline(cin, zone);

                if (ALLOWED_ZONES.count(zone) > 0) {
                    break;
                } else {
                    cout << "Ошибка: зона недопустима. Используйте одну из предустановленных зон." << endl;
                }
            }

            cout << "Введите описание события (например, запах дыма в коридоре): ";
            getline(cin, description);

            cout << "Введите температуру в градусах Цельсия (например, 20, 60, 80): ";
            cin >> temperature;

            cout << "Введите уровень дыма в процентах (0–100): ";
            cin >> smokeLevel;

            Incident incident = service.registerIncident(zone, description, temperature, smokeLevel);
            cout << "Инцидент зарегистрирован:" << endl;
            cout << incident.toString() << endl;

            cout << "Пояснение уровней: "
                 << "Низкий – температура < 50 и дым < 50; "
                 << "Средний – температура >= 50 или дым >= 50; "
                 << "Высокий – температура >= 70 или дым >= 80."
                 << endl;

        } else if (choice == 2) {
            auto incidents = service.getIncidents();
            if (!incidents.empty()) {
                for (const auto& incident : incidents) {
                    cout << incident.toString() << endl;
                }
            } else {
                cout << "Список инцидентов пуст" << endl;
            }

        } else if (choice == 3) {
            cout << "Введите id инцидента: ";
            int id;
            cin >> id;
            auto incident = service.findIncident(id);
            if (incident.has_value()) {
                cout << incident->toString() << endl;
            } else {
                cout << "Инцидент не найден" << endl;
            }

        } else if (choice == 4) {
            cout << "Введите id инцидента: ";
            int id;
            cin >> id;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string status;
            while (true) {
                cout << "Введите новый статус (норма/ожидание/локализован/критический): ";
                getline(cin, status);

                if (status == "норма" ||
                    status == "ожидание" ||
                    status == "локализован" ||
                    status == "критический") {
                    break;
                } else {
                    cout << "Ошибка: недопустимый статус. "
                         << "Используйте одно из значений: "
                         << "норма, ожидание, локализован, критический."
                         << endl;
                }
            }

            bool updated = service.changeIncidentStatus(id, status);
            if (updated) {
                cout << "Статус обновлён" << endl;
            } else {
                cout << "Инцидент не найден" << endl;
            }

        } else if (choice == 5) {
            cout << "Введите id инцидента для закрытия: ";
            int id;
            cin >> id;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string status;
            while (true) {
                cout << "Введите финальный статус при закрытии (норма/локализован): ";
                getline(cin, status);

                if (status == "норма" || status == "локализован") {
                    break;
                } else {
                    cout << "Ошибка: недопустимый статус. "
                         << "Используйте одно из значений: норма, локализован."
                         << endl;
                }
            }

            bool closed = service.closeIncident(id, status);
            if (closed) {
                cout << "Инцидент закрыт и перенесён в архив" << endl;
            } else {
                cout << "Инцидент не найден среди активных" << endl;
            }

        } else if (choice == 6) {
            auto closed = service.getClosedIncidents();
            if (!closed.empty()) {
                cout << "Архив закрытых инцидентов:" << endl;
                for (const auto& incident : closed) {
                    cout << incident.toString() << endl;
                }
            } else {
                cout << "Архив закрытых инцидентов пуст" << endl;
            }

        } else if (choice == 0) {
            cout << "Завершение работы" << endl;
            break;
        } else {
            cout << "Некорректный пункт меню" << endl;
        }
    }
}