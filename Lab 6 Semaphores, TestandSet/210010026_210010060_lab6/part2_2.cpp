#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <semaphore.h>
#include <sys/types.h>  //pid_t 
#include <sys/wait.h>  //wait
#include <sys/ipc.h>
#include <unistd.h>   //fork
#include <sys/shm.h>  //shared memory

using namespace std;

//key_t key = 0x6400000;  //The key_t data type is an integral type (often an int) that uniquely identifies an IPC resource. 
//It can be defined using a numeric value or generated dynamically using the ftok() function.

sem_t lock;   //sem_t is datatype of semaphore

void rgb_to_grayscale(key_t key, int height, int width)
{  
	//ofstream T1_out("T1.txt");
    int shmid = shmget(key, sizeof(int) * height * width * 3, 0666 | IPC_CREAT);
    int *rgb_data = (int*)shmat(shmid, NULL, 0);

	for (int i = 0; i < height; ++i) 
	{
		for (int j = 0; j < width; ++j) 
		{   
            sem_wait(&lock);

			int index = i * width + j;
            int r = rgb_data[index * 3];
            int g = rgb_data[index * 3 + 1];
            int b = rgb_data[index * 3 + 2];
			
			int grayscale = round(0.2989 * r + 0.5870 * g + 0.1140 * b);  //luminosity method
			
			rgb_data[index * 3] = grayscale;
			rgb_data[index * 3 + 1] = grayscale;
			rgb_data[index * 3 + 2] = grayscale;
			//T1_out << i << ' ' << j << endl;
            sem_post(&lock);
			
		}
	}
}

void color_inversion(key_t key, int height, int width)
{   
	//ofstream T2_out("T2.txt");
    int shmid = shmget(key, sizeof(int) * height * width * 3, 0666 | IPC_CREAT);
    int *rgb_data = (int*)shmat(shmid, NULL, 0);

	for (int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width; j++) 
		{   
            sem_wait(&lock);

		    int index = i * width + j;
            int r = rgb_data[index * 3];
            int g = rgb_data[index * 3 + 1];
            int b = rgb_data[index * 3 + 2];
			
			rgb_data[index * 3] = 255 - r;
			rgb_data[index * 3 + 1] = 255 - g;
			rgb_data[index * 3 + 2] = 255 - b;
			//T2_out << i << ' ' << j << endl;
            sem_post(&lock);
		}
	}
}

int main(int argc, char* argv[]) {
  
	ifstream input(argv[1]);

    string image_format;
	int width, height, max_color_value;
	input >> image_format >> width >> height >> max_color_value;

	int R, G, B;

	key_t key = ftok(argv[1], 'R');
    if (key == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
	
    int shmid = shmget(key, sizeof(int) * height * width * 3, 0666 | IPC_CREAT);
	//shmget() function is used to create a new shared memory segment or access an existing one.
	//key: The key that identifies the shared memory segment.
	//sizeof(int) * height * width * 3: The size of the shared memory segment in bytes
	//0666 | IPC_CREAT: File permissions for the shared memory segment. IPC_CREAT flag indicates that the segment should be created if it doesn't already exist, and 0666 sets the permissions to allow read and write access to the owner, group, and others.

    if(shmid<0)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
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
    sem_init(&lock, 1, 1);  // &s reference to semaphore object
    //1 indicates it is shared between processes
    //1 is the intial value given to semaphore

    pid_t pid = fork();

	if (pid <0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		rgb_to_grayscale(key, height, width); 
		exit(0);
	}
    else if (pid > 0)
	{
    	wait(NULL);
    	color_inversion(key, height, width);
  
    }
  
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
  	
    return 0;
}
