#include <iostream>
#include <fstream>
#include <vector>
#include<cmath>  //round
#include<string>

using namespace std;

void rgb_to_grayscale(vector<int> &rgb_data, int height, int width)// function to convert rgb values into gray scale values
{
	for (int i = 0; i < height; ++i) 
	{
		for (int j = 0; j < width; ++j) 
		{
			int index = i * width + j;
            int r = rgb_data[index * 3];
            int g = rgb_data[index * 3 + 1];
            int b = rgb_data[index * 3 + 2];
			
			int grayscale = round(0.2989 * r + 0.5870 * g + 0.1140 * b);  //luminosity method
			
			rgb_data[index * 3] = grayscale;
			rgb_data[index * 3 + 1] = grayscale;
			rgb_data[index * 3 + 2] = grayscale;
			
		}
	}
}

void color_inversion(vector<int> &rgb_data, int height, int width) // function to flip the image in terms of color(ie white to black and black to white)
{
	for (int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width; j++) 
		{
		    int index = i * width + j;
            int r = rgb_data[index * 3];
            int g = rgb_data[index * 3 + 1];
            int b = rgb_data[index * 3 + 2];
			
			rgb_data[index * 3] = 255 - r;
			rgb_data[index * 3 + 1] = 255 - g;
			rgb_data[index * 3 + 2] = 255 - b;
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

	rgb_to_grayscale(rgb_data,height,width); 
	  	  	
  	color_inversion(rgb_data,height,width); 
  
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
