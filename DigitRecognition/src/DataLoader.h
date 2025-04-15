#ifndef DATALOADER_H
#define DATALOADER_H

#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <intrin.h>

class DataLoader {
public:
	DataLoader(const std::string& filepathImage, const std::string& filepathLables);

	std::vector<std::vector<float>> images;
	std::vector<int> labels;
	static std::vector<float> load_bmp_as_mnist_input(const std::string& filepath);

private:
	std::vector<std::vector<float>> load_mnist_images(const std::string& filepath);
	std::vector<int> load_mnist_labels(const std::string& filepath);
};

#endif