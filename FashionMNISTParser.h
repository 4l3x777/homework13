#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class FashionMNISTParser {
  std::ifstream file_stream;
  uint32_t class_data_size;

  std::pair<float, std::vector<float>> parse_csv_row(const std::string & csv_row) 
  {
    std::vector<float> class_data;
    class_data.reserve(class_data_size);
    std::stringstream ss(csv_row);
    std::string class_data_element;
    auto is_class_type = true;
    float class_type;
    while(!ss.eof()) 
    {
      std::getline(ss, class_data_element, ',');
      if(is_class_type) 
      {
        class_type = atoi(class_data_element.c_str());
        is_class_type = false;
      }
      else
      {
        class_data.push_back(atoi(class_data_element.c_str()));
      }
    }
    return std::make_pair(class_type, class_data);
  }

public:
  FashionMNISTParser(const std::string & file_path, int image_size) : class_data_size(image_size) 
  {
    file_stream.open(file_path);
    if (!file_stream.is_open()) 
    {
      std::cout << "failed to open file"<<std::endl;
    }
  }
  
  std::pair<float, std::vector<float>> get_content() 
  {
    if (!file_stream.is_open()) 
    {
      std::cout << "failed to open file"<<std::endl;
    }
    std::string csv_row;
    while(!file_stream.eof()) 
    {
      std::getline(file_stream, csv_row);
      if(csv_row.empty()) break;
      return parse_csv_row(csv_row);
    }
    return std::make_pair(-1, std::vector<float>());
  }
};
