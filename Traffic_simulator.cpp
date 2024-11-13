#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <memory>
using namespace std;

enum class LightState {
    RED,
    YELLOW,
    GREEN
};

// Abstract Base Class Vehicle - Represents vehicles
class Vehicle {
public:
    Vehicle(const string& name, int roadIndex)
        : name(name), roadIndex(roadIndex) {}

    virtual void move() const = 0;
    virtual void stop() const = 0;
    
    string getName() const { return name; }
    int getRoadIndex() const { return roadIndex; }

protected:
    string name;
    int roadIndex;
};

// Derived Class EmergencyVehicle - Represents an emergency vehicle
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

// Derived Class RegularVehicle - Represents a regular vehicle
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

// A separate class responsible for managing traffic lights
class TrafficLightManager {
public:
    TrafficLightManager(int numRoads) : trafficLights(numRoads, LightState::RED) {}

    void changeLight(int roadIndex, LightState state) {
        trafficLights[roadIndex] = state;
    }

    LightState getLightState(int roadIndex) const {
        return trafficLights[roadIndex];
    }

private:
    vector<LightState> trafficLights;
};

// A separate class responsible for managing vehicles
class VehicleManager {
public:
    void addVehicle(unique_ptr<Vehicle> vehicle) {
        vehicles.push_back(move(vehicle));
    }

    const vector<unique_ptr<Vehicle>>& getVehicles() const {
        return vehicles;
    }

private:
    vector<unique_ptr<Vehicle>> vehicles;
};

// Base Simulation Strategy Interface - Open for Extension, Closed for Modification
class ISimulationStrategy {
public:
    virtual void run(const VehicleManager& vehicleManager, TrafficLightManager& trafficLightManager, int numCycles) = 0;
    virtual ~ISimulationStrategy() = default;
};

// Basic Traffic Simulation Strategy - Original behavior
class BasicTrafficSimulation : public ISimulationStrategy {
public:
    void run(const VehicleManager& vehicleManager, TrafficLightManager& trafficLightManager, int numCycles) override {
        for (int i = 0; i < numCycles; ++i) {
            cout << "Cycle " << i + 1 << "\n";
            trafficLightManager.changeLight(i % 3, LightState::GREEN); // Change light for simplicity
            
            for (const auto& v : vehicleManager.getVehicles()) {
                if (trafficLightManager.getLightState(v->getRoadIndex()) == LightState::GREEN) {
                    v->move();
                } else {
                    v->stop();
                }
            }
        }
    }
};

// Advanced Traffic Simulation Strategy - Extended behavior
class AdvancedTrafficSimulation : public ISimulationStrategy {
public:
    void run(const VehicleManager& vehicleManager, TrafficLightManager& trafficLightManager, int numCycles) override {
        for (int i = 0; i < numCycles; ++i) {
            cout << "Advanced Cycle " << i + 1 << "\n";

            for (const auto& v : vehicleManager.getVehicles()) {
                v->move();  // In advanced simulation, all vehicles move regardless of light
            }
        }
    }
};

// TrafficSimulation Class - Uses a strategy to run the simulation
class TrafficSimulation {
public:
    TrafficSimulation(int numVehicles, int numRoads, unique_ptr<ISimulationStrategy> strategy)
        : numVehicles(numVehicles), numRoads(numRoads), vehicleManager(), trafficLightManager(numRoads), simulationStrategy(move(strategy)) {}

    void run() {
        int cycles;
        cout << "Enter the number of cycles: ";
        cin >> cycles;

        simulationStrategy->run(vehicleManager, trafficLightManager, cycles);
    }

    void addVehicle(unique_ptr<Vehicle> vehicle) {
        vehicleManager.addVehicle(move(vehicle));
    }

protected:
    int numVehicles, numRoads;
    VehicleManager vehicleManager;
    TrafficLightManager trafficLightManager;
    unique_ptr<ISimulationStrategy> simulationStrategy;  // Strategy pattern for extensibility
};

int main() {
    int numVehicles, numRoads;
    cout << "Enter number of roads: ";
    cin >> numRoads;

    cout << "Enter number of vehicles: ";
    cin >> numVehicles;

    // Choose the simulation strategy at runtime (OCP: Easily extendable without modifying existing code)
    unique_ptr<ISimulationStrategy> strategy = make_unique<AdvancedTrafficSimulation>();

    TrafficSimulation sim(numVehicles, numRoads, move(strategy));

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

        if (isEmergency) {
            sim.addVehicle(make_unique<EmergencyVehicle>(name, roadIndex));
        } else {
            sim.addVehicle(make_unique<RegularVehicle>(name, roadIndex));
        }
    }

    sim.run();
    return 0;
}
