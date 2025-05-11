#include "GeneticClustering.h"
#include "FilePoints.h"
#include "Point.h"
#include <random>

using namespace std;


int main() {
    
    FilePoints<double> fp;
    std::vector<Point<double>> data = fp.readFromFile("input/Mall_Customers.txt");

    // Parameters for experiments
    const std::vector<size_t> population_sizes = {50, 100, 200};
    const std::vector<double> crossover_rates = {0.7, 0.8, 0.9};
    const std::vector<double> mutation_rates = {0.01, 0.05, 0.1};
    const std::vector<size_t> clusters_k = {2,3,4,5,6};  // For data with n clusters
    const size_t max_generations = 100;
    const size_t num_runs = 10;  // Number of independent runs for each parameter set

    // Best solution from all runs
    Individual<double> best_solution_all;
    double best_wcss_all = std::numeric_limits<double>::max();

    // Iterate through all parameter combinations
    for(size_t k_c: clusters_k){
        for (size_t pop_size : population_sizes) {
            for (double cross_rate : crossover_rates) {
                for (double mut_rate : mutation_rates) {
                    std::cout << "\nTesting parameters: clusters= " << k_c
                            << ", pop_size=" << pop_size 
                            << ", cross_rate=" << cross_rate 
                            << ", mut_rate=" << mut_rate << std::endl;

                    // Best solution for current parameter set
                    Individual<double> best_solution_params;
                    double best_wcss_params = std::numeric_limits<double>::max();

                    // 10 independent runs for current parameter set
                    for (size_t run = 0; run < num_runs; ++run) {
                        std::cout << "Run " << run + 1 << "/" << num_runs << "... ";

                        // Create and configure genetic algorithm
                        GeneticClustering<double> gc(pop_size, max_generations, cross_rate, mut_rate, k_c);
                        gc.set_stop_conditions(max_generations, 20, 0.01, 0.0);
                        
                        // Run the algorithm
                        gc.fit(data);
                        
                        // Get best solution for this run
                        auto current_solution = gc.get_best_solution();
                        double current_wcss = gc.getBestWCSS();
                        
                        std::cout << "WCSS: " << current_wcss << std::endl;

                        // Update best solution for current parameters
                        if (current_wcss < best_wcss_params) {
                            best_wcss_params = current_wcss;
                            best_solution_params = current_solution;
                        }
                    }

                    // Output results for current parameter set
                    std::cout << "Best WCSS for these parameters: " << best_wcss_params << std::endl;

                    // Update global best solution
                    if (best_wcss_params < best_wcss_all) {
                        best_wcss_all = best_wcss_params;
                        best_solution_all = best_solution_params;
                    }
                }
            }
        }

        // Output final results
        std::cout << "\n\n=== FINAL RESULTS ===" << std::endl;
        std::cout << "Best WCSS from all runs: " << best_wcss_all << std::endl;
        std::cout << "Best centroids:" << std::endl;
        for (const auto& centroid : best_solution_all.centroids) {
            std::cout << "(";
            for (size_t i = 0; i < centroid.coors.size(); ++i) {
                std::cout << centroid.coors[i];
                if (i < centroid.coors.size() - 1) std::cout << ", ";
            }
            std::cout << ")" << std::endl;
        }

        std::string outFileName = "output/" + std::to_string(k_c) + "_RealClustering_Bestresult.txt";
        fp.writeClustersWithLabels(data, best_solution_all.labels, best_solution_all.centroids, k_c, outFileName, best_wcss_all);
    }

    return 0;
}