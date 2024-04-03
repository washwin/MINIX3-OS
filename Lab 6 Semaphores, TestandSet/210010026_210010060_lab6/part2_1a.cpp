#include <iostream>
#include <fstream>
#include <vector>
#include<cmath>
#include<string>
#include<thread>
#include<atomic>

using namespace std;
atomic_flag l = ATOMIC_FLAG_INIT; //initializes the atomic_flag variable l with an initial value of false

void rgb_to_grayscale(vector<int> &rgb_data, int height, int width)
{	
	//ofstream T1_out("T1.txt");
	for (int i = 0; i < height; ++i) 
	{
		for (int j = 0; j < width; ++j) 
		{	
			while (atomic_flag_test_and_set(&l));

			int index = i * width + j;
            int r = rgb_data[index * 3];
            int g = rgb_data[index * 3 + 1];
            int b = rgb_data[index * 3 + 2];
			
			int grayscale = round(0.2989 * r + 0.5870 * g + 0.1140 * b);  //luminosity method
			
			rgb_data[index * 3] = grayscale;
			rgb_data[index * 3 + 1] = grayscale;
			rgb_data[index * 3 + 2] = grayscale;
			//T1_out << i << ' ' << j << endl;
			//cout<<"gray"<<i<<' '<<j<<endl;
			atomic_flag_clear(&l);
		}
	}
}

void color_inversion(vector<int> &rgb_data, int height, int width)
{	
	//ofstream T2_out("T2.txt");
	for (int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width; j++) 
		{	
			while (atomic_flag_test_and_set(&l));

		    int index = i * width + j;
            int r = rgb_data[index * 3];
            int g = rgb_data[index * 3 + 1];
            int b = rgb_data[index * 3 + 2];
			
			rgb_data[index * 3] = 255 - r;
			rgb_data[index * 3 + 1] = 255 - g;
			rgb_data[index * 3 + 2] = 255 - b;
			//T2_out << i << ' ' << j << endl;
			//cout<<"inv"<<i<<' '<<j<<endl;

			atomic_flag_clear(&l);
		}
	}
}

int main(int argc, char* argv[]) {
  
	ifstream input(argv[1]);

    string image_format;
	int width, height, max_color_value;
	input >> image_format >> width >> height >> max_color_value;
	//cout<<image_format<<' '<<width<<' '<<height<<' '<<max_color_value<<endl;
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

	thread T1 (rgb_to_grayscale, ref(rgb_data), height, width);  //ref is necessary as we cannot pass it copy we have to pass pointer
    thread T2 (color_inversion, ref(rgb_data), height, width);
      
    T1.join();  //main process wait till T1 and t2 completes
    T2.join();
  
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
