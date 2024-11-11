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

// Base Class Vehicle
class Vehicle {
public:
    Vehicle() : name("Unnamed"), roadIndex(0) {}
    Vehicle(const string& name, int roadIndex)
        : name(name), roadIndex(roadIndex) {}

    // Virtual function for polymorphism
    virtual void move() const {
        cout << setw(15) << left << name << " is moving on road " << roadIndex + 1 << ".\n";
    }

    // Virtual function for polymorphism
    virtual void stop() const {
        cout << setw(15) << left << name << " has stopped on road " << roadIndex + 1 << ".\n";
    }
    
    string getName() const { return name; }
    int getRoadIndex() const { return roadIndex; }

protected:
    string name;
    int roadIndex;
};

// Derived Class EmergencyVehicle
class EmergencyVehicle : public Vehicle {
public:
    EmergencyVehicle(const string& name, int roadIndex)
        : Vehicle(name, roadIndex) {}

    // Overriding move method for emergency vehicles (Polymorphism)
    void move() const override {
        cout << setw(15) << left << name << " (EMERGENCY) is moving on road " << roadIndex + 1 << ".\n";
    }

    // Overriding stop method for emergency vehicles (Polymorphism)
    void stop() const override {
        cout << setw(15
        ) << left << name << " is an emergency vehicle and doesn't stop.\n";
    }
};

// Base TrafficSimulation Class
class TrafficSimulation {
public:
    TrafficSimulation(int numVehicles, int numRoads)
        : numVehicles(numVehicles), numRoads(numRoads) {
        vehicles.reserve(numVehicles);
        trafficLights.resize(numRoads);
    }

    // Polymorphism: addVehicle can work with different types of vehicles
    virtual void addVehicle(Vehicle* vehicle) {
        vehicles.push_back(vehicle);
    }

    // Polymorphism: run can call different move/stop methods based on vehicle type
    virtual void run() {
        int cycles;
        cout << "Enter the number of cycles: ";
        cin >> cycles;

        for (int i = 0; i < cycles; ++i) {
            cout << "Cycle " << i + 1 << "\n";
            trafficLights[i % numRoads] = LightState::GREEN;

            // Polymorphism: Correct method (move/stop) is called based on vehicle type
            for (Vehicle* v : vehicles) {
                if (trafficLights[v->getRoadIndex()] == LightState::GREEN) {
                    v->move();
                } else {
                    v->stop();
                }
            }
        }
    }

protected:
    int numVehicles, numRoads;
    vector<Vehicle*> vehicles;
    vector<LightState> trafficLights;
};

// Derived Class AdvancedTrafficSimulation
class AdvancedTrafficSimulation : public TrafficSimulation {
public:
    AdvancedTrafficSimulation(int numVehicles, int numRoads)
        : TrafficSimulation(numVehicles, numRoads) {}

    // Overriding the run method (Polymorphism)
    void run() override {
        int cycles;
        cout << "Enter the number of advanced simulation cycles: ";
        cin >> cycles;

        for (int i = 0; i < cycles; ++i) {
            cout << "Advanced Cycle " << i + 1 << "\n";
            trafficLights[i % numRoads] = LightState::GREEN;

            // Polymorphism: Correct move method is called based on vehicle type
            for (Vehicle* v : vehicles) {
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

    // Advanced simulation
    AdvancedTrafficSimulation sim(numVehicles, numRoads);

    // Adding vehicles to the simulation
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

        // Polymorphism: Different types of vehicles added to simulation
        if (isEmergency) {
            sim.addVehicle(new EmergencyVehicle(name, roadIndex));
        } else {
            sim.addVehicle(new Vehicle(name, roadIndex));
        }
    }

    // Running the simulation
    sim.run();
    return 0;
}
