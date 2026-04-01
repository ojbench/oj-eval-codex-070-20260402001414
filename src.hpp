// src.hpp: Implement CPU scheduling derived classes in provided base framework.

#ifndef _SJTU_CPP_FINAL_CPU_HPP_
#define _SJTU_CPP_FINAL_CPU_HPP_

#include <vector>
#include "task.hpp"

using namespace std;

typedef unsigned int uint;

namespace sjtu {
    // CPU base class, modifications is not allowed.
    class CPU {
    protected:
        CPUState state;
        vector<Task> tasks;

    public:
        CPU() : tasks() { state = idle; }

        // Add a new task.
        int addTask(const Task &t) {
            tasks.push_back(t);
            return 1;
        }

        // Change the priority of one process, return 1 if success and return 0 if fail.
        int changePriority(uint task_id, uint priority) {
            for (auto &task: tasks)
                if (task.task_id == task_id) {
                    task.priority = priority;
                    return 1;
                }
            return 0;
        }

        virtual pair<CPUState, uint> run() = 0;

        virtual ~ CPU() = default;
    };

    // FCFS method based CPU.
    class CPU_FCFS : public CPU {
    public:
        pair<CPUState, uint> run() override {
            for (size_t i = 0; i < tasks.size(); ++i) {
                if (tasks[i].time > 0) {
                    --tasks[i].time;
                    uint id = tasks[i].task_id;
                    if (tasks[i].time == 0) {
                        tasks.erase(tasks.begin() + i);
                    }
                    state = busy;
                    return make_pair(busy, id);
                }
            }
            state = idle;
            return make_pair(idle, 0u);
        }
    };

    // SRTF method based CPU.
    class CPU_SRTF : public CPU {
    public:
        pair<CPUState, uint> run() override {
            int idx = -1;
            for (size_t i = 0; i < tasks.size(); ++i) {
                if (tasks[i].time == 0) continue;
                if (idx == -1 || tasks[i].time < tasks[idx].time) {
                    idx = static_cast<int>(i);
                }
            }
            if (idx == -1) {
                state = idle;
                return make_pair(idle, 0u);
            }
            --tasks[idx].time;
            uint id = tasks[idx].task_id;
            if (tasks[idx].time == 0) {
                tasks.erase(tasks.begin() + idx);
            }
            state = busy;
            return make_pair(busy, id);
        }
    };

    // priority method based CPU.
    class CPU_PRIORITY : public CPU {
    public:
        pair<CPUState, uint> run() override {
            int idx = -1;
            for (size_t i = 0; i < tasks.size(); ++i) {
                if (tasks[i].time == 0) continue;
                if (idx == -1 || tasks[i].priority < tasks[idx].priority) {
                    idx = static_cast<int>(i);
                }
            }
            if (idx == -1) {
                state = idle;
                return make_pair(idle, 0u);
            }
            --tasks[idx].time;
            uint id = tasks[idx].task_id;
            if (tasks[idx].time == 0) {
                tasks.erase(tasks.begin() + idx);
            }
            state = busy;
            return make_pair(busy, id);
        }
    };
}

#endif
