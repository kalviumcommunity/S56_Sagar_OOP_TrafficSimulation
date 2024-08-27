#include <iostream>
#include <vector>
#include <string>
#include <iomanip> 

using namespace std;

enum class LightState { RED, YELLOW, GREEN };

class Vehicle {
public:
    Vehicle() : name("Unknown"), roadIndex(0), isEmergency(false) {}

    Vehicle(const string& name, int roadIndex, bool isEmergency = false)
        : name(name), roadIndex(roadIndex), isEmergency(isEmergency) {}

    void move() {
        cout << setw(15) << left << name << " is moving on road " << roadIndex + 1 << ".\n";
    }

    void slowDown() {
        cout << setw(15) << left << name << " is slowing down on road " << roadIndex + 1 << ".\n";
    }

    void stop() {
        if (!isEmergency) {
            cout << setw(15) << left << name << " has stopped on road " << roadIndex + 1 << ".\n";
        }
    }

    bool isEmergencyVehicle() const {
        return isEmergency;
    }

    int getRoadIndex() const {
        return roadIndex;
    }

private:
    string name;
    int roadIndex;
    bool isEmergency;
};

class TrafficLight {
public:
    TrafficLight() : state(LightState::RED) {}

    void setGreen() {
        state = LightState::GREEN;
    }

    void setYellow() {
        state = LightState::YELLOW;
    }

    void setRed() {
        state = LightState::RED;
    }

    LightState getState() const {
        return state;
    }

    string getStateString() const {
        switch (state) {
            case LightState::GREEN: return "GREEN";
            case LightState::YELLOW: return "YELLOW";
            case LightState::RED: return "RED";
            default: return "UNKNOWN";
        }
    }

private:
    LightState state;
};

class TrafficSimulation {
public:
    TrafficSimulation(int numVehicles, int numRoads)
        : numVehicles(numVehicles), numRoads(numRoads) {
        vehicles = new Vehicle[numVehicles];
        trafficLights.resize(numRoads);
    }

    ~TrafficSimulation() {
        delete[] vehicles;
    }

    void addVehicle(const Vehicle& vehicle, int index) {
        if (index < numVehicles) {
            vehicles[index] = vehicle;
        }
    }

    void simulateTraffic() {
        for (int i = 0; i < numRoads; ++i) {
            cout << "Traffic light on road " << i + 1 << " is " << trafficLights[i].getStateString() << ".\n";
        }

        for (int i = 0; i < numVehicles; ++i) {
            if (vehicles[i].isEmergencyVehicle()) {
                trafficLights[vehicles[i].getRoadIndex()].setGreen();
                vehicles[i].move();
            } else {
                vehicles[i].stop();
            }
        }
    }

private:
    int numVehicles;
    int numRoads;
    Vehicle* vehicles;
    vector<TrafficLight> trafficLights;
};

int main() {
    TrafficSimulation simulation(3, 3);

    Vehicle car("Car", 0);
    Vehicle bike("Bike", 1);
    Vehicle ambulance("Ambulance", 2, true);

    simulation.addVehicle(car, 0);
    simulation.addVehicle(bike, 1);
    simulation.addVehicle(ambulance, 2);

    simulation.simulateTraffic();

    return 0;
}
