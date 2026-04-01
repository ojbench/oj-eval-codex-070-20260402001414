#include <bits/stdc++.h>
using namespace std;
#include "src.hpp"
using namespace sjtu;
int main(){
    CPU_FCFS cpu;
    cpu.addTask(Task(1,10,3));
    cpu.addTask(Task(2,10,1));
    for(int t=0;t<6;++t){
        auto p=cpu.run();
        cout<<p.first<<" "<<p.second<<"\n";
    }
}
