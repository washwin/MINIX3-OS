#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cmath>
#include <string>

using namespace std::chrono;
using namespace std;
int width, height, max_color_value;

// function to convert rgb values into gray scale values
void rgb_to_gray(vector<int> &rgb_data, int height, int width, int pipefds[2])
{
	//ofstream t1_out("t1.txt");
	close(pipefds[0]); 
	for (int i = 0; i < height; ++i) 
	{
		for (int j = 0; j < width; ++j) 
		{
			int index = i * width + j;
		    int r = rgb_data[index * 3];
		    int g = rgb_data[index * 3 + 1];
		    int b = rgb_data[index * 3 + 2];
			
			int gray = round(0.2989 * r + 0.5870 * g + 0.1140 * b);
			int data[3];
			
			data[0] = gray;
			data[1] = gray;
			data[2] = gray;
			
			//cout << data[0] << ' ' << data[1] << '\n' ;
			write(pipefds[1], &data , sizeof(data));
			//t1_out << i<<' ' << j  << endl;
			
		}
	}
}

// function to flip the image top to down
void inversion(vector<int> &g_data, int height, int width, int pipe[2])
{
		close(pipe[1]); 
		
     	//ofstream t2_out("t2.txt");
	     for (int i = 0; i < height; i++) 
		{
			for (int j = 0; j < width; j++) 
			{
				int temp[3];
				
				read(pipe[0], &temp, sizeof(temp)); 
			
				//cout << temp[0] << ' ' << temp[1] << '\n' ;
			    int index = i * width + j;
			    int r = 255 - temp[0];
			    int g = 255 - temp[1];
			    int b = 255 - temp[2];
				
				g_data.push_back(r);
				g_data.push_back(g);
				g_data.push_back(b);
				
				//rgb_data[index * 3] = 255 - r;
				//rgb_data[index * 3 + 1] = 255 - g;
				//rgb_data[index * 3 + 2] = 255 - b;
				//t2_out << i<<' ' << j  << endl;
				
				
			}
			
		}
}

// driver code
int main(int argc, char* argv[]) {
  
  	vector<int> rgb_data;
  	vector<int> g_data;
	ifstream input(argv[1]);
	
	string m;
	input >> m;
	
	

	
	input >> width >> height >> max_color_value;
	
	// declaring vectors for rgb values and gray values
	int R, G, B;
	
	// reading rgb values from ppm file
	for (int i = 0; i < height; ++i) 
	{
		for (int j = 0; j < width; ++j) 
		{
			input >> R >> G >> B;
			rgb_data.push_back(R);
			rgb_data.push_back(G);
			rgb_data.push_back(B);
			
		}
	}
	//--------------------------------------------------------------------------------------
	// vector<int> g_data;
	int pipefd[2];
    if (pipe(pipefd) == -1) 
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    auto start = high_resolution_clock::now();
    pid_t pid1 = fork();
    //cout<<pid1<<"\n";
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid1 == 0) 
    { 	//cout<<"Grey\n";
        
        rgb_to_gray(rgb_data, height, width, pipefd);  
        exit(0);	
    }
    else
    {
    	//cout<<"Flip\n";
    	//wait(NULL);
    	//close(pipefd[1]);
        inversion(g_data, height, width, pipefd); 
        close(pipefd[0]);
        //cout << "in done" << '\n' ;
        
    }	
  	
  	auto stop = high_resolution_clock::now();
  	
  	auto duration = duration_cast<microseconds>(stop - start);
  	
        ofstream output(argv[2]);
	output << "P3" << endl;
	output << width << ' ' << height << endl;
	output << max_color_value << endl;
	
	for (int i = 0; i < height; ++i) 
	{	
		for (int j = 0; j < width; ++j) 
		{
			int index = i * width + j;
			int r = g_data[index * 3];
			int g = g_data[index * 3 + 1];
			int b = g_data[index * 3 + 2];
			output << r << ' ' << g << ' ' << b << endl;
		}
	}
	
    

	cout << "Time taken : "<< duration.count() << " microseconds" << endl;
  	
  return 0;
}
