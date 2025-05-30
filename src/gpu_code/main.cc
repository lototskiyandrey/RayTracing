#include <iostream>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>


void load_and_populate_cmfs(std::vector<double> &cmf_r, std::vector<double> &cmf_g, std::vector<double> &cmf_b) ;

int longest_wavelength = 830;
int shortest_wavelength = 390;

int main() 
{   

    // int image_width = 500;
    // int image_height = 500;

    // std::cout << "P3\n " << image_width << " " << image_height << "\n255\n";

    // for(int j = 0; j < image_height; j++)
    // {
    //     std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
    //     for(int i = 0; i < image_width; i++)
    //     {
    //         auto r = (double)i / image_width;
    //         auto g = (double)j / image_height;
    //         auto b = 0;

    //         auto ir = int(r * 255);
    //         auto ig = int(g * 255);
    //         auto ib = int(b * 255);

    //         std::cout << ir << ' ' << ig << ' ' << ib << std::endl;
    //     }
    // }

    // std::clog << "\rDone.                             \n";

    int spectrum_length = longest_wavelength - shortest_wavelength + 1;

    std::vector<double> spectrum(spectrum_length);

    std::vector<double> cmf_r(spectrum_length);
    std::vector<double> cmf_g(spectrum_length);
    std::vector<double> cmf_b(spectrum_length);



    load_and_populate_cmfs(cmf_r, cmf_g, cmf_b);

    for(auto r : cmf_r)
    {
        std::cout << r << "\n";
    }
    

    return 0;
}

/*

Implmenting a more realistic color system. Every ray is now an array where each element corresponds to the intesity of
a particular wavelength of visible light

*/

void load_and_populate_cmfs(std::vector<double> &cmf_r, std::vector<double> &cmf_g, std::vector<double> &cmf_b) 
{
    std::ifstream f("cmf.csv");

    if(!f.is_open())
    {
        std::cerr << "Error opening file!";
        exit(-1);
    }

    std::string s;


    while(std::getline(f, s))
    {   
        int row_num = 0;
        std::string elems[4];

        std::string delimiter = ",";

        auto pos = s.find(delimiter);

        while(pos != std::string::npos)
        {
            elems[row_num] = s.substr(0, pos);

            s.erase(0, pos + delimiter.length());

            pos = s.find(delimiter);

            row_num++;
        }

        elems[row_num] = s;


        int wavelength = std::stoi(elems[0]);
        int index = wavelength - shortest_wavelength;

        double red_val = std::stod(elems[1]);
        double green_val = std::stod(elems[2]);
        double blue_val = std::stod(elems[3]);
        
        cmf_r.at(index) = red_val;
        cmf_g.at(index) = green_val;
        cmf_b.at(index) = blue_val;
    }

    f.close();
}


void write_color(std::ostream &out, std::vector<double> spectrum)
{

}