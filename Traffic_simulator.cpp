#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <memory> // For smart pointers
using namespace std;

enum class LightState {
    RED,
    YELLOW,
    GREEN
};

// Abstract Base Class Vehicle - This class has a single responsibility: Representing vehicles
class Vehicle {
public:
    Vehicle(const string& name, int roadIndex)
        : name(name), roadIndex(roadIndex) {}

    // Pure virtual functions (Abstract methods) for move and stop actions
    virtual void move() const = 0;  // Must be overridden by derived classes
    virtual void stop() const = 0;  // Must be overridden by derived classes
    
    string getName() const { return name; }
    int getRoadIndex() const { return roadIndex; }

protected:
    string name;
    int roadIndex;
};

// Derived Class EmergencyVehicle - Represents a specific type of vehicle with its own behavior
class EmergencyVehicle : public Vehicle {
public:
    EmergencyVehicle(const string& name, int roadIndex)
        : Vehicle(name, roadIndex) {}

    void move() const override {
        cout << setw(15) << left << name << " (EMERGENCY) is moving on road " << roadIndex + 1 << ".\n";
    }

    void stop() const override {
        cout << setw(15) << left << name << " is an emergency vehicle and doesn't stop.\n";
    }
};

// Derived Class RegularVehicle - Represents a regular vehicle with its own behavior
class RegularVehicle : public Vehicle {
public:
    RegularVehicle(const string& name, int roadIndex)
        : Vehicle(name, roadIndex) {}

    void move() const override {
        cout << setw(15) << left << name << " is moving on road " << roadIndex + 1 << ".\n";
    }

    void stop() const override {
        cout << setw(15) << left << name << " is stopped at the red light on road " << roadIndex + 1 << ".\n";
    }
};

// A separate class responsible for managing traffic lights - This class has a single responsibility: Traffic light management
class TrafficLightManager {
public:
    TrafficLightManager(int numRoads) : trafficLights(numRoads, LightState::RED) {}

    // Change the light for a specific road
    void changeLight(int roadIndex, LightState state) {
        trafficLights[roadIndex] = state;
    }

    // Get the current light state for a specific road
    LightState getLightState(int roadIndex) const {
        return trafficLights[roadIndex];
    }

private:
    vector<LightState> trafficLights;  // Holds the state of traffic lights for each road
};

// A separate class responsible for managing vehicles - This class has a single responsibility: Vehicle management
class VehicleManager {
public:
    void addVehicle(unique_ptr<Vehicle> vehicle) {
        vehicles.push_back(move(vehicle));
    }

    const vector<unique_ptr<Vehicle>>& getVehicles() const {
        return vehicles;
    }

private:
    vector<unique_ptr<Vehicle>> vehicles;  // Holds all the vehicles in the simulation
};

// Base TrafficSimulation Class - This class has a single responsibility: Simulating traffic
class TrafficSimulation {
public:
    TrafficSimulation(int numVehicles, int numRoads)
        : numVehicles(numVehicles), numRoads(numRoads), vehicleManager(), trafficLightManager(numRoads) {}

    virtual void run() {
        int cycles;
        cout << "Enter the number of cycles: ";
        cin >> cycles;

        for (int i = 0; i < cycles; ++i) {
            cout << "Cycle " << i + 1 << "\n";
            trafficLightManager.changeLight(i % numRoads, LightState::GREEN);

            // Only responsibility of TrafficSimulation is to manage the simulation cycles
            for (const auto& v : vehicleManager.getVehicles()) {
                if (trafficLightManager.getLightState(v->getRoadIndex()) == LightState::GREEN) {
                    v->move();  // Move vehicles if light is green
                } else {
                    v->stop();  // Stop vehicles if light is not green
                }
            }
        }
    }

    void addVehicle(unique_ptr<Vehicle> vehicle) {
        vehicleManager.addVehicle(move(vehicle));
    }

protected:
    int numVehicles, numRoads;
    VehicleManager vehicleManager;       // Handles vehicle management
    TrafficLightManager trafficLightManager; // Handles traffic light management
};

// Derived Class AdvancedTrafficSimulation - This class overrides the run method but still focuses on the simulation logic
class AdvancedTrafficSimulation : public TrafficSimulation {
public:
    AdvancedTrafficSimulation(int numVehicles, int numRoads)
        : TrafficSimulation(numVehicles, numRoads) {}

    void run() override {
        int cycles;
        cout << "Enter the number of advanced simulation cycles: ";
        cin >> cycles;

        for (int i = 0; i < cycles; ++i) {
            cout << "Advanced Cycle " << i + 1 << "\n";

            // In this advanced simulation, all vehicles move regardless of traffic light state
            for (const auto& v : vehicleManager.getVehicles()) {
                v->move();
            }
        }
    }
};

int main() {
    int numVehicles, numRoads;
    cout << "Enter number of roads: ";
    cin >> numRoads;

    cout << "Enter number of vehicles: ";
    cin >> numVehicles;

    AdvancedTrafficSimulation sim(numVehicles, numRoads);

    for (int i = 0; i < numVehicles; ++i) {
        string name;
        int roadIndex;
        bool isEmergency;
        cout << "Vehicle name: ";
        cin >> name;
        cout << "Road index: ";
        cin >> roadIndex;
        cout << "Is it an emergency vehicle? (1 for Yes, 0 for No): ";
        cin >> isEmergency;

        // Conditionally create either an EmergencyVehicle or RegularVehicle based on user input
        if (isEmergency) {
            sim.addVehicle(make_unique<EmergencyVehicle>(name, roadIndex));
        } else {
            sim.addVehicle(make_unique<RegularVehicle>(name, roadIndex));
        }
    }

    sim.run();
    return 0;
}
