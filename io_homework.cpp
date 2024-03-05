#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept> 
#include <ios>      

std::vector<std::string> split(const std::string& str, const char sep) {
    std::vector<std::string> split_string;
    std::stringstream input_str(str);
    std::string item;
    while (std::getline(input_str, item, sep)) {
        split_string.push_back(item);
    }
    return split_string;
}

std::vector<std::vector<double>> parsing(const std::string& file_name, const char sep) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        throw std::ios_base::failure("Unable to open file.");
    }

    std::string line;
    if (!std::getline(file, line)) {
        throw std::runtime_error("Header line is missing.");
    }

    char file_sep = line.find(',') != std::string::npos ? ',' : '\t'; 

    if (file_sep != sep) { 
        file.close();
        throw std::runtime_error("Chosen separator does not match the separator in the file.");
    }

    std::vector<std::string> col_names = split(line, sep);

    for (auto it = col_names.begin(); it != col_names.end(); ++it) {
        std::cout << *it;
        if (it != std::prev(col_names.end())) {
            std::cout << sep;
        }
    }
    std::cout << std::endl;

    std::vector<std::vector<double>> data;
    int line_number = 1; 
    while (std::getline(file, line)) {
        std::vector<std::string> values = split(line, sep);
        std::vector<double> row;
        for (auto value : values) {
            try {
                double number = std::stod(value);
                row.push_back(number);
            }
            catch (const std::invalid_argument& e) {
                throw std::runtime_error("Invalid data format on line " + std::to_string(line_number) + ": " + value);
            }
        }
        if (row.size() != col_names.size()) {
            throw std::runtime_error("Mismatch between column count and data values on line " + std::to_string(line_number));
        }
        data.push_back(row);
        line_number++; 
    }

    file.close();

    return data;
}

void read_file(const std::string& file_name, const char sep) {
    try {
        std::vector<std::vector<double>> data = parsing(file_name, sep);

        for (const auto& row : data) {
            for (auto it = row.begin(); it != row.end(); ++it) {
                std::cout << *it;
                if (it != std::prev(row.end())) {
                    std::cout << sep;
                }
            }
            std::cout << std::endl;
        }

    }
    catch (const std::ios_base::failure& e) {
        std::cout << "Error opening or reading the file: " << e.what() << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cout << "Error parsing the file: " << e.what() << std::endl;
    }
}

char sep_choice() {
    char separator;
    std::cout << "Input separator of your file." << std::endl
        << "0 - comma (,) separator" << std::endl
        << "1 - tab (\\t) separator." << std::endl;
    std::cin >> separator;
    return separator;
}

int main() {
    std::string file_name;
    std::cout << "Input file name:" << std::endl;
    std::cin >> file_name;

    char separator;
    bool right_choice = false;
    while (!right_choice) {
        separator = sep_choice();
        switch (separator) {
        case '0':
            read_file(file_name, ',');
            right_choice = true;
            break;
        case '1':
            read_file(file_name, '\t');
            right_choice = true;
            break;
        default:
            std::cout << "Invalid input. Please enter 0 or 1." << std::endl;
            break;
        }
    }

    return 0;
}
