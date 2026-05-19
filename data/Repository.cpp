#include "Repository.hpp"

using namespace std;

InMemoryRepository::InMemoryRepository() : nextIncidentId(1) {}

void InMemoryRepository::addIncident(const Incident& incident) {
    incidents.push_back(incident);
}

vector<Incident> InMemoryRepository::getAllIncidents() const {
    return incidents;
}

optional<Incident> InMemoryRepository::getIncidentById(int id) const {
    for (const auto& incident : incidents) {
        if (incident.getId() == id) {
            return incident;
        }
    }
    return nullopt;
}

bool InMemoryRepository::updateIncidentStatus(int id, const string& status) {
    for (auto& incident : incidents) {
        if (incident.getId() == id) {
            incident.setStatus(status);
            return true;
        }
    }
    return false;
}

int InMemoryRepository::generateIncidentId() {
    int current = nextIncidentId;
    nextIncidentId++;
    return current;
}

bool InMemoryRepository::closeIncident(int id, const string& finalStatus) {
    for (auto it = incidents.begin(); it != incidents.end(); ++it) {
        if (it->getId() == id) {
            Incident closed = *it;
            closed.setStatus(finalStatus);
            closedIncidents.push_back(closed);
            incidents.erase(it);
            return true;
        }
    }
    return false;
}

vector<Incident> InMemoryRepository::getClosedIncidents() const {
    return closedIncidents;
}