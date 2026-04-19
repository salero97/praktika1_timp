#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

enum class SensorType { SMOKE, HEAT, GAS };
enum class SensorStatus { NORM, WARNING, ALARM };
enum class EventStatus { ACTIVE, LOCALIZED, CLOSED };
enum class Severity { INFO, WARNING, CRITICAL };
enum class UserRole { DISPATCHER, INSPECTOR, ADMIN };

struct FireSensor {
    int id;
    SensorType type;
    string location;
    double currentValue;
    SensorStatus status;
    double warningThreshold;
    double criticalThreshold;
};

struct FireEvent {
    int id;
    string zone;
    Severity severity;
    EventStatus status;
    int triggeredSensorId;
    string timestamp;
};

struct User {
    int id;
    string fullName;
    UserRole role;
    string department;
    string passwordHash;
};

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool readInt(const string& prompt, int& value) {
    cout << prompt;
    if (!(cin >> value)) {
        clearInput();
        cout << "Некорректный ввод. Ожидалось целое число.\n";
        return false;
    }
    return true;
}

bool readDouble(const string& prompt, double& value) {
    cout << prompt;
    if (!(cin >> value)) {
        clearInput();
        cout << "Некорректный ввод. Ожидалось число.\n";
        return false;
    }
    return true;
}

class SensorRepository {
    vector<FireSensor> sensors;
public:
    void save(const FireSensor& s) {
        sensors.push_back(s);
    }
    FireSensor* findById(int id) {
        for (auto& s : sensors)
            if (s.id == id) return &s;
        return nullptr;
    }
    vector<FireSensor> getAll() {
        return sensors;
    }
    void updateStatus(int id, SensorStatus newStatus) {
        auto* s = findById(id);
        if (s) s->status = newStatus;
    }
};

class FireEventRepository {
    vector<FireEvent> events;
    int nextId = 1;
public:
    void save(FireEvent& e) {
        e.id = nextId++;
        events.push_back(e);
    }
    FireEvent* findById(int id) {
        for (auto& e : events)
            if (e.id == id) return &e;
        return nullptr;
    }
    vector<FireEvent> getAll() {
        return events;
    }
    void updateStatus(int id, EventStatus newStatus) {
        auto* e = findById(id);
        if (e) e->status = newStatus;
    }
};

class UserRepository {
    vector<User> users;
public:
    void save(const User& u) {
        users.push_back(u);
    }
    User* findByName(const string& name) {
        for (auto& u : users)
            if (u.fullName == name) return &u;
        return nullptr;
    }
    vector<User> getAll() {
        return users;
    }
};

class FireEventService {
    FireEventRepository& eventRepo;
    SensorRepository& sensorRepo;
public:
    FireEventService(FireEventRepository& er, SensorRepository& sr)
        : eventRepo(er), sensorRepo(sr) {}

    Severity calculateRisk(double value, double warnThr, double critThr) {
        if (value >= critThr) return Severity::CRITICAL;
        if (value >= warnThr) return Severity::WARNING;
        return Severity::INFO;
    }

    string severityToString(Severity s) {
        switch (s) {
            case Severity::CRITICAL: return "CRITICAL";
            case Severity::WARNING:  return "WARNING";
            default:                 return "INFO";
        }
    }

    string statusToString(EventStatus st) {
        switch (st) {
            case EventStatus::ACTIVE:     return "ACTIVE";
            case EventStatus::LOCALIZED:  return "LOCALIZED";
            case EventStatus::CLOSED:     return "CLOSED";
        }
        return "";
    }

    void registerEvent(const string& zone, int sensorId, double value) {
        auto* sensor = sensorRepo.findById(sensorId);
        if (!sensor) {
            cout << "Датчик с ID " << sensorId << " не найден. "
                 << "(Сначала посмотри список датчиков в пункте 1.)\n";
            return;
        }
        Severity sev = calculateRisk(value,
                                     sensor->warningThreshold,
                                     sensor->criticalThreshold);
        FireEvent ev;
        ev.zone = zone;
        ev.severity = sev;
        ev.status = EventStatus::ACTIVE;
        ev.triggeredSensorId = sensorId;
        ev.timestamp = "2026-04-18 19:00";
        eventRepo.save(ev);
        cout << "Инцидент создан. Зона: " << zone
             << ", Уровень: " << severityToString(sev) << "\n";
        if (sev == Severity::CRITICAL) {
            cout << "[Информация] Критический уровень. "
                 << "Лифты заблокированы, шлюзы эвакуации открыты.\n";
            cout << "[Информация] Оповещение МЧС отправлено.\n";
        }
    }

