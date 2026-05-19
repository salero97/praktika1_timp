#pragma once
#include <string>

using namespace std;

class Sensor {
public:
    Sensor(int id, const string& type, const string& zone, double value = 0.0);

    int getId() const;
    string getType() const;
    string getZone() const;
    double getValue() const;
    void setValue(double value);

private:
    int sensorId;
    string sensorType;
    string zone;
    double currentValue;
};

class Incident {
public:
    Incident(int id,
             const string& zone,
             const string& description,
             const string& dangerLevel,
             const string& status = "Новый");

    int getId() const;
    string getZone() const;
    string getDescription() const;
    string getDangerLevel() const;
    string getStatus() const;

    void setStatus(const string& status);
    string toString() const;

private:
    int incidentId;
    string zone;
    string description;
    string dangerLevel;
    string status;
};

class Object {
public:
    Object(int id, const string& name, const string& zone);

    int getId() const;
    string getName() const;
    string getZone() const;

private:
    int objectId;
    string name;
    string zone;
};