#include "util.h"
#include "custom_types.h"

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

std::atomic<unsigned short> n, t, h, d, t1, t2;
std::vector<Instance> instances;
std::vector<std::thread> instance_threads;

std::mutex m;
std::condition_variable available_instances;

void print_instance_status() {
    std::cout << std::endl;
    for (const auto &instance: instances) {
        std::cout << "Instance " << instance.id << ": "
                << (instance.isActive ? "active" : "empty") << ", "
                << "Total time run: " << instance.totalTimeRun
                << ", Parties served: " << instance.partiesServed << "\n";
    }
    std::cout << "----------------------------------------\n";
}

void party_formation() {
    while (true) {
        std::unique_lock lock(m);

        if (!(t >= 1 && h >= 1 && d >= 3)) {
            lock.unlock();
            available_instances.notify_all();
            break;
        }

        available_instances.wait(lock, [] {
            return std::any_of(instances.begin(), instances.end(), [](const Instance &instance) {
                return !instance.isActive;
            });
        });

        // Double check after waiting
        if (!(t >= 1 && h >= 1 && d >= 3)) {
            lock.unlock();
            available_instances.notify_all();
            break;
        }

        // Find available instance
        short instance = -1;
        for (unsigned short i = 0; i < n; ++i) {
            if (!instances[i].isActive) {
                instance = i;
                instances[instance].isActive = true;
                break;
            }
        }

        if (instance == -1) continue;

        t -= 1;
        h -= 1;
        d -= 3;

        std::uniform_int_distribution<unsigned int> distribution(t1, t2);
        unsigned int duration = distribution(generator);

        std::cout << "Instance " << instance << " started, to take " << duration << " seconds.\n";
        print_instance_status();

        lock.unlock();

        // Start the instance thread
        std::thread instanceThread([instance, duration]() {
            // Sleep for the duration
            std::this_thread::sleep_for(std::chrono::seconds(duration));

            // Finish the instance
            {
                std::lock_guard lock(m);
                instances[instance].isActive = false;
                instances[instance].totalTimeRun += duration;
                instances[instance].partiesServed++;
                std::cout << "Instance " << instance << " finished\n";
                print_instance_status();
            }

            available_instances.notify_all();
        });

        // Lock the thread to the vector
        {
            std::lock_guard threadLock(m);
            instance_threads.push_back(std::move(instanceThread));
        }
    }
}

int main() {
    const auto config = read_config();

    // Check if all keys are present
    for (const std::string required_keys[] = {"n", "t", "h", "d", "t1", "t2"}; const auto &key: required_keys) {
        if (!config.contains(key)) {
            throw std::invalid_argument("Missing configuration key: " + key);
        }
    }

    n = config.at("n");
    t = config.at("t");
    h = config.at("h");
    d = config.at("d");
    t1 = config.at("t1");
    t2 = config.at("t2");

    // Output config
    std::cout << "--  Configuration  --\n";
    std::cout << "Max instances: " << n << "\n";
    std::cout << "Tanks: " << t << "\n";
    std::cout << "Healers: " << h << "\n";
    std::cout << "DPS: " << d << "\n";
    std::cout << "Min duration: " << t1 << "\n";
    std::cout << "Max duration: " << t2 << "\n";
    std::cout << "=====================\n";

    // n must be greater than 0
    if (n == 0) {
        throw std::invalid_argument("n must be greater than 0");
    }

    // Create instances
    for (unsigned short i = 0; i < n; ++i) {
        instances.emplace_back(i);
    }

    std::thread partyFormationThread(party_formation);
    partyFormationThread.join();

    // Join all instance threads
    for (auto &instanceThread: instance_threads) {
        if (instanceThread.joinable()) {
            instanceThread.join();
        }
    }

    return 0;
}