    void escalateEvent(int eventId) {
        auto* ev = eventRepo.findById(eventId);
        if (!ev) {
            cout << "Инцидент с таким ID не найден.\n";
            return;
        }
        if (ev->severity == Severity::WARNING) {
            ev->severity = Severity::CRITICAL;
            cout << "Инцидент #" << eventId
                 << " эскалирован до CRITICAL.\n";
        } else {
            cout << "Эскалация доступна только из WARNING в CRITICAL.\n";
        }
    }

    void closeEvent(int eventId) {
        auto* ev = eventRepo.findById(eventId);
        if (!ev) {
            cout << "Инцидент с таким ID не найден.\n";
            return;
        }
        eventRepo.updateStatus(eventId, EventStatus::CLOSED);
        cout << "Инцидент #" << eventId << " закрыт.\n";
    }

    vector<FireEvent> getActiveEvents() {
        auto all = eventRepo.getAll();
        vector<FireEvent> result;
        for (auto& e : all)
            if (e.status == EventStatus::ACTIVE) result.push_back(e);
        return result;
    }

    vector<FireEvent> getAllEvents() {
        return eventRepo.getAll();
    }
};

class AuthService {
    UserRepository& userRepo;
public:
    AuthService(UserRepository& ur) : userRepo(ur) {}

    User* authenticate(const string& name, const string& password) {
        auto* u = userRepo.findByName(name);
        if (u && u->passwordHash == password) return u;
        return nullptr;
    }
};

void printSeparator() {
    cout << "--------------------------------------------\n";
}

void showSensorList(SensorRepository& repo) {
    printSeparator();
    cout << "Список датчиков (пример: ID 1, Этаж 2, каб. 201)\n";
    auto all = repo.getAll();
    if (all.empty()) {
        cout << "Датчиков нет.\n";
        printSeparator();
        return;
    }
    for (auto& s : all) {
        string statusStr;
        switch (s.status) {
            case SensorStatus::NORM:    statusStr = "НОРМА"; break;
            case SensorStatus::WARNING: statusStr = "ПРЕДУПРЕЖДЕНИЕ"; break;
            case SensorStatus::ALARM:   statusStr = "ТРЕВОГА"; break;
        }
        cout << "ID: " << s.id
             << " | Зона: " << s.location
             << " | Статус: " << statusStr
             << " | Значение: " << s.currentValue << "\n";
    }
    printSeparator();
}

void showActiveEvents(FireEventService& service) {
    printSeparator();
    auto events = service.getActiveEvents();
    cout << "Активные инциденты "
         << "(используй ID из этого списка в пунктах 4 и 5).\n";
    if (events.empty()) {
        cout << "Активных инцидентов нет.\n";
        printSeparator();
        return;
    }
    for (auto& e : events) {
        string sevStr;
        switch (e.severity) {
            case Severity::CRITICAL: sevStr = "CRITICAL"; break;
            case Severity::WARNING:  sevStr = "WARNING"; break;
            default:                 sevStr = "INFO"; break;
        }
        cout << "ID: " << e.id
             << " | Зона: " << e.zone
             << " | Уровень: " << sevStr
             << " | Датчик: " << e.triggeredSensorId << "\n";
    }
    printSeparator();
}

void showAllEvents(FireEventService& service) {
    printSeparator();
    cout << "Все инциденты (включая закрытые).\n";
    auto events = service.getAllEvents();
    if (events.empty()) {
        cout << "Инцидентов нет.\n";
        printSeparator();
        return;
    }
    for (auto& e : events) {
        string sevStr;
        switch (e.severity) {
            case Severity::CRITICAL: sevStr = "CRITICAL"; break;
            case Severity::WARNING:  sevStr = "WARNING"; break;
            default:                 sevStr = "INFO"; break;
        }
        string stStr;
        switch (e.status) {
            case EventStatus::ACTIVE:     stStr = "ACTIVE"; break;
            case EventStatus::LOCALIZED:  stStr = "LOCALIZED"; break;
            case EventStatus::CLOSED:     stStr = "CLOSED"; break;
        }
        cout << "ID: " << e.id
             << " | Зона: " << e.zone
             << " | Уровень: " << sevStr
             << " | Статус: " << stStr
             << " | Датчик: " << e.triggeredSensorId << "\n";
    }
    printSeparator();
}

