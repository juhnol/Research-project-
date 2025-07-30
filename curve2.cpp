#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <cassert>
#include <fstream>

const double PI = 3.14159265358979323846;
const int GRID_SIZE = 500; // Grid size for a 2D simulation
double l = 5;       //before GRID_SIZE 50; 
const double MEAN_VELOCITY_U = 1; //before 1 // Mean velocity in the e1 direction
const double SD = 1 / l;  //before .5
//variance has to be 1 in that 
//sigma is 1 
//IF l is 5, then u bar has to be equal to 5 
//also do quiver plot of u and v 
//
//ratio between l and u bar has to be identical 


// 2D vector to represent points or velocities
struct Vector2D {
    double x, y;
    
    Vector2D(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    // Operator overloading for vector addition
    Vector2D operator+(const Vector2D& other) const {
        return {x + other.x, y + other.y};
    }

    // Operator overloading for scalar multiplication
    Vector2D operator*(double scalar) const {
        return {x * scalar, y * scalar};
    }

    // Operator overloading for vector subtraction
    Vector2D operator-(const Vector2D& other) const {
        return {x - other.x, y - other.y};
    }
};

// Function to generate a random wave vector with Gaussian distribution
Vector2D generateRandomWaveVector(std::mt19937& gen, std::normal_distribution<>& gaussian_dist) {
    return {gaussian_dist(gen), gaussian_dist(gen)};
}

// Function to generate a random phase with Uniform distribution
double generateRandomPhase(std::mt19937& gen, std::uniform_real_distribution<>& uniform_dist) {
    return uniform_dist(gen);
}

// Function to calculate divergence in 2D
double calculateDivergence(int i, int j, const std::vector<std::vector<Vector2D>>& velocityField, double delta_x, double delta_y) {
    
    if (i == 0 || j == 0 || i == GRID_SIZE - 1 || j == GRID_SIZE - 1) {
        return 0.0; // Assuming zero divergence at boundaries or handle appropriately
    }

    double dU_dx = (velocityField[i + 1][j].x - velocityField[i - 1][j].x) / (2.0 * delta_x);
    double dV_dy = (velocityField[i][j + 1].y - velocityField[i][j - 1].y) / (2.0 * delta_y);
    return dU_dx + dV_dy;
}

// Main program
int main() {
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::normal_distribution<> gaussian_dist(0.0, 1.0 / pow(l, 2));
    std::uniform_real_distribution<> uniform_dist(0.0, 2.0 * PI);

    int N;
    std::cout << "Enter the number of modes: ";
    std::cin >> N;
    assert(N > 0);  // Ensure N is positive

    std::vector<Vector2D> waveVectors(N);
    std::vector<double> phases(N);

 

    // Generate wave vectors and phases for N modes
    //SUMMATION PART OF FORMULA!!!
    for (int i = 0; i < N; ++i) {
        waveVectors[i] = generateRandomWaveVector(gen, gaussian_dist);
        phases[i] = generateRandomPhase(gen, uniform_dist);
    }

    // Create velocity field
    


// Main simulation loop for each mode
    for(int mode = 1; mode <= N; ++mode) {
        std::vector<std::vector<Vector2D>> velocityField(GRID_SIZE, std::vector<Vector2D>(GRID_SIZE));

        // Iterate over each point in the grid
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                Vector2D position(i, j);
                Vector2D velocity(MEAN_VELOCITY_U, 0.0); // Start with the mean flow velocity

                // Add the contribution from each mode
                for (int n = 0; n < mode; ++n) {
                    double phase = phases[n];
                    Vector2D kvec = waveVectors[n];
                    double k_magnitude_squared = kvec.x * kvec.x + kvec.y * kvec.y;

                    const double k_threshold = 1e-10; // Correct the spelling
                    if (k_magnitude_squared > k_threshold) {
                        Vector2D p(-kvec.y, kvec.x); // p vector orthogonal to k
                        double dotProd = kvec.x * position.x + kvec.y * position.y;
                        // Add divergence-free mode contribution
                        velocity = velocity + p * (std::cos(dotProd + phase) / std::sqrt(k_magnitude_squared));
                    }
                }
                velocity = velocity * (SD * MEAN_VELOCITY_U * std::sqrt(2.0 / mode));


                // Normalize the contribution from all modes to achieve the desired variance
                //if (mode > 0) {
                   // velocity = velocity * (std::sqrt(VARIANCE / mode));
                //}

                // Store the computed velocity in the velocity field
                velocityField[i][j] = velocity;
            }
        }

        // Output the velocity field to a file for the current mode
        std::string filename = "velocityField_mode_" + std::to_string(mode) + ".csv";
        std::ofstream outFile(filename);
        if (outFile.is_open()) {
            for (int i = 0; i < GRID_SIZE; ++i) {
                for (int j = 0; j < GRID_SIZE; ++j) {
                    outFile << velocityField[i][j].x << "," << velocityField[i][j].y;
                    if (j != GRID_SIZE - 1) {
                        outFile << ",";
                    }
                }
                outFile << "\n"; 
            }
            outFile.close();
        } else {
            std::cerr << "Unable to open file " << filename << " for writing." << std::endl;
        }
        double delta_x = 30.0; 
        double delta_y = 30.0;
        bool isMassConserved = true;
        const double divergenceThreshold = .001; // Set a threshold for numerical zero
        double maxDivergence = 0.0; // Variable to store the maximum divergence
        double totalDivergence = 0.0; // Variable to store the sum of all divergences

        for (int i = 1; i < GRID_SIZE - 1; ++i) {
            for (int j = 1; j < GRID_SIZE - 1; ++j) {
                double divergence = calculateDivergence(i, j, velocityField, delta_x, delta_y);
                totalDivergence += std::abs(divergence); // Sum the absolute values of divergence
                if (std::abs(divergence) > maxDivergence) {
                    maxDivergence = std::abs(divergence); // Update max divergence if current is larger
                }
                if (std::abs(divergence) > divergenceThreshold) {
                    isMassConserved = false;
                    // break; 
                }
            }
        }
        std::cout << "Maximum divergence in the field: " << maxDivergence << std::endl;
        std::cout << "Total divergence in the field: " << totalDivergence << std::endl;

        if (isMassConserved) {
            std::cout << "Mass is conserved in the velocity field." << std::endl;
        } 
        else {
            std::cout << "Mass is not conserved in the velocity field." << std::endl;
        } 
        std::string divFilename = "divergenceField_mode_" + std::to_string(mode) + ".csv";
        std::ofstream divOutFile(divFilename);
        if (divOutFile.is_open()) {
            for (int i = 0; i < GRID_SIZE; ++i) {
                for (int j = 0; j < GRID_SIZE; ++j) {
                    double divergence = calculateDivergence(i, j, velocityField, delta_x, delta_y);
                    divOutFile << divergence;
                    if (j != GRID_SIZE - 1) {
                        divOutFile << ",";
                    }
                }
                divOutFile << "\n";
            }
            divOutFile.close();
        } else {
            std::cerr << "Unable to open file " << divFilename << " for writing." << std::endl;
        }


    }
    

    // Check for mass conservation
  
      
    
    return 0;
}


//next mission 
//flow field - DONE
//PLOTTING, Export the data to Matlab, or python. 
//Streamline plots, and Contour plots
//plot the modulus or the absolute value of the speed. 

//Do In matlab!!!

//when doing plots, label x and y in nice plots. 
//LOOK INTO RANDOM WALKS and Brownian Motion
//Meeting monday Nov, 6 at 1:30!!
//
