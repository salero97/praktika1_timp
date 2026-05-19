#pragma once
#include <vector>
#include <optional>
#include "business/Entities.hpp"

using namespace std;

class InMemoryRepository {
public:
    InMemoryRepository();

    void addIncident(const Incident& incident);
    vector<Incident> getAllIncidents() const;
    optional<Incident> getIncidentById(int id) const;
    bool updateIncidentStatus(int id, const string& status);
    int generateIncidentId();

    bool closeIncident(int id, const string& finalStatus);
    vector<Incident> getClosedIncidents() const;

private:
    vector<Incident> incidents;
    vector<Incident> closedIncidents;
    int nextIncidentId;
};