void registerManualEvent(FireEventService& service, SensorRepository& sensorRepo) {
    printSeparator();
    cout << "Регистрация инцидента вручную.\n";
    cout << "(Сначала посмотри список датчиков в пункте 1 "
         << "и выбери существующий ID.)\n";

    string zone;
    int sensorId;
    double value;

    cout << "Введите зону (пример: Этаж2_Каб201): ";
    cin >> zone;

    if (!readInt("Введите ID датчика (пример: 1): ", sensorId))
        return;

    if (!readDouble("Введите текущее значение датчика "
                    "(пример: 75.5): ", value))
        return;

    auto* sensor = sensorRepo.findById(sensorId);
    if (!sensor) {
        cout << "Датчика с таким ID не существует. "
             << "Вернись в пункт 1 и посмотри ID.\n";
        return;
    }

    service.registerEvent(zone, sensorId, value);
}

void escalateEventMenu(FireEventService& service) {
    printSeparator();
    cout << "Эскалация инцидента.\n";
    cout << "(Сначала посмотри список активных инцидентов в пункте 2 "
         << "и выбери ID.)\n";
    int id;
    if (!readInt("Введите ID инцидента: ", id))
        return;
    service.escalateEvent(id);
}

void closeEventMenu(FireEventService& service) {
    printSeparator();
    cout << "Закрытие инцидента.\n";
    cout << "(Сначала посмотри список активных инцидентов в пункте 2 "
         << "и выбери ID.)\n";
    int id;
    if (!readInt("Введите ID инцидента: ", id))
        return;
    service.closeEvent(id);
}

void mainMenu(FireEventService& service, SensorRepository& sensorRepo) {
    int choice = -1;
    while (true) {
        printSeparator();
        cout << "Система пожарной безопасности\n";
        cout << "1. Просмотр датчиков "
             << "(ID, зона, значение)\n";
        cout << "2. Просмотр активных инцидентов "
             << "(только ACTIVE)\n";
        cout << "3. Зарегистрировать инцидент вручную "
             << "(указать зону, ID датчика, значение)\n";
        cout << "4. Эскалировать инцидент "
             << "(WARNING -> CRITICAL)\n";
        cout << "5. Закрыть инцидент "
             << "(перевести в CLOSED)\n";
        cout << "6. Просмотр всех инцидентов "
             << "(история)\n";
        cout << "0. Выход\n";
        cout << "Выбор (введите число, пример: 1): ";

        if (!(cin >> choice)) {
            clearInput();
            cout << "Некорректный ввод. "
                 << "Нужно ввести число от 0 до 6.\n";
            continue;
        }

        switch (choice) {
            case 1:
                showSensorList(sensorRepo);
                break;
            case 2:
                showActiveEvents(service);
                break;
            case 3:
                registerManualEvent(service, sensorRepo);
                break;
            case 4:
                escalateEventMenu(service);
                break;
            case 5:
                closeEventMenu(service);
                break;
            case 6:
                showAllEvents(service);
                break;
            case 0:
                cout << "Выход из системы.\n";
                return;
            default:
                cout << "Нет такого пункта меню. "
                     << "Введи число от 0 до 6.\n";
        }
    }
}

User* authMenu(AuthService& authService) {
    string name, password;
    printSeparator();
    cout << "Авторизация.\n";
    cout << "(Пример: логин dispatch, пароль gaga0148 "
         << "или admin / admin123.)\n";
    cout << "Имя пользователя: ";
    cin >> name;
    cout << "Пароль: ";
    cin >> password;
    User* user = authService.authenticate(name, password);
    if (user) {
        cout << "Добро пожаловать, " << user->fullName << ".\n";
    } else {
        cout << "Неверные данные. Доступ запрещён.\n";
    }
    return user;
}

int main() {
    SensorRepository sensorRepo;
    FireEventRepository eventRepo;
    UserRepository userRepo;

    FireSensor s1 = {1, SensorType::SMOKE, "Этаж 2, каб. 201",
                     0.0, SensorStatus::NORM, 30.0, 70.0};
    FireSensor s2 = {2, SensorType::HEAT,  "Этаж 1, серверная",
                     0.0, SensorStatus::NORM, 60.0, 90.0};
    sensorRepo.save(s1);
    sensorRepo.save(s2);

    User admin = {1, "admin", UserRole::ADMIN,
                  "Диспетчерская", "admin123"};
    User dispatch = {2, "dispatch", UserRole::DISPATCHER,
                     "Диспетчерская", "gaga0148"};
    userRepo.save(admin);
    userRepo.save(dispatch);

    FireEventService eventService(eventRepo, sensorRepo);
    AuthService authService(userRepo);

    User* currentUser = authMenu(authService);
    if (currentUser) {
        mainMenu(eventService, sensorRepo);
    }

    return 0;
}