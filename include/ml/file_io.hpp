#include "../ai/neural_network.cpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class File_IO {
public:
    void write (std::vector<std::vector<double>> data, std::string filename) {
        std::stringstream ss;

        //mutex.take();
        for (int i = 0; i < data.size(); i++) {
            for (int j = 0; j < data[i].size(); j++) {
                ss << data[i][j] << " ";
            }
            ss << '\n';
        }
        //mutex.give();

        //mutex.take();
        std::ofstream file(filename);
        file << ss.str();
        file.close();
        //mutex.give();
    }

    std::vector<std::vector<double>> read (std::string filename) {
        std::vector<std::vector<double>> data;
        std::ifstream inFile(filename);

        //mutex.take();
        std::string line;
        while (std::getline(inFile, line)) {
            std::istringstream iss(line);
            std::vector<double> row;
            double value;
            while (iss >> value) {
                row.push_back(value);
            }
            data.push_back(row);
        }
        inFile.close();
        //mutex.give();

        return data;
    }
    
private:
    //pros::Mutex mutex;
};