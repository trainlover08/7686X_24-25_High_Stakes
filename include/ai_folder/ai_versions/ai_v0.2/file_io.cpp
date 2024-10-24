#include <fstream>
#include <string>
#include <sstream>
#include <vector>

/*

    vec<     --layer

        vec<vec<double>> weights 

        vec<double> biases

*/

double my_rand(const int &range) {
    return ((std::rand() % (range * 2)) - range) / static_cast<double>(range / 2);
}

unsigned short total_lines(const std::string &input)
    {
        std::ifstream file(input);
        unsigned int count = 0;
        std::string line;

        while (std::getline(file, line))
        {
            ++count;
        }

        file.close();
        return count;
    }

    void creation_cycle(std::ofstream &newFile)
{
    static bool d = false;
    newFile << (my_rand(100000) * 0.0001);

    if (!d)
    {
        newFile << ",";
    }
    else
    {
        newFile << " ";
    }
    d = !d;
}

namespace fio {

    std::vector<std::vector<std::pair<double, double>>> read_file(std::string filename) {

        std::vector<std::vector<std::pair<double, double>>> output;

        if(!filename.ends_with(".txt")) {
            filename = filename.append(".txt");
        }

        std::ifstream file(filename);

        std::stringstream buffer;

        buffer << file.rdbuf();

        file.close();

        std::string file_content = buffer.str();
        std::stringstream file_stream(file_content);

        std::string temp_string = "";

        auto total_lines_in_file = total_lines(filename);

        for(int file_lines = 0; file_lines < total_lines_in_file; ++file_lines) {
            char character; 
            bool weight = false;
            int set_index = 0;

            if (output.size() <= file_lines)
                {
                    output.resize(file_lines + 1);
                }
                if (output[file_lines].size() <= set_index)
                {
                    output[file_lines].resize(set_index + 1);
                }

                std::vector<char> temp_char_string(1000);
                unsigned short temp_char_string_index = 0;
                double doubleValue;
                file_stream.get(character);

                switch (character)
                {
                case ' ':
                    weight = false;
                    output[file_lines][set_index].second = doubleValue;
                    set_index++;
                    break;
                case ',':
                    weight = false;
                    doubleValue = std::stof(temp_string);
                    output[file_lines][set_index].first = doubleValue;
                    temp_string.clear();
                    break;
                case '\n':
                    file_lines++;
                    break;
                default:
                    temp_string.push_back(character);
                    break;
            
                }
            
            return output;

        }

    }

    bool create_file(const std::string& filename, unsigned int n_input, unsigned int n_hidden_layers, unsigned int n_hidden_neurons, unsigned int n_output) {
        std::ofstream file(filename);

        if (!file.is_open()) {
            return false;
        }

        for (unsigned char j = 0; j < (n_input * 2); ++j)
        {
            creation_cycle(file);
        }
        file << std::endl;

        for (char k = 0; k < n_hidden_layers; ++k)
        {
            for (unsigned char m = 0; m < (n_hidden_neurons * 2); ++m)
            {
                creation_cycle(file);
            }
            file << std::endl;
        }

        for (unsigned char g = 0; g < (n_output * 2); ++g)
        {
            creation_cycle(file);
        }
        file << std::endl;

        file.close();

        return true;
    }

    bool write_file(std::string file_name, const std::vector<std::vector<std::vector<<double>>> &input) {

        if(!file_name.ends_with(".txt")) {
            file_name = file_name.append(".txt");
        }

        std::ofstream file(file_name);

        for(int a = 0; a < input.size(); a++) {
            for(int b = 0; b < input[a].size(); ++b){
                file << input[a][b].second;
                file << ',';
                file << input[a][b].first;
                file << ' ';
            }
            file << '\n';
        }

    }

};