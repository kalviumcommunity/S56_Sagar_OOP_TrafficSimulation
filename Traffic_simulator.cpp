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

    // Accessor (getter) for vehicle name
    string getName() const {
        return name;
    }

    // Mutator (setter) for vehicle name
    void setName(const string& name) {
        this->name = name;
    }

    // Accessor for road index
    int getRoadIndex() const {
        return roadIndex;
    }

    // Mutator for road index
    void setRoadIndex(int roadIndex) {
        this->roadIndex = roadIndex;
    }

    // Accessor for emergency status
    bool isEmergencyVehicle() const {
        return isEmergency;
    }

    // Mutator for emergency status
    void setIsEmergency(bool isEmergency) {
        this->isEmergency = isEmergency;
    }

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

    // Static Member Function to get total vehicles
    static int getTotalVehicles() {
        return totalVehicles;
    }

    // Static Member Function to increment total vehicles
    static void incrementTotalVehicles() {
        ++totalVehicles;
    }

private:
    string name;
    int roadIndex;
    bool isEmergency;

    static int totalVehicles;  // Static variable to keep track of total vehicles
};

// Initialize static member
int Vehicle::totalVehicles = 0;

class TrafficLight {
public:
    TrafficLight() : state(LightState::RED) {}

    // Accessor for traffic light state
    LightState getState() const {
        return state;
    }

    // Mutator for traffic light state
    void setState(LightState newState) {
        state = newState;
    }

    void setGreen() {
        setState(LightState::GREEN);
    }

    void setYellow() {
        setState(LightState::YELLOW);
    }

    void setRed() {
        setState(LightState::RED);
    }

    string getStateString() const {
        switch (state) {
            case LightState::GREEN: return "GREEN";
            case LightState::YELLOW: return "YELLOW";
            case LightState::RED: return "RED";
            default: return "UNKNOWN";
        }
    }

    // Static Member Function to get total traffic lights
    static int getTotalTrafficLights() {
        return totalTrafficLights;
    }

    // Static Member Function to increment total traffic lights
    static void incrementTotalTrafficLights() {
        ++totalTrafficLights;
    }

private:
    LightState state;

    static int totalTrafficLights;
};

// Initialize static member
int TrafficLight::totalTrafficLights = 0;

class TrafficSimulation {
public:
    TrafficSimulation(int numVehicles, int numRoads)
        : numVehicles(numVehicles), numRoads(numRoads) {
        vehicles = new Vehicle*[numVehicles];
        for (int i = 0; i < numVehicles; ++i) {
            vehicles[i] = new Vehicle();
            Vehicle::incrementTotalVehicles();
        }
        trafficLights = new TrafficLight*[numRoads];
        for (int i = 0; i < numRoads; ++i) {
            trafficLights[i] = new TrafficLight();
            TrafficLight::incrementTotalTrafficLights();
        }
    }

    ~TrafficSimulation() {
        for (int i = 0; i < numVehicles; ++i) {
            delete vehicles[i];
        }
        delete[] vehicles;

        for (int i = 0; i < numRoads; ++i) {
            delete trafficLights[i];
        }
        delete[] trafficLights;
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
                    trafficLights[j]->setYellow();
                } else {
                    trafficLights[j]->setRed();
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
        cout << "Total Vehicles: " << Vehicle::getTotalVehicles() << endl;
        cout << "Total Traffic Lights: " << TrafficLight::getTotalTrafficLights() << endl;
    }

private:
    int numVehicles;
    int numRoads;
    Vehicle** vehicles;
    TrafficLight** trafficLights;

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
        simulation.addVehicle(vehicle, i);
    }

    simulation.run();

    return 0;
}
