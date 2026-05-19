#include "Entities.hpp"
#include <sstream>

using namespace std;

Sensor::Sensor(int id, const string& type, const string& zone, double value)
    : sensorId(id), sensorType(type), zone(zone), currentValue(value) {}

int Sensor::getId() const {
    return sensorId;
}

string Sensor::getType() const {
    return sensorType;
}

string Sensor::getZone() const {
    return zone;
}

double Sensor::getValue() const {
    return currentValue;
}

void Sensor::setValue(double value) {
    currentValue = value;
}

Incident::Incident(int id,
                   const string& zone,
                   const string& description,
                   const string& dangerLevel,
                   const string& status)
    : incidentId(id),
      zone(zone),
      description(description),
      dangerLevel(dangerLevel),
      status(status) {}

int Incident::getId() const {
    return incidentId;
}

string Incident::getZone() const {
    return zone;
}

string Incident::getDescription() const {
    return description;
}

string Incident::getDangerLevel() const {
    return dangerLevel;
}

string Incident::getStatus() const {
    return status;
}

void Incident::setStatus(const string& newStatus) {
    status = newStatus;
}

string Incident::toString() const {
    ostringstream os;
    os << "[" << incidentId << "] "
       << "Зона: " << zone << ", "
       << "Уровень: " << dangerLevel << ", "
       << "Статус: " << status << ", "
       << "Описание: " << description;
    return os.str();
}

Object::Object(int id, const string& name, const string& zone)
    : objectId(id), name(name), zone(zone) {}

int Object::getId() const {
    return objectId;
}

string Object::getName() const {
    return name;
}

string Object::getZone() const {
    return zone;
}