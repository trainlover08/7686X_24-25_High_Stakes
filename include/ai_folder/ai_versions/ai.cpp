#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <utility>

// Function to apply ReLU activation
double ReLU(double x)
{
    return std::max(0.0, x);
}

class LayerDense
{
public:
    void initialize(size_t n_inputs, size_t n_neurons, const std::vector<std::vector<double>> &weights, const std::vector<double> &biases);
    std::vector<std::vector<double>> forward(const std::vector<std::vector<double>> &inputs);
    bool apply_ReLU = true;

private:
    std::vector<std::vector<double>> weights;
    std::vector<double> biases;
};

void LayerDense::initialize(size_t n_inputs, size_t n_neurons, const std::vector<std::vector<double>> &init_weights, const std::vector<double> &init_biases)
{
    weights = init_weights;
    biases = init_biases;
}

std::vector<std::vector<double>> LayerDense::forward(const std::vector<std::vector<double>> &inputs)
{
    std::vector<std::vector<double>> output(inputs.size(), std::vector<double>(biases.size(), 0.0));
    for (size_t i = 0; i < inputs.size(); ++i)
    {
        for (size_t k = 0; k < biases.size(); ++k)
        {
            double sum = 0.0;
            for (size_t j = 0; j < inputs[i].size(); ++j)
            {
                sum += inputs[i][j] * weights[j][k];
            }
            output[i][k] = sum + biases[k];
            // Apply ReLU activation
            if (apply_ReLU == true)
            {
                output[i][k] = ReLU(output[i][k]);
            }
        }
    }
    return output;
}

namespace ai{

class NeuralNetwork
{
public:
    std::string FILENAME = "NNQtable";
    double greedy_start = 1.0f;
    unsigned char selected_models = 1;
    unsigned short episodes = 1;
    unsigned short current_epoch = 0;
    double LEARNINGRATE = 1.0;

private:
    double greedy_value()
    {
        double greedy_min = greedy_start * 0.001f;
        double decay_rate = LEARNINGRATE;
        return (greedy_min + (greedy_start - greedy_min)) * std::exp(-decay_rate * current_epoch);
    }

    double range_rand(const int &range)
    {
        return ((std::rand() % (range * 2)) - range) / static_cast<double>(range / 2);
    }

    double greedy_adjusted_value(double &inputs, const double &greedy_val)
    {
        return inputs + (range_rand(1000) * greedy_val);
    }

    std::vector<std::string> file_name(const std::string &base_file_name)
    {
        std::vector<std::string> output;
        for (unsigned char j = 0; j < selected_models; ++j)
        {
            std::string index_str = std::to_string(j + 1);
            std::string file_name = base_file_name + index_str + ".txt";
            output.push_back(file_name);
        }
        return output;
    }

    std::vector<std::vector<std::vector<std::vector<double>>>> readTxt(const std::string &file_name, unsigned char iteration_number)
    {
        std::vector<std::vector<std::vector<std::vector<double>>>> output;

        std::string line;
        std::ifstream file(file_name);

        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            unsigned char i, j, k, l;
            double value;
            if (iss >> i >> j >> k >> l >> value)
            {
                while (output.size() <= i)
                    output.push_back({});
                while (output[i].size() <= j)
                    output[i].push_back({});
                while (output[i][j].size() <= k)
                    output[i][j].push_back({});
                output[i][j][k].push_back(value);
            }
        }

        file.close();
        return output;
    }

    std::vector<std::vector<std::vector<std::vector<double>>>> mutated_values(std::vector<std::vector<std::vector<std::vector<double>>>> input_data)
    {
        for (unsigned short index = 0; index < input_data.size(); ++index)
        {
            for (unsigned short k = 0; k < input_data[index].size(); ++k)
            {
                for (unsigned short j = 0; j < input_data[index][k].size(); ++j)
                {
                    for (unsigned short a = 0; a < input_data[index][k][j].size(); ++a)
                    {
                        input_data[index][k][j][a] = greedy_adjusted_value(input_data[index][k][j][a], greedy_value());
                    }
                }
            }
        }
        return input_data;
    }

    std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> generate_batch(const unsigned char n_batch_size)
    {
        std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> output(n_batch_size);
        std::vector<std::string> names = file_name(FILENAME);

        unsigned char actual_batch_size = std::min(n_batch_size, static_cast<unsigned char>(names.size()));

        for (char j = 0; j < actual_batch_size; ++j)
        {
            std::string name_gen = names.at(j);
            std::vector<std::vector<std::vector<std::vector<double>>>> input = mutated_values(readTxt(name_gen, j));
            output[j] = input;
        }
        return output;
    }

void creation_cycle(std::ofstream &newFile)
{
    static bool d = false;
    newFile << 0.001f;

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


    void create_model(const std::vector<std::string> &file_names, char n_input, char &n_hidden_neurons, char &n_hidden_layers, char &n_output)
{
    for (unsigned char i = 0; i < file_names.size(); ++i)
    {
        std::string filename = file_names[i];
        std::ofstream newFile(filename);
        if (!newFile.is_open())
        {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            continue;
        }

        for (unsigned char j = 0; j < (n_input * 2); ++j)
        {
            creation_cycle(newFile);
        }
        newFile << std::endl;

        for (char k = 0; k < n_hidden_layers; ++k)
        {
            for (unsigned char m = 0; m < (n_hidden_neurons * 2); ++m)
            {
                creation_cycle(newFile);
            }
            newFile << std::endl;
        }

        for (unsigned char g = 0; g < (n_output * 2); ++g)
        {
            creation_cycle(newFile);
        }
        newFile << std::endl;

        newFile.close();
    }
}


    void create_multiple_models(char n_input, char n_hidden_neurons, char n_hidden_layers, char n_output)
    {
        for (unsigned char j = 0; j < selected_models; ++j)
        {
            create_model(file_name(FILENAME), n_input, n_hidden_neurons, n_hidden_layers, n_output);
        }
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

    unsigned short n_neurons_of_line(const std::string &input, unsigned short target_line)
    {
        std::ifstream file(input);
        unsigned short count = 0;
        std::string line;
        char character;
        unsigned short index = 0;

        for (unsigned short i = 0; i < target_line; ++i)
        {
            std::getline(file, line);
            ++count;
        }

        while (file.get(character))
        {
            if (character == '\n')
            {
                return index / 3;
            }
            if (character == ' ' || character == '\t')
            {
                if (character == ' ')
                {
                    ++index;
                }
            }
        }
        file.close();
        return index;
    }

    double find_value(const std::string &file_name, unsigned char nn_layer, unsigned short dataset_in_layer, unsigned char weight_or_bias)
    {
        double output;
        std::string outputstr;
        std::ifstream file(file_name);
        std::string line;
        char character;

        for (unsigned char i = 0; i < nn_layer;)
        {
            std::getline(file, line);
        }

        for (unsigned short k = 0; k < dataset_in_layer; ++k)
        {
            file.get(character);
            if (character == ' ')
            {
                file.get(character);
                if (character == ' ')
                {
                    ++k;
                }
            }

            if (k + 1 == dataset_in_layer)
            {
                if (weight_or_bias == 0)
                {
                    file.get(character);
                    while (character != ' ')
                    {
                        file.get(character);
                    }
                    file >> outputstr;
                }
                else
                {
                    file.get(character);
                    while (character != ' ')
                    {
                        file.get(character);
                    }
                    file >> outputstr;
                }
                output = std::stof(outputstr);
            }
        }

        return output;
    }

    std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> full_read_of_files(const std::string &base_file_name)
    {
        std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> output;
        std::vector<std::string> names = file_name(base_file_name);
        output.resize(names.size());

        for (unsigned char my_file_name = 0; my_file_name < names.size(); my_file_name++)
        {
            std::ifstream nfile(names[my_file_name]);

            std::stringstream buffer;
            buffer << nfile.rdbuf(); // Read the entire file into the stringstream
            nfile.close();
        
            std::string file_content = buffer.str();
            std::stringstream file_stream(file_content);

            unsigned short total_lines_of_file = total_lines(names[my_file_name]);
            std::string temp_string = "";

            for (unsigned short file_lines = 0; file_lines < total_lines_of_file;)
            {
                char character;
                bool weight = false;
                unsigned short set_index = 0;

                if (output[my_file_name].size() <= file_lines)
                {
                    output[my_file_name].resize(file_lines + 1);
                }
                if (output[my_file_name][file_lines].size() <= set_index)
                {
                    output[my_file_name][file_lines].resize(set_index + 1);
                    output[my_file_name][file_lines][set_index].resize(2);
                }

                std::vector<char> temp_char_string(50);
                unsigned short temp_char_string_index = 0;
                double doubleValue;
                file_stream.get(character);

                switch (character)
                {
                case ' ':
                    weight = false;
                    output[my_file_name][file_lines][set_index][weight].push_back(doubleValue);
                    set_index++;
                    break;
                case ',':
                    weight = false;
                    doubleValue = std::stof(temp_string);
                    output[my_file_name][file_lines][set_index][weight].push_back(doubleValue);
                    temp_string.clear();
                    break;
                case '\n':
                    file_lines++;
                    break;
                default:
                    temp_string.push_back(character);
                    break;
                }
            }
        }

        return output;
    }

    std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> create_episodes(std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> input_vector)
    {
        std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> output;

        int TOTALNETWORKS = input_vector.size() * episodes;

        output.resize(TOTALNETWORKS);

        for (int currentSet = 0; currentSet < input_vector.size(); ++currentSet)
        {
            int intigralNet = 0;
            for (int currentNetwork = 0; currentNetwork < episodes; ++currentNetwork)
            {
                output.push_back(input_vector[currentSet]);
                ++intigralNet;
            }
        }
        return output;
    }

    std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> mutate_batch(std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> input)
    {
        for (unsigned short a = 0; a < input.size(); ++a)
        {
            for (unsigned short b = 0; b < input[a].size(); ++b)
            {
                for (unsigned short c = 0; c < input[a][b].size(); ++c)
                {
                    for (unsigned short d = 0; d < input[a][b][c].size(); ++d)
                    {
                        for (unsigned short e = 0; e < input[a][b][c][d].size(); ++e)
                        {
                            if (input[a][b][c][d].size() > e)
                            {
                                input[a][b][c][d][e] = greedy_adjusted_value(input[a][b][c][d][e], greedy_value());
                            }
                        }
                    }
                }
            }
        }

        return input;
    }

    std::vector<std::vector<std::vector<double>>> run_batch(std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> &input, std::vector<std::vector<std::vector<double>>> &input_data)
    {
        LayerDense layer;
        std::vector<std::vector<std::vector<double>>> output(input.size());

        for (size_t input_first_index = 0; input_first_index < input.size(); ++input_first_index)
        {
            size_t num_layers = input[input_first_index].size();
            std::vector<std::vector<double>> temp_output;

            for (size_t input_second_index = 0; input_second_index < num_layers; ++input_second_index)
            {
                size_t n_inputs;
                std::vector<std::vector<double>> weights;
                std::vector<double> biases;
                std::vector<std::vector<double>> forward_inputs;

                for (size_t neuron_set_index = 0; neuron_set_index < input[input_first_index][input_second_index].size(); ++neuron_set_index)
                {
                    weights.push_back(std::vector<double>());
                }

                for (size_t neuron_set_index = 0; neuron_set_index < input[input_first_index][input_second_index].size(); ++neuron_set_index)
                {
                    if (input_second_index == 0)
                    {
                        n_inputs = input_data.size();
                        forward_inputs = input_data[neuron_set_index];
                    }
                    else
                    {
                        n_inputs = input[input_first_index][input_second_index][neuron_set_index].size();
                    }

                    double temp_weight = input[input_first_index][input_second_index][neuron_set_index][0][0];
                    weights[neuron_set_index].push_back(temp_weight);
                    double temp_biases = input[input_first_index][input_second_index][neuron_set_index][0][1];
                    biases.push_back(temp_biases);
                }

                layer.initialize(n_inputs, input[input_first_index][input_second_index].size(), weights, biases);

                if (input_second_index != 0)
                {
                    forward_inputs = temp_output;
                }

                if (num_layers == input_second_index + 1)
                {
                    layer.apply_ReLU = false;
                }

                temp_output = layer.forward(forward_inputs);
                layer.apply_ReLU = true;
            }

            output[input_first_index] = temp_output;
        }
        return output;
    }

    std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> find_best_networks(std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> &vector_of_networks, std::vector<double> &vector_of_scores)
    {
        std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> output;

        std::vector<std::pair<double, int>> score_and_index;

        for (int index = 0; index < vector_of_scores.size(); ++index)
        {
            score_and_index.push_back(std::make_pair(vector_of_scores[index], index));
        }

        std::sort(score_and_index.begin(), score_and_index.end(), std::greater<std::pair<double, int>>());

        std::vector<int> top_episodes;

        for (int second_index = 0; second_index < selected_models; ++second_index)
        {
            top_episodes.push_back(score_and_index[second_index].second);
        }

        for (int a = 0; a < top_episodes.size(); ++a)
        {
            output.push_back(vector_of_networks[top_episodes[a]]);
        }

        return output;
    }

    void write_data(const std::string &base_file_name, std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> &input)
    {
        for (int a = 0; a < input.size(); ++a)
        {
            std::ofstream newFile((base_file_name + std::to_string(a + 1) + ".txt"));
            for (int b = 0; b < input[a].size(); ++b)
            {
                for (int c = 0; c < input[a][b].size(); ++c)
                {
                    for (int d = 0; d < input[a][b][c].size(); ++d)
                    {
                        bool commaSwitch = false;
                        for (int e = 0; e < input[a][b][c][d].size(); ++e)
                        {
                            newFile << input[a][b][c][d][e];
                            if (!commaSwitch)
                            {
                                newFile << ',';
                                commaSwitch = true;
                            }
                            else
                            {
                                newFile << ' ';
                                commaSwitch = false;
                            }
                        }
                    }
                }
                newFile << '\n';
            }
            newFile.close();
        }
    }

public:
    void createnewmodel(int input, int neurons, int layers, int output)
    {
        create_multiple_models(input, neurons, layers, output);
    }

    std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> mutate()
    {
        return mutate_batch(create_episodes(full_read_of_files(FILENAME)));
    }

    std::vector<std::vector<std::vector<double>>> run(std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> inputNetworks, std::vector<std::vector<std::vector<double>>> &SCORES)
    {
        return run_batch(inputNetworks, SCORES);
    }

    std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> selection(std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> &mutatedData, std::vector<double> &testResults)
    {
        return find_best_networks(mutatedData, testResults);
    }

    void write(std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> data)
    {
        write_data(FILENAME, data);
    }

    std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> read() {
        return full_read_of_files(FILENAME);
    }

    double ReLU(double input)
    {
        return ReLU(input);
    }
};
};