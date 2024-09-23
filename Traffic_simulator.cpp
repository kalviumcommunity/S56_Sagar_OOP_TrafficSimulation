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

// The Vehicle class hides the internal details of a vehicle and only exposes necessary methods.
// This is an example of **abstraction** in OOP.
class Vehicle {
public:
    Vehicle(const string& name, int roadIndex, bool isEmergency = false)
        : name(name), roadIndex(roadIndex), isEmergency(isEmergency) {}

    // The public method getName() allows access to the vehicle's name without exposing how it's stored.
    string getName() const {
        return name;
    }

    void setName(const string& newName) {
        name = newName;
    }

    int getRoadIndex() const {
        return roadIndex;
    }

    void setRoadIndex(int newIndex) {
        roadIndex = newIndex;
    }

    // The method isEmergencyVehicle() exposes whether the vehicle is an emergency vehicle,
    // hiding the internal variable `isEmergency` from direct access (another example of abstraction).
    bool isEmergencyVehicle() const {
        return isEmergency;
    }

    void move() const {
        cout << setw(15) << left << name << " is moving on road " << roadIndex + 1 << ".\n";
    }

    void slowDown() const {
        cout << setw(15) << left << name << " is slowing down on road " << roadIndex + 1 << ".\n";
    }

    // The stop() method hides the complex logic of what happens when a vehicle stops,
    // providing an abstraction of the action to the user of the class.
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

// The TrafficLight class provides an abstraction for managing traffic light states.
// Users of this class don't need to know how the light state is stored or updated.
class TrafficLight {
public:
    TrafficLight() : state(LightState::RED) {}

    // Public methods expose functionality to change the light state
    // without revealing the internal implementation of how the state is managed.
    void setGreen() {
        state = LightState::GREEN;
    }

    void setYellow() {
        state = LightState::YELLOW;
    }

    void setRed() {
        state = LightState::RED;
    }

    // The getState() method provides a way to access the current light state without
    // exposing the internal representation of the state.
    LightState getState() const {
        return state;
    }

private:
    LightState state;
};

// The TrafficSimulation class abstracts the complexity of managing the simulation of vehicles and traffic lights.
// It provides high-level methods like run() and addVehicle() that hide internal logic.
class TrafficSimulation {
public:
    TrafficSimulation(int numVehicles, int numRoads)
        : numVehicles(numVehicles), numRoads(numRoads) {
        vehicles.reserve(numVehicles);
        trafficLights.resize(numRoads);
        for (int i = 0; i < numRoads; ++i) {
            trafficLights[i] = new TrafficLight();
        }
    }

    ~TrafficSimulation() {
        for (TrafficLight* light : trafficLights) {
            delete light;
        }
    }

    // The addVehicle() method abstracts the logic of adding vehicles to the simulation.
    void addVehicle(const Vehicle& vehicle) {
        vehicles.push_back(vehicle);
    }

    // The run() method abstracts the entire process of running a traffic simulation,
    // providing a simple interface for users of the class.
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

    // The private method printTrafficLightStates() abstracts the process of printing the current
    // states of all traffic lights, providing a clean interface to the rest of the class.
    void printTrafficLightStates() const {
        for (int j = 0; j < numRoads; ++j) {
            string stateStr = (trafficLights[j]->getState() == LightState::GREEN) ? "GREEN" :
                              (trafficLights[j]->getState() == LightState::YELLOW) ? "YELLOW" :
                              "RED";
            cout << "Road " << j + 1 << ": Traffic light is " << stateStr << ".\n";
        }
    }

    // The handleVehicleActions() method provides an abstraction for managing vehicle behavior
    // based on the current traffic light state, hiding complex logic.
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
        simulation.addVehicle(vehicle);
    }

    simulation.run();

    return 0;
}
