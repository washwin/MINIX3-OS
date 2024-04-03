#include <iostream>
#include <fstream>
#include <vector>
#include<cmath>
#include<string>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void rgb_to_grayscale(vector<int> &rgb_data, int height, int width, int pipefd[2])
{   
	//ofstream T1_out("T1.txt");
    close(pipefd[0]);

	for (int i = 0; i < height; ++i) 
	{
		for (int j = 0; j < width; ++j) 
		{   
            int data[3];
			int index = i * width + j;
            int r = rgb_data[index * 3];
            int g = rgb_data[index * 3 + 1];
            int b = rgb_data[index * 3 + 2];
			
			int grayscale = round(0.2989 * r + 0.5870 * g + 0.1140 * b);  //luminosity method
			
			data[0] = grayscale;
			data[1] = grayscale;
			data[2] = grayscale;
			//T1_out << i << ' ' << j << endl;
            write(pipefd[1], &data, sizeof(data));
			
		}
	}
}

void color_inversion(vector<int> &output_data, int height, int width, int pipefd[2])
{   
	//ofstream T2_out("T2.txt");
    close(pipefd[1]);

	for (int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width; j++) 
		{   
            int data1[3];
            read(pipefd[0], &data1, sizeof(data1));

		    int index = i * width + j;
			int r = 255 - data1[0];
			int g = 255 - data1[1];
			int b = 255 - data1[2];

            output_data.push_back(r);
            output_data.push_back(g);
            output_data.push_back(b);
			//T2_out << i << ' ' << j << endl;
		}
	}
}

int main(int argc, char* argv[]) {
  
	ifstream input(argv[1]);

    string image_format;
	int width, height, max_color_value;
	input >> image_format >> width >> height >> max_color_value;

	int R, G, B;
	vector<int> rgb_data;
    vector<int> output_data;
	
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

    int pipefd[2];
    int x = pipe(pipefd);
    if (x == -1)                //The pipe() function returns 0 if successful, and -1 if it fails
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid <0)
    {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
    {
		rgb_to_grayscale(rgb_data, height, width, pipefd); 
        close(pipefd[1]);
		exit(0);
	}
    else if (pid > 0)
    {
    	color_inversion(output_data, height, width, pipefd);
        close(pipefd[0]);
  
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
			int r = output_data[index * 3];
			int g = output_data[index * 3 + 1];
			int b = output_data[index * 3 + 2];
			output << r << ' ' << g << ' ' << b << endl;
		}
	}
  	
    return 0;
}
