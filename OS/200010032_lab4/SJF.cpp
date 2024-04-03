#include<bits/stdc++.h>
#include<vector>
#include <fstream>
using namespace std;
#include <queue>
#include <iostream>
#include <string>
#include <sstream>
#include <queue>

struct process
{
    int pid;
    int arrival_time;
    int arrival_time_in_queue;
    vector<int> burst_time;

    int current_burst; // index of  current burst that we are in;
    int cur_needed_time; // current burst time
    int total_bust;
    int completed_time; // when was the job fully completed
};

bool compareBurstTime(const process &v1,const process &v2)
{
    return (v1.cur_needed_time< v2.cur_needed_time);
}
// bool compareBurstTime(const process &v1,const process &v2)
// {
//     return (v1.burst_time[v1.current_burst] < v2.burst_time[v1.current_burst]);
// }

bool compareArrivalTime(const process &v1, const process &v2)
{
    return (v1.arrival_time < v2.arrival_time);
}

bool compareEqual(const process &v1,const process &v2)
{
    return (v1.burst_time[v1.current_burst] == v2.burst_time[v1.current_burst]) && (v1.arrival_time_in_queue > v2.arrival_time_in_queue);
}


int main(int argc, char const *argv[]) {

    ifstream myFile(argv[1]);
    string line;
    vector<process> processes;
    vector<process> io_ready_queue;
    vector<process> cpu_ready_queue; // cpu ready queue is a priority queue based on CPU burst time of current burst
    vector<process> processes_ready; //

    // ----------------------reading input -------------------------------
    getline(myFile, line);
    getline(myFile, line);
    getline(myFile, line);

    int count = 0;
    while (getline(myFile, line)) {
        if(line == "</pre></body></html>"){
            break;
        }
        string s;
        stringstream ss(line);
        struct process P;
        P.pid = count;
        count++;

        getline(ss, s, ' ');
        stringstream number(s);
        number >> P.arrival_time;
        P.arrival_time_in_queue = P.arrival_time;
        P.total_bust = 0;
        // cout << "\narrival time : " << P.arrival_time << '\n';
        int k = 0;
        while (getline(ss, s, ' ') && s != "-1") {
            // int num = stoi(s);
            int x;
            stringstream number(s);
            number >> x;
            P.burst_time.push_back(x);
            if (k%2 == 0)
            {
                P.total_bust = P.total_bust + x;
            }
            k++;
        }
        // for (size_t i = 0 ; i < P.burst_time.size(); i++) {
        //     cout << P.burst_time[i] << ' ';
        // }
        // std::cout << "____________________________" << '\n';
        P.current_burst = 0;
        P.cur_needed_time = P.burst_time[0];
        processes.push_back(P);
        processes_ready.push_back(P);

    }
    int processes_remaining = count;
    // std::cout << processes_remaining << '\n';
    // return 0;
    myFile.close();
    // return 0;

    // ------------- Initializing variables --------------------
    // time = 0
    int current_time = 0;
    process p = processes_ready[0];
    processes_ready.erase(processes_ready.begin());
    cpu_ready_queue.push_back(p);
    process prev;
    bool check;
    bool check_cpu = false;
    check = false;
    std::cout << "PID  Arrival_time Completed_Time  Turn_Around_Time  Total_Bust  Waiting_Time  Penality_Ratio" << '\n';
    std::cout << "__________________________________________________________________________________" << '\n';
    float t_turn = 0;
    float t_wait = 0;
    float burst = 0;
    // --------------------------Execution of Processes----------------------------------------
    while (processes_remaining > 0) {
        // std::cout << "____________________________" << '\n';
        // std::cout << "\n processes remaing :" << processes_remaining  << "\n";
        // std::cout << "\n Current Time :" << current_time  << "\n";

        //--------- adding processes to cpu_ready_queue for first time ----------------------
        sort(processes_ready.begin(),processes_ready.end(),compareArrivalTime);

        while(processes_ready.size()>0)
        {
            process p = processes_ready[0];
            if (current_time == p.arrival_time)
            {

                cpu_ready_queue.push_back(p);
                processes_ready.erase(processes_ready.begin());

            }
            else
            {
                break;
            }

        }
        //------------------------------------------------
        // std::cout << cpu_ready_queue.size() << '\n';
        // ----------- CPU -------------------------------
        if (check)
        {
                io_ready_queue.push_back(prev);
                check = false;
        }

        if (cpu_ready_queue.size() > 0)
        {
                if (check_cpu == true)
                {
                    sort(cpu_ready_queue.begin(),cpu_ready_queue.end(),compareBurstTime);
                    sort(cpu_ready_queue.begin(),cpu_ready_queue.end(),compareEqual);
                    check_cpu = false;
                }
                if(cpu_ready_queue[0].cur_needed_time > 1)
                {
                    cpu_ready_queue[0].cur_needed_time = cpu_ready_queue[0].cur_needed_time - 1;

                    // std::cout << "\n Process running IN cpu :" << cpu_ready_queue[0].pid << "Remaining time : " <<cpu_ready_queue[0].cur_needed_time << "\n";


                }
                else
                {

                    cpu_ready_queue[0].current_burst = cpu_ready_queue[0].current_burst + 1;
                    if (cpu_ready_queue[0].current_burst < cpu_ready_queue[0].burst_time.size())
                    {
                        cpu_ready_queue[0].cur_needed_time = cpu_ready_queue[0].burst_time[cpu_ready_queue[0].current_burst];
                        prev = cpu_ready_queue[0];
                        // io_ready_queue.push_back(prev);
                        check = true;
                    }
                    else
                    {
                        cpu_ready_queue[0].completed_time = current_time + 1;
                        int arrival =  cpu_ready_queue[0].arrival_time;
                        int comp = cpu_ready_queue[0].completed_time;
                        int turn = comp - arrival;
                        int total_bust = cpu_ready_queue[0].total_bust;
                        int wait = turn - total_bust;
                        t_wait = t_wait + wait;
                        t_turn = t_turn + turn;
                        burst = burst + total_bust;
                        float penality = turn/total_bust;
                        std::cout << cpu_ready_queue[0].pid << "   " << arrival << "   "<< comp <<"   "<< turn <<"   " << total_bust << "   "<< wait << "  "<< penality<<endl;
                        processes_remaining = processes_remaining - 1;

                    }
                    cpu_ready_queue.erase(cpu_ready_queue.begin());
                    check_cpu = true;

                }

        }
        //-----------------------------------------------
        // std::cout << "IO number "<< io_ready_queue.size() << '\n';
        // ------------------ I/O -----------------------
        if (io_ready_queue.size()>0)
        {
            if(io_ready_queue[0].cur_needed_time > 1)
            {
                // std::cout << "Process running in IO : " << io_ready_queue[0].pid << '\n';
                io_ready_queue[0].cur_needed_time = io_ready_queue[0].cur_needed_time - 1;

            }
            else
            {
                io_ready_queue[0].arrival_time_in_queue = current_time;
                io_ready_queue[0].current_burst = io_ready_queue[0].current_burst + 1 ;
                io_ready_queue[0].cur_needed_time = io_ready_queue[0].burst_time[io_ready_queue[0].current_burst];
                // cout << io_ready_queue[0].pid<< ' ' << io_ready_queue[0].current_burst<< ' ' << io_ready_queue[0].cur_needed_time<<endl;
                cpu_ready_queue.push_back(io_ready_queue[0]);
                // std::cout << "Process completed IO :" <<io_ready_queue[0].pid << ' ';
                io_ready_queue.erase(io_ready_queue.begin());

            }

        }
        //---------------------------------------------------

        current_time++;

    }

    std::cout << "Average Waiting Time : " << t_wait/count <<'\n';
    std::cout << "Average Turn Around Time : " << t_turn/count <<'\n';
    std::cout << "Throughput : " << (float) count/current_time <<'\n';

    return 0;

}
