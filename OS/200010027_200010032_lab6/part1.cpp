#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <chrono>

using namespace std::chrono;
using namespace std;

// function to convert rgb values into gray scale values
void rgb_to_gray(vector<int> &rgb_data, int height, int width)
{
	for (int i = 0; i < height; ++i) 
	{
		for (int j = 0; j < width; ++j) 
		{
			int index = i * width + j;
            int r = rgb_data[index * 3];
            int g = rgb_data[index * 3 + 1];
            int b = rgb_data[index * 3 + 2];
			
			int gray = round(0.2989 * r + 0.5870 * g + 0.1140 * b);
			
			rgb_data[index * 3] = gray;
			rgb_data[index * 3 + 1] = gray;
			rgb_data[index * 3 + 2] = gray;
			
		}
	}
}

// function to flip the image top to down
void inversion(vector<int> &rgb_data, int height, int width)
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

// driver code
int main(int argc, char* argv[]) {
  
	ifstream input(argv[1]);

	string m;
	input >> m;

	int width, height, max_color_value;
	input >> width >> height >> max_color_value;
	
	// declaring vectors for rgb values and gray values
	int R, G, B;
	vector<int> rgb_data;
	
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
	auto start = high_resolution_clock::now();
	
	rgb_to_gray(rgb_data,height,width); // converting rgb to gray
	  	  	
  	inversion(rgb_data,height,width); // inversion
  	
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
			int r = rgb_data[index * 3];
			int g = rgb_data[index * 3 + 1];
			int b = rgb_data[index * 3 + 2];
			output << r << ' ' << g << ' ' << b << endl;
		}
	}
	cout << "Time taken : "<< duration.count() << " microseconds" << endl;
  	
  return 0;
}
