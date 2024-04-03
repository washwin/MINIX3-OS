#include <iostream>
#include <vector>
using namespace std;

#define MAX_PROCESSES 50
#define MAX_TIME 1000

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
vector<int> ready_queue;
vector<int> io_queue;

struct device {
  bool idle;
  int time_left;
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
      j++;
    }
    i++;
  }
  fclose(ptr);
  return i;
}

void insert_ready_queue(int pid) {
  if (ready_queue.size() == 0) {
    ready_queue.push_back(pid);
  }
  for (int i = 0; i < ready_queue.size(); i++) {
    Process p = process[ready_queue.at(0)];
    if (p.task.at(0) < process[pid].task.at(0)) {
      ready_queue.insert(ready_queue.begin() + i, pid);
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  int n = take_input(argv[1]); // take input from process1.dat
  if (n == -1) {
    return 0;
  }

  int time = 0;
  struct device cpu;
  struct device io;
  cpu.idle = true;
  io.idle = false;
  io.time_left = -1;

  ready_queue.push_back(process[0].pid);
  int arrived_processes = 1;
  Process *rp = &process[0]; // running process
  //   int rcp, rip;              // running cpu process and running io proces

  while (time <= MAX_TIME) {
    if (arrived_processes < n) {
      for (int i = 1; i < n; i++) {
        if (process[i].arrival_time == time) {
          insert_ready_queue(process[i].pid);
          arrived_processes++;
        }
      }
    }

    if (cpu.idle) {
      if (ready_queue.size() > 0) {
        rp = &process[ready_queue.at(0)];
        if (rp->task.size() > 0) {
          cpu.time_left = rp->task.at(0);
          rp->task.erase(rp->task.begin());
          ready_queue.erase(ready_queue.begin());
          cpu.idle = false;
        }
      }
    }
    cpu.time_left--;
    if (cpu.time_left == 0) {
      cpu.idle = true;
      if (rp->task.size() > 0) {
        io_queue.push_back(rp->pid);
      }
    }

    if (io.idle) {
      if (io_queue.size() > 0) {
        rp = &process[io_queue.at(0)];
        io.time_left = rp->task.at(0);
        rp->task.erase(rp->task.begin());
        io_queue.erase(io_queue.begin());
        io.idle = false;
      }
    }
    io.time_left--;
    if (io.time_left == 0) {
      io.idle = true;
      if (rp->task.size() > 0) {
        insert_ready_queue(rp->pid);
      }
    }

    time++;
  }

  return 0;
}