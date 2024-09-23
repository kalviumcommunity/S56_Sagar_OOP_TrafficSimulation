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
    // Default constructor (Type 1)
    Vehicle() : name("Unnamed"), roadIndex(0), isEmergency(false) {
        // This is the default constructor, initializes Vehicle with default values
    }

    // Parameterized constructor (Type 2)
    Vehicle(const string& name, int roadIndex, bool isEmergency = false)
        : name(name), roadIndex(roadIndex), isEmergency(isEmergency) {
        // This is the parameterized constructor, initializes Vehicle with provided values
    }

    string getName() const { return name; }
    void setName(const string& newName) { name = newName; }
    int getRoadIndex() const { return roadIndex; }
    void setRoadIndex(int newIndex) { roadIndex = newIndex; }
    bool isEmergencyVehicle() const { return isEmergency; }

    void move() const {
        cout << setw(15) << left << name << " is moving on road " << roadIndex + 1 << ".\n";
    }

    void slowDown() const {
        cout << setw(15) << left << name << " is slowing down on road " << roadIndex + 1 << ".\n";
    }

    void stop() const {
        if (!isEmergency) {
            cout << setw(15) << left << name << " has stopped on road " << roadIndex + 1 << ".\n";
        }
    }

private:
    string name;
    int roadIndex;
    bool isEmergency;
};

class TrafficLight {
public:
    // Default constructor
    TrafficLight() : state(LightState::RED) {
        // This is the default constructor, initializes traffic light to RED state
    }

    void setGreen() { state = LightState::GREEN; }
    void setYellow() { state = LightState::YELLOW; }
    void setRed() { state = LightState::RED; }
    LightState getState() const { return state; }

private:
    LightState state;
};

class TrafficSimulation {
public:
    // Parameterized constructor to initialize the number of vehicles and roads
    TrafficSimulation(int numVehicles, int numRoads)
        : numVehicles(numVehicles), numRoads(numRoads) {
        vehicles.reserve(numVehicles); // Reserves space for vehicles
        trafficLights.resize(numRoads); // Resizes the vector to hold traffic lights

        // Allocating memory for each traffic light dynamically
        for (int i = 0; i < numRoads; ++i) {
            trafficLights[i] = new TrafficLight(); // Constructor for TrafficLight called here
        }
    }

    // Destructor to release memory allocated for traffic lights
    ~TrafficSimulation() {
        // Destructor cleans up dynamically allocated memory
        for (TrafficLight* light : trafficLights) {
            delete light; // Destructor for each TrafficLight object called here
        }
    }

    void addVehicle(const Vehicle& vehicle) {
        vehicles.push_back(vehicle); // Adds vehicle to the vector
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
    }

private:
    int numVehicles;
    int numRoads;
    vector<Vehicle> vehicles;
    vector<TrafficLight*> trafficLights;

    void printTrafficLightStates() const {
        for (int j = 0; j < numRoads; ++j) {
            string stateStr = (trafficLights[j]->getState() == LightState::GREEN) ? "GREEN" :
                              (trafficLights[j]->getState() == LightState::YELLOW) ? "YELLOW" :
                              "RED";
            cout << "Road " << j + 1 << ": Traffic light is " << stateStr << ".\n";
        }
    }

    void handleVehicleActions(LightState lightState) {
        for (const Vehicle& vehicle : vehicles) {
            LightState roadLightState = trafficLights[vehicle.getRoadIndex()]->getState();
            if (roadLightState == LightState::GREEN || vehicle.isEmergencyVehicle()) {
                vehicle.move();
            } else if (roadLightState == LightState::YELLOW) {
                vehicle.slowDown();
            } else {
                vehicle.stop();
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

    // Constructor for TrafficSimulation called here
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

        // Parameterized constructor for Vehicle called here
        Vehicle vehicle(name, roadIndex, isEmergency);
        simulation.addVehicle(vehicle); // Adds vehicle to the simulation
    }

    simulation.run();

    // Destructor for TrafficSimulation called here when the program ends
    return 0;
}
