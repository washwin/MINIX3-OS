#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#include <fcntl.h>

using namespace std::chrono;
using namespace std;
key_t k = 0x1256;
sem_t s;

void rgb_to_gray(key_t k, int height, int width)
{
	//cout<<"PID of gray"<<pid<<"\n";
	// If it is a parent process, then return
	
        
        int shmid = shmget(k, sizeof(int) * (height) * width*3, 0666 | IPC_CREAT);
        //int* rgb_data = new int[width*height*3];
        
    	int *rgb_data = (int*)shmat(shmid, NULL, 0);
    	
    	
	//cout << " I am in gray" << "\n";
	//ofstream t1_out("t1.txt");
	for (int i = 0; i < height; ++i) 
	{
		for (int j = 0; j < width; ++j) 
		{
			sem_wait(&s);
			
			int index = i * width + j;
		    int r = rgb_data[index * 3];
		    int g = rgb_data[index * 3 + 1];
		    int b = rgb_data[index * 3 + 2];
			
			int gray = round(0.2989 * r + 0.5870 * g + 0.1140 * b);
			
			rgb_data[index * 3] = gray;
			rgb_data[index * 3 + 1] = gray;
			rgb_data[index * 3 + 2] = gray;
			//t1_out << i<<' ' << j  << endl;
			
			sem_post(&s); // Semaphore unlocks
		}
		
	}
	
	//cout << "gray Done" << "\n";
}

// function to flip the image top to down
void inversion(key_t k, int height, int width)
{	
	//cout<<"PID of flip"<<pid<<"\n";
	// If it is a parent process, then return
	
        
        int shmid = shmget(k, sizeof(int) * (height) * width*3, 0666 | IPC_CREAT);
        //int rgb_data[width*height];
        //int* rgb_data = new int[width*height*3];
    	int *rgb_data = (int*)shmat(shmid, NULL, 0);
	//cout << "flip start" << "\n";
    
    //ofstream t2_out("t2.txt");
    for (int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width; j++) 
		{
			sem_wait(&s);
		    int index = i * width + j;
		    int r = rgb_data[index * 3];
		    int g = rgb_data[index * 3 + 1];
		    int b = rgb_data[index * 3 + 2];
			
			rgb_data[index * 3] = 255 - r;
			rgb_data[index * 3 + 1] = 255 - g;
			rgb_data[index * 3 + 2] = 255 - b;
			//t2_out << i<<' ' << j  << endl;
			sem_post(&s); // Semaphore unlocks
			
		}
		
	}
    
    //cout << "flip Done" << "\n";
}


int main(int argc, char *argv[])
{
    /*char version[100];
    int height, width, max, r, g, b;
    FILE *inputImage = fopen(argv[1], "r");
    FILE *outputImage = fopen(argv[2], "w");
    fscanf(inputImage, "%s%d%d%d", version, &width, &height, &max);*/
    ifstream input(argv[1]);
	string m;
	input >> m;
	int width, height, max_color_value;
	input >> width >> height >> max_color_value;
	int R, G, B;
    int shmid = shmget(k, sizeof(int) *height * width*3, 0666 | IPC_CREAT);
    if(shmid<0)
    {
        perror("shmget");
        exit(1);
    }
    int *rgb_data = (int *)shmat(shmid,NULL,0);
	for (int i = 0; i < height; ++i) 
	{
		for (int j = 0; j < width; ++j) 
		{
			int index = i*width +j;
			input >> R >> G >> B;
			rgb_data[index*3] = R;
			rgb_data[index*3 + 1] = G;
			rgb_data[index*3 + 2] = B;
			
		}
	}
	/*
	for(int i = 0; i < height*width*3; ++i) {
		cout<<rgb_data[i];
	}*/
	
	auto start = high_resolution_clock::now();
	sem_init(&s, 0, 1);

  	// Threads made for each image transformation
	int pid = fork();
	if(pid <0){
		cout<<"Fork failed";
		exit(0);
	}
	if(pid == 0){
		rgb_to_gray(k, height, width); 
		exit(0);
	}
    	else if(pid > 0){
    		wait(NULL);
    		inversion(k, height, width);
  
    	}
   
    
  	auto stop = high_resolution_clock::now();
  	auto duration = duration_cast<microseconds>(stop - start);

  	
  	// writing to out file
	ofstream output(argv[2]);
	output << "P3" << endl;
	output << width << ' ' << height << endl;
	output << max_color_value << endl;
	
	for (int i = 0; i < height; ++i) 
	{	
		for (int j = 0; j < width; ++j) 
		{
			int index = i * width + j;
			int r = rgb_data[index * 3];
			int g = rgb_data[index * 3 + 1];
			int b = rgb_data[index * 3 + 2];
			output << r << ' ' << g << ' ' << b << endl;
		}
	}
	cout << "Time taken :"<< duration.count() << " microseconds" << endl;
 

   
    return 0;
}
