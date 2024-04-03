#include<bits/stdc++.h>
#include <iostream>
#include <fstream>

using namespace std;


int main(int argc, char const *argv[]) {

    int addressable_pages = atoi(argv[1]);
    int physical_frames = atoi(argv[2]);
    int swap_space_blocks = atoi(argv[3]);
    int page_faults = 0;
    int hits = 0;
    vector<int> physical_memory;
    vector<int> swap_space;

    ifstream input_file(argv[4]);
    int page;

    while(input_file >> page)
    {
        cout << page << ' ';
        if(page>addressable_pages)
        {
            cout << "segmentation fault" <<'\n';
        }
        else
        {
            if (!count(physical_memory.begin(), physical_memory.end(), page))
            {
                cout << "page fault ";
                if (physical_memory.size() == physical_frames)
                {
                    int victim = physical_memory[0];
                    physical_memory.erase(physical_memory.begin());
                    physical_memory.push_back(page);
                    swap_space.push_back(victim);
                    vector<int>::iterator it;
                    it = find(swap_space.begin(), swap_space.end(), page);
                    if (it != swap_space.end())
                    {
                        swap_space.erase(it);
                        cout<<"swapped" <<'\n';
                    }
                    else
                    {
                        cout << "\n";
                    }
                }
                else
                {
                    physical_memory.push_back(page);
                    cout << "\n";
                }
                page_faults++;
            }
            else
            {
                hits++;
                cout << "hit"<<'\n';
            }
        }

    }

    cout << "Page faults : " << page_faults << "\n";
    cout << "Number of hits : " << hits << "\n";
    return 0;
}