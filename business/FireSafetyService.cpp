#include "FireSafetyService.hpp"

using namespace std;

FireSafetyService::FireSafetyService(InMemoryRepository& repo) : repository(repo) {}

string FireSafetyService::analyzeSensorData(double temperature, double smokeLevel) const {
    if (temperature >= 70.0 || smokeLevel >= 80.0) {
        return "Высокий";
    }
    if (temperature >= 50.0 || smokeLevel >= 50.0) {
        return "Средний";
    }
    return "Низкий";
}

Incident FireSafetyService::registerIncident(const string& zone,
                                             const string& description,
                                             double temperature,
                                             double smokeLevel) {
    string dangerLevel = analyzeSensorData(temperature, smokeLevel);
    int id = repository.generateIncidentId();
    Incident incident(id, zone, description, dangerLevel);
    repository.addIncident(incident);
    return incident;
}

vector<Incident> FireSafetyService::getIncidents() const {
    return repository.getAllIncidents();
}

optional<Incident> FireSafetyService::findIncident(int id) const {
    return repository.getIncidentById(id);
}

bool FireSafetyService::changeIncidentStatus(int id, const string& status) {
    return repository.updateIncidentStatus(id, status);
}

bool FireSafetyService::closeIncident(int id, const string& finalStatus) {
    return repository.closeIncident(id, finalStatus);
}

vector<Incident> FireSafetyService::getClosedIncidents() const {
    return repository.getClosedIncidents();
}