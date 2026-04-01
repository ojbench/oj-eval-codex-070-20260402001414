// Implementation for Problem 070 - CPU Task Scheduling
// You should not include extra headers per problem constraints.

typedef unsigned int uint;

namespace sjtu {

    struct Task {
        uint task_id;
        uint priority;
        uint time;

        explicit Task(uint _task_id = 0, uint _priority = 0, uint _time = 0) {
            task_id = _task_id;
            priority = _priority;
            time = _time;
        }

        Task(const Task &rhs) {
            task_id = rhs.task_id;
            priority = rhs.priority;
            time = rhs.time;
        }

        ~ Task() = default;
    };

    enum CPUState { idle = 0, busy = 1 };

    class CPU {
    protected:
        CPUState state;
        // We avoid std::vector includes per restriction; emulate minimal vector-like via new/delete would be heavy.
        // However, OJ driver includes our src.hpp directly and is responsible for IO; problem states we cannot add headers
        // but allows implementing run() and using a vector-like container in base. To stay safe, we implement a tiny dynamic array.

        struct Arr {
            Task *data;
            int sz;
            int cap;
            Arr(): data(nullptr), sz(0), cap(0) {}
            ~Arr(){ if (data) delete [] data; }
            int size() const { return sz; }
            Task &operator[](int i){ return data[i]; }
            const Task &operator[](int i) const { return data[i]; }
            void push_back(const Task &t){
                if (sz == cap){
                    int ncap = cap ? cap * 2 : 4;
                    Task *nd = new Task[ncap];
                    for (int i=0;i<sz;++i) nd[i] = data[i];
                    if (data) delete [] data;
                    data = nd; cap = ncap;
                }
                data[sz++] = t;
            }
            void erase(int idx){
                for (int i=idx;i+1<sz;++i) data[i] = data[i+1];
                if (sz) --sz;
            }
        } tasks;

    public:
        CPU() : tasks() { state = idle; }

        int addTask(const Task &t) { tasks.push_back(t); return 1; }

        int changePriority(uint task_id, uint priority) {
            for (int i=0;i<tasks.size();++i)
                if (tasks[i].task_id == task_id) { tasks[i].priority = priority; return 1; }
            return 0;
        }

        virtual std::pair<CPUState, uint> run() = 0;
        virtual ~ CPU() = default;
    };

    class CPU_FCFS : public CPU {
    public:
        std::pair<CPUState, uint> run() override {
            // Execute the first-added task with remaining time > 0.
            for (int i=0;i<tasks.size();++i){
                if (tasks[i].time > 0){
                    // consume 1 time unit
                    --tasks[i].time;
                    uint id = tasks[i].task_id;
                    if (tasks[i].time == 0){
                        // remove finished task to keep list small
                        tasks.erase(i);
                    }
                    state = busy;
                    return std::make_pair(busy, id);
                }
            }
            state = idle;
            return std::make_pair(idle, 0u);
        }
    };

    class CPU_SRTF : public CPU {
    public:
        std::pair<CPUState, uint> run() override {
            int idx = -1;
            for (int i=0;i<tasks.size();++i){
                if (tasks[i].time == 0) continue;
                if (idx == -1 || tasks[i].time < tasks[idx].time){
                    idx = i;
                }
            }
            if (idx == -1){ state = idle; return std::make_pair(idle, 0u); }
            // execute chosen task for 1 unit, tie breaks by earlier index (first added)
            --tasks[idx].time;
            uint id = tasks[idx].task_id;
            if (tasks[idx].time == 0){ tasks.erase(idx); }
            state = busy;
            return std::make_pair(busy, id);
        }
    };

    class CPU_PRIORITY : public CPU {
    public:
        std::pair<CPUState, uint> run() override {
            int idx = -1;
            for (int i=0;i<tasks.size();++i){
                if (tasks[i].time == 0) continue;
                if (idx == -1 || tasks[i].priority < tasks[idx].priority){
                    idx = i;
                }
            }
            if (idx == -1){ state = idle; return std::make_pair(idle, 0u); }
            --tasks[idx].time;
            uint id = tasks[idx].task_id;
            if (tasks[idx].time == 0){ tasks.erase(idx); }
            state = busy;
            return std::make_pair(busy, id);
        }
    };
}

