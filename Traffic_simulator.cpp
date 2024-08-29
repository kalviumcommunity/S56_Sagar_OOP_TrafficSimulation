#include <iostream>
#include <vector>
#include <string>
#include <iomanip> 
using namespace std;

enum class LightState {
    RED,
    YELLOW,
    GREEN
};

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

    void setName(const string& name) {
        this->name = name;
    }

    void setRoadIndex(int roadIndex) {
        this->roadIndex = roadIndex;
    }

    void setIsEmergency(bool isEmergency) {
        this->isEmergency = isEmergency;
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
        vehicles = new Vehicle*[numVehicles];  // Array of pointers to Vehicle objects
        for (int i = 0; i < numVehicles; ++i) {
            vehicles[i] = new Vehicle();  // Allocate each Vehicle object
        }
        trafficLights = new TrafficLight*[numRoads];  // Array of pointers to TrafficLight objects
        for (int i = 0; i < numRoads; ++i) {
            trafficLights[i] = new TrafficLight();  // Allocate each TrafficLight object
        }
    }

    ~TrafficSimulation() {
        for (int i = 0; i < numVehicles; ++i) {
            delete vehicles[i];  // Free each Vehicle object
        }
        delete[] vehicles;  // Free the array of pointers to Vehicle objects
        
        for (int i = 0; i < numRoads; ++i) {
            delete trafficLights[i];  // Free each TrafficLight object
        }
        delete[] trafficLights;  // Free the array of pointers to TrafficLight objects
    }

    void addVehicle(const Vehicle& vehicle, int index) {
        if (index < numVehicles) {
            *vehicles[index] = vehicle;
        }
    }

    void run() {
        int cycles;
        cout << "Enter the number of cycles for the simulation: ";
        cin >> cycles;

        cout << "\n===========================\n";
        cout << "   Traffic Simulation\n";
        cout << "===========================\n\n";

        for (int i = 0; i < cycles; ++i) {
            cout << "========== Cycle " << i + 1 << " ==========\n";

            for (int j = 0; j < numRoads; ++j) {
                if (j == i % numRoads) {
                    trafficLights[j]->setYellow();  // Set yellow before green
                } else {
                    trafficLights[j]->setRed();  // Other roads are red
                }
            }

            cout << "\n** Yellow Alert **\n";
            printTrafficLightStates();
            
            handleVehicleActions(LightState::YELLOW);

            trafficLights[i % numRoads]->setGreen();

            cout << "\n** Green Light **\n";
            printTrafficLightStates();
            handleVehicleActions(LightState::GREEN);

            cout << "===========================\n\n";
        }

        cout << "Simulation ended.\n";
    }

private:
    int numVehicles;
    int numRoads;
    Vehicle** vehicles;  // Array of pointers to Vehicle objects
    TrafficLight** trafficLights;  // Array of pointers to TrafficLight objects

    void printTrafficLightStates() const {
        for (int j = 0; j < numRoads; ++j) {
            cout << "Road " << j + 1 << ": Traffic light is " << trafficLights[j]->getStateString() << ".\n";
        }
    }

    void handleVehicleActions(LightState lightState) {
        for (int i = 0; i < numVehicles; ++i) {
            LightState roadLightState = trafficLights[vehicles[i]->getRoadIndex()]->getState();
            if (roadLightState == LightState::GREEN || vehicles[i]->isEmergencyVehicle()) {
                vehicles[i]->move();
            } else if (roadLightState == LightState::YELLOW) {
                vehicles[i]->slowDown();
            } else {
                vehicles[i]->stop();
            }
        }
    }
};

int main() {
    int numVehicles, numRoads;

    cout << "Enter the number of roads (intersections): ";
    cin >> numRoads;

    cout << "Enter the number of vehicles in the simulation: ";
    cin >> numVehicles;

    TrafficSimulation simulation(numVehicles, numRoads);

    for (int i = 0; i < numVehicles; ++i) {
        string name;
        int roadIndex;
        bool isEmergency;
        cout << "Enter the name of vehicle " << i + 1 << ": ";
        cin >> name;
        cout << "Enter the road index (0 to " << numRoads - 1 << ") for this vehicle: ";
        cin >> roadIndex;
        cout << "Is this vehicle an emergency vehicle? (1 for Yes, 0 for No): ";
        cin >> isEmergency;

        Vehicle vehicle(name, roadIndex, isEmergency);
        simulation.addVehicle(vehicle, i);  // Add vehicle to the array
    }

    simulation.run();

    return 0;
}





