#include <bits/stdc++.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
using namespace std;

struct process {
  int pid;
  int arrival_time;
  vector<int> burst_time;

  int current_burst;   // index of  current burst that we are in;
  int cur_needed_time; // current burst time
  int total_burst;
  int arrival_time_in_queue;
  int completed_time; // when was the job fully completed
};

bool compareBurstTime(const process &v1, const process &v2) {
  return (v1.cur_needed_time < v2.cur_needed_time);
}

bool compareEqual(const process &v1, const process &v2) {
  return (v1.cur_needed_time == v2.cur_needed_time) &&
         (v1.arrival_time_in_queue > v2.arrival_time_in_queue);
}

bool compareArrivalTime(const process &v1, const process &v2) {
  return (v1.arrival_time < v2.arrival_time);
}

int main(int argc, char const *argv[]) {

  ifstream myFile(argv[1]);
  string line;
  vector<process> processes;
  vector<process> io_ready_queue;
  vector<process> cpu_ready_queue;
  vector<process> processes_ready;

  getline(myFile, line);
  getline(myFile, line);
  getline(myFile, line);

  int count = 0;
  while (getline(myFile, line)) {
    if (line == "</pre></body></html>") {
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
    P.total_burst = 0;
    int k = 0;
    while (getline(ss, s, ' ') && s != "-1") {
      int x;
      stringstream number(s);
      number >> x;
      P.burst_time.push_back(x);
      if (k % 2 == 0) {
        P.total_burst = P.total_burst + x;
      }
      k++;
    }
    P.current_burst = 0;
    P.cur_needed_time = P.burst_time[0];
    processes.push_back(P);
    processes_ready.push_back(P);
  }
  int processes_remaining = count;
  myFile.close();

  int current_time = 0;
  process p = processes_ready[0];
  processes_ready.erase(processes_ready.begin());
  cpu_ready_queue.push_back(p);
  process prev;
  bool check;
  check = false;
  printf("PID\tArrival_time\tCompleted_Time\tTurn_Around_Time\tTotal_"
         "Burst\tWaiting_Time\tPenalty_Ratio\n\n");
  float t_turn = 0;
  float t_wait = 0;
  float burst = 0;

  while (processes_remaining > 0) {
    sort(processes_ready.begin(), processes_ready.end(), compareArrivalTime);

    while (processes_ready.size() > 0) {
      process p = processes_ready[0];
      if (current_time == p.arrival_time) {
        cpu_ready_queue.push_back(p);
        processes_ready.erase(processes_ready.begin());

      } 
      else {
        break;
      }
    }
    if (check) {
      io_ready_queue.push_back(prev);
      check = false;
    }

    if (cpu_ready_queue.size() > 0) {
      sort(cpu_ready_queue.begin(), cpu_ready_queue.end(), compareBurstTime);
      sort(cpu_ready_queue.begin(), cpu_ready_queue.end(), compareEqual);
      if (cpu_ready_queue[0].cur_needed_time > 1) {
        cpu_ready_queue[0].cur_needed_time =
            cpu_ready_queue[0].cur_needed_time - 1;
      } 
      else {
        cpu_ready_queue[0].current_burst = cpu_ready_queue[0].current_burst + 1;
        if (cpu_ready_queue[0].current_burst <
            cpu_ready_queue[0].burst_time.size()) {
          cpu_ready_queue[0].cur_needed_time =
              cpu_ready_queue[0].burst_time[cpu_ready_queue[0].current_burst];
          prev = cpu_ready_queue[0];
          check = true;

        } 
        else {
          cpu_ready_queue[0].completed_time = current_time + 1;
          int arrival = cpu_ready_queue[0].arrival_time;
          int comp = cpu_ready_queue[0].completed_time;
          int turn = comp - arrival;
          int total_burst = cpu_ready_queue[0].total_burst;
          int wait = turn - total_burst;
          t_wait = t_wait + wait;
          t_turn = t_turn + turn;
          burst = burst + total_burst;
          float penalty = (float)turn / total_burst;         
          printf("%d\t%d\t%d\t%d\t%d\t%d\t%f\n", cpu_ready_queue[0].pid,
                 arrival, comp, turn, total_burst, wait, penalty);
          processes_remaining = processes_remaining - 1;
        }
        cpu_ready_queue.erase(cpu_ready_queue.begin());
      }
    }
    if (io_ready_queue.size() > 0) {
      if (io_ready_queue[0].cur_needed_time > 1) {
        io_ready_queue[0].cur_needed_time =
            io_ready_queue[0].cur_needed_time - 1;

      } 
      else {
        io_ready_queue[0].arrival_time_in_queue = current_time;
        io_ready_queue[0].current_burst = io_ready_queue[0].current_burst + 1;
        io_ready_queue[0].cur_needed_time =
            io_ready_queue[0].burst_time[io_ready_queue[0].current_burst];
        cpu_ready_queue.push_back(io_ready_queue[0]);
        io_ready_queue.erase(io_ready_queue.begin());
      }
    }

    current_time++;
  }

  printf("\nAverage Waiting Time : %f\n", t_wait / count);
  printf("Average Turn Around Time : %f\n", t_turn / count);
  printf("Throughput : %f\n", (float)count / current_time);
  return 0;
}
