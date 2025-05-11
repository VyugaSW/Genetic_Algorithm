#include "FilePoints.h"

using namespace std;

template <typename T>
vector<Point<T>> FilePoints<T>::readFromFile(string filename){
    vector<Point<T>> points;
    ifstream in(filename);
    if (!in.is_open())
        throw runtime_error("Could not open the file.");

    string line;
    while (getline(in, line)) {
        istringstream iss(line);
        vector<T> coords;
        T value;

        while (iss >> value) {
            coords.push_back(value);
        }

        if (!coords.empty())
            points.emplace_back(coords);
        else
            cerr << "Error while reading line: " << line << "\n";
    }

    return points;     
}

template <typename T>
void FilePoints<T>::writeToFile(const std::vector<Point<T>> &points, std::string &filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    try {
        for (const auto& point : points) {
            for (size_t i = 0; i < point.dimension(); ++i) {
                out << point.coors[i];
                if (i != point.dimension() - 1) {
                    out << " "; 
                }
            }
            out << "\n";
        }

        
        if (out.fail()) {
            throw std::runtime_error("Write error occurred");
        }
    }
    catch (const std::exception& e) {
        out.close();
        throw std::runtime_error("Error while writing to file: " + std::string(e.what()));
    }

    out.close();
}

template <typename T>
void FilePoints<T>::writeClustersWithLabels(const std::vector<Point<T>>& points, 
                                      const std::vector<size_t>& labels,
                                      const std::vector<Point<T>> &centroids,
                                      const size_t& num_clusters,
                                      const std::string& filename,
                                      const double WCSS) {
        std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    try {
        const size_t dim = points[0].dimension();

        out << "# Cluster grouping | Format: ";
        for (size_t d = 0; d < dim; ++d) {
            out << "coord" << d+1 << " ";
        }
        out << "| cluster_id\n\n";
        out << "WSCC is " << WCSS << "\n\n";

        for (size_t cluster = 0; cluster < num_clusters; ++cluster) {
            out << "# Cluster " << cluster << "\n";

            out << "Centroid: ";
            for (size_t d = 0; d < dim; ++d) {
                out << centroids[cluster].coors[d] << ", ";
            }
            out << "\n";
            
            for (size_t i = 0; i < points.size(); ++i) {
                if (labels[i] == cluster) {
                    for (size_t d = 0; d < dim; ++d) {
                        out << points[i].coors[d] << ", ";
                    }
                    out << "\n";
                }
            }
            out << "\n";  
        }

        if (!out.good()) {
            throw std::runtime_error("Error during file writing");
        }
    }
    catch (...) {
        out.close();
        throw;
    }
    out.close();
}


template class FilePoints<double>;
template class FilePoints<int>;
