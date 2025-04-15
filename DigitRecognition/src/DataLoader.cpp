#include "DataLoader.h"

DataLoader::DataLoader(const std::string& filepathImage, const std::string& filepathLables) {
	images = load_mnist_images(filepathImage);
	labels = load_mnist_labels(filepathLables);
}

std::vector<std::vector<float>> DataLoader::load_mnist_images(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath);
    }

    int magic_number = 0, num_images = 0, rows = 0, cols = 0;
    file.read(reinterpret_cast<char*>(&magic_number), 4);
    file.read(reinterpret_cast<char*>(&num_images), 4);
    file.read(reinterpret_cast<char*>(&rows), 4);
    file.read(reinterpret_cast<char*>(&cols), 4);

    magic_number = _byteswap_ulong(magic_number);
    num_images = _byteswap_ulong(num_images);
    rows = _byteswap_ulong(rows);
    cols = _byteswap_ulong(cols);

    if (magic_number != 2051) {
        throw std::runtime_error("Invalid magic number in images file!");
    }

    std::vector<std::vector<float>> images(num_images, std::vector<float>(rows * cols));
    for (int i = 0; i < num_images; ++i) {
        for (int j = 0; j < rows * cols; ++j) {
            unsigned char pixel = 0;
            file.read(reinterpret_cast<char*>(&pixel), 1);
            images[i][j] = static_cast<float>(pixel) / 255.0f; // Normalizacja do [0, 1]
        }
    }
    return images;
}

std::vector<int> DataLoader::load_mnist_labels(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath);
    }

    int magic_number = 0, num_labels = 0;
    file.read(reinterpret_cast<char*>(&magic_number), 4);
    file.read(reinterpret_cast<char*>(&num_labels), 4);

    magic_number = _byteswap_ulong(magic_number);
    num_labels = _byteswap_ulong(num_labels);

    if (magic_number != 2049) {
        throw std::runtime_error("Invalid magic number in labels file!");
    }

    std::vector<int> labels(num_labels);
    for (int i = 0; i < num_labels; ++i) {
        unsigned char label = 0;
        file.read(reinterpret_cast<char*>(&label), 1);
        labels[i] = static_cast<int>(label);
    }
    return labels;
}

std::vector<float> DataLoader::load_bmp_as_mnist_input(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Nie mo¿na otworzyæ pliku BMP");
    }

    // Odczytaj nag³ówek BMP (54 bajty)
    uint8_t header[54];
    file.read(reinterpret_cast<char*>(header), 54);

    // SprawdŸ poprawnoœæ pliku BMP (nag³ówek powinien zaczynaæ siê od "BM")
    if (header[0] != 'B' || header[1] != 'M') {
        throw std::runtime_error("Plik nie jest poprawnym plikiem BMP");
    }

    // Wyodrêbnij szerokoœæ i wysokoœæ obrazu
    int width = *reinterpret_cast<int*>(&header[18]);
    int height = *reinterpret_cast<int*>(&header[22]);

    // Oblicz rozmiar danych pikseli (uwzglêdniaj¹c padding wierszy do wielokrotnoœci 4 bajtów)
    int rowPadded = (width * 3 + 3) & (~3);
    std::vector<float> pixels(width * height);

    // Odczytaj dane pikseli (od do³u do góry, ka¿dy wiersz jest odczytywany odwrotnie)
    std::vector<uint8_t> row(rowPadded);
    for (int i = height - 1; i >= 0; --i) {
        file.read(reinterpret_cast<char*>(row.data()), rowPadded);
        for (int j = 0; j < width; ++j) {
            uint8_t blue = row[j * 3];
            uint8_t green = row[j * 3 + 1];
            uint8_t red = row[j * 3 + 2];
            // Zamieñ na skalê szaroœci: Y = 0.299*R + 0.587*G + 0.114*B
            float gray = 0.299f * red + 0.587f * green + 0.114f * blue;
            // Normalizuj wartoœæ do zakresu [0, 1]
            pixels[i * width + j] = gray / 255.0f;
        }
    }

    file.close();
    return pixels;
}
