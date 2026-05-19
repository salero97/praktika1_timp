#pragma once
#include <vector>
#include <string>
#include <optional>
#include "data/Repository.hpp"

using namespace std;

class FireSafetyService {
public:
    explicit FireSafetyService(InMemoryRepository& repository);

    string analyzeSensorData(double temperature, double smokeLevel) const;
    Incident registerIncident(const string& zone,
                              const string& description,
                              double temperature,
                              double smokeLevel);

    vector<Incident> getIncidents() const;
    optional<Incident> findIncident(int id) const;
    bool changeIncidentStatus(int id, const string& status);

    bool closeIncident(int id, const string& finalStatus);
    vector<Incident> getClosedIncidents() const;

private:
    InMemoryRepository& repository;
};