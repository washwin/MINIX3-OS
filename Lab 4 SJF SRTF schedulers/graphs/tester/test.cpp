#include <iostream>
#include <vector>
#include<queue>
using namespace std;

#define MAX_PROCESSES 50
#define MAX_TIME 2212

struct Process {
  int pid;
  int status;
  int arrival_time;
  int waiting_time;
  int turnaround_time;
  int response_time;
  float penalty_ratio;
  vector<int> task; // cpu burst or io burst time
};
struct Process process[MAX_PROCESSES];
// vector<int> ready_queue;
// vector<int> io_queue;

struct device {
  bool idle;
  int time_left;
};

struct sjf{
  bool operator()(Process const& p1, Process const& p2){
    return p1.task.at(0) < p2.task.at(0);
  }
};

int take_input(char *fname) {
  FILE *ptr = fopen(fname, "r");
  if (ptr == NULL) {
    printf("no such file.\n");
    return -1;
  }
  char buf[10];
  int temp, i = 0, j = 0;
  fscanf(ptr, "%s", buf); //<html>
  fscanf(ptr, "%s", buf); //<body>
  fscanf(ptr, "%s", buf); //<pre>
  while (fscanf(ptr, "%d ", &temp) == 1) {
    // save process info in the array of structures
    process[i].pid = i;
    process[i].status = 0;
    process[i].arrival_time = temp;
    process[i].waiting_time = 0;
    process[i].turnaround_time = -1;
    process[i].response_time = -1;
    while (fscanf(ptr, "%d ", &temp) == 1) {
      if (temp == -1) {
        break;
      }
      process[i].task.push_back(temp);
    }
    i++;
  }
  fclose(ptr);
  return i;
}

// void insert_ready_queue(int pid) {
//   // if (ready_queue.size() == 0) {
//   //   printf("notok\n");
//   //   ready_queue.push_back(pid);
//   // }
//   for (int i = 0; i < ready_queue.size(); i++) {
//     printf("ok\n");
//     Process p = process[ready_queue.at(i)];
//     if (p.task.at(0) < process[pid].task.at(0)) {
//       ready_queue.insert(ready_queue.begin() + i, pid);
//       break;
//     }
//   }
//   for (int i = 0; i < ready_queue.size(); i++) {
//   printf("ready queue pid : %d | task %d \n", ready_queue.at(i), process[ready_queue.at(i)].task.at(0));
//   }
// }

int main(int argc, char *argv[]) {
  int n = take_input(argv[1]); // take input from process1.dat
  if (n == -1) {
    return 0;
  }

  int time = 0;
  struct device cpu;
  struct device io;
  cpu.idle = true;
  io.idle = true;
  io.time_left = -1;

  // ready_queue.push_back(process[0].pid);
  priority_queue<Process, vector<Process>, sjf> ready_queue;
  queue<Process> io_queue;
  int arrived_processes = 0;
  Process rp; // running cpu process
  Process iop; // running io process

  while (time <= MAX_TIME) {
    if (arrived_processes < n) {
      for (int i = 0; i < n; i++) {
        if (process[i].arrival_time == time) {
          // insert_ready_queue(process[i].pid);
          ready_queue.push(process[i]);
          arrived_processes++;
        }
      }
    }
    time++;
    if (cpu.idle) {
      if (ready_queue.size() > 0) {
        // rp = &process[ready_queue.at(0)];
        rp = ready_queue.top();
        printf("top pid %d : tsk %d : time %d\n", rp.pid, rp.task.at(0), time);
        if (rp.task.size() > 0) {
          cpu.time_left = rp.task.at(0);
          rp.task.erase(rp.task.begin());
          // ready_queue.erase(ready_queue.begin());
          ready_queue.pop();
          cpu.idle = false;
        }
      }
    }
    cpu.time_left--;
    if (cpu.time_left == 0) {
      // printf("cpu task completed for pid : %d at time %d\n", rp.pid, time);
      cpu.idle = true;
      if (rp.task.size() > 0) {
        // io_queue.push_back(rp.pid);
        io_queue.push(process[rp.pid]);
      }
    }

    if (io.idle) {
      if (io_queue.size() > 0) {
        // rp = &process[io_queue.at(0)];
        iop = io_queue.front();
        io.time_left = iop.task.at(0);
        iop.task.erase(iop.task.begin());
        // io_queue.erase(io_queue.begin());
        io_queue.pop();
        io.idle = false;
      }
    }
    io.time_left--;
    if (io.time_left == 0) {
      // printf("i_o task completed for pid : %d at time %d\n", iop.pid, time);
      io.idle = true;
      if (iop.task.size() > 0) {
        // insert_ready_queue(rp->pid);
        ready_queue.push(iop);
      }
    }
  }
  // printf("%ld\n", process[0].task.size());
  // printf("%ld\n", process[1].task.size());
  // printf("%ld\n", process[2].task.size());
  // printf("%ld\n", process[3].task.size());
  return 0;
}