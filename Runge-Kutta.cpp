#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>



const double stddev = 0.05;


struct Vector2D {
    double x, y;
    
    Vector2D(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& other) const {
        return {x + other.x, y + other.y};
    }

    Vector2D operator*(double scalar) const {
        return {x * scalar, y * scalar};
    }

    Vector2D operator-(const Vector2D& other) const {
        return {x - other.x, y - other.y};
    }
};
Vector2D calculateVelocityAtPosition(double x, double y, const std::vector<Vector2D>& waveVectors, const std::vector<double>& phases);

struct Particle {
    double x, y;

    double diffusion;  // Add diffusion attribute

    // Constructor with diffusion parameter
    Particle(double x = 0.0, double y = 0.0, double diffusion = 0.1) : x(x), y(y), diffusion(diffusion) {}



    // Getter for position
    Vector2D position() const {
        return Vector2D(x, y);
    }

    // Setter for position
    void setPosition(const Vector2D& newPos) {
        x = newPos.x;
        y = newPos.y;
    }

    // Apply a Brownian step to the particle, including constant velocity effect
    void applyBrownianStep(double stepStdDev, double timeStep, double diffusion, double constantVelocityX, std::mt19937& gen) {
        std::normal_distribution<> stepDist(0.0, stepStdDev);
        x += stepDist(gen) * std::sqrt(2 * diffusion * timeStep) + constantVelocityX * timeStep; 
        y += stepDist(gen) * std::sqrt(2 * diffusion * timeStep);
    }

    void applyVelocityField(const std::vector<Vector2D>& waveVectors, const std::vector<double>& phases, double timeStep) {
        Vector2D velocity = calculateVelocityAtPosition(x, y, waveVectors, phases);
        x += velocity.x * timeStep;
        y += velocity.y * timeStep;
    }
};






Vector2D calculateVelocityAtPosition(double x, double y, const std::vector<Vector2D>& waveVectors, const std::vector<double>& phases) {
    Vector2D velocity(0.0, 0.0);
    for (size_t i = 0; i < waveVectors.size(); ++i) {
        Vector2D k = waveVectors[i];
        double phase = phases[i];
        double magnitude = std::sqrt(k.x * k.x + k.y * k.y);
        if (magnitude > 0.0) {
            Vector2D unitK = k * (1 / magnitude);
            Vector2D orthoK(-unitK.y, unitK.x);
            double contribution = std::cos(k.x * x + k.y * y + phase);
            velocity = velocity + orthoK * contribution;
        }
    }
    return velocity * stddev;
}

//now I need to add the movements from the contours

std::vector<Particle> generateGaussianParticles(int n, double mean, double stddev, std::mt19937& gen) {
    std::normal_distribution<> dist(mean, stddev);
    std::vector<Particle> particles;
    for (int i = 0; i < n; ++i) {
        Particle p;
        p.x = dist(gen);
        p.y = dist(gen);
        particles.push_back(p);
    }
    return particles;
}




void logParticlePositions(const std::vector<Particle>& particles, double currentTime, std::ofstream& positionOutFile) {
    if (positionOutFile.is_open()) {
        positionOutFile << currentTime;
        for (const auto& p : particles) {
            positionOutFile << "," << p.x << "," << p.y;
        }
        positionOutFile << std::endl;
    }
}

void calculateAndPrintStats(const std::vector<Particle>& particles, int N, double currentTime, std::ofstream& varianceOutFile) {
    double sumX = 0, sumY = 0;
    for (const auto& p : particles) {
        sumX += p.x;
        sumY += p.y;
    }
    double avgX = sumX / N;
    double avgY = sumY / N;

    double sumSqX = 0, sumSqY = 0;
    for (const auto& p : particles) {
        sumSqX += (p.x - avgX) * (p.x - avgX);
        sumSqY += (p.y - avgY) * (p.y - avgY);
    }
    double varianceX = sumSqX / N;
    double varianceY = sumSqY / N;

    // Calculate dispersion coefficients as the square root of the variances
    double dispersionCoefficientX = std::sqrt(varianceX);
    double dispersionCoefficientY = std::sqrt(varianceY);

    // Output variance and dispersion data to the CSV file
    if (varianceOutFile.is_open()) {
        varianceOutFile << currentTime << "," << varianceX << "," << varianceY << "," 
                        << dispersionCoefficientX << "," << dispersionCoefficientY << std::endl;
    }
}




// Implementing the Extended Runge-Kutta method for particles
void applyExtendedRungeKutta(Particle& particle, const std::vector<Vector2D>& waveVectors, const std::vector<double>& phases, double timeStep, std::mt19937& gen) {
  
    const double alpha[3] = {0.66275881, 0.00733232, 0.24997509};
    const double beta[3] = {0.81410000, 0.66311191, 0.37079131};

    // random variables for noise
    std::normal_distribution<> noiseDist(0.0, 1.0);

    // Intermediate variables
    Vector2D intermediatePos = particle.position();
    Vector2D velocity;
    double noise;

    for (int i = 0; i < 3; i++) {
        // Calculate the deterministic component
        velocity = calculateVelocityAtPosition(intermediatePos.x, intermediatePos.y, waveVectors, phases);
        
        // Calculate the stochastic component -----noise
        noise = noiseDist(gen) * sqrt(2.0 * particle.diffusion * timeStep);

        // Update position using Extended Runge-Kutta formula
        intermediatePos.x += alpha[i] * velocity.x * timeStep + beta[i] * noise;
        intermediatePos.y += alpha[i] * velocity.y * timeStep;
    }

    // Update particle's position
    particle.setPosition(intermediatePos);
}

int main() {
    // Initialize particles, wave vectors, etc.

    // During the simulation loop:
   const int N = 1000; // Number of particles
    const double meanPosition = 0.0; // Mean position for initial Gaussian distribution
    const double stddevPosition = 1.0; // Standard deviation for initial Gaussian distribution
    const double diffusion = 0.1; // Diffusion coefficient
    const double constantVelocityX = 1.0; // Constant velocity in x-direction
    const double timeStep = 0.01; // Time step for the simulation
    const double endTime = 10.0; // End time for the simulation
    const int numberOfWaveVectors = 20; // Number of wave vectors

    std::random_device rd;
    std::mt19937 gen(rd()); // Random number generator

    std::vector<Vector2D> waveVectors(numberOfWaveVectors);
    std::vector<double> phases(numberOfWaveVectors);
    std::uniform_real_distribution<> uniformDist(0.0, 2 * M_PI); // Uniform distribution for phases

    // Generate random wave vectors and phases
    for (int i = 0; i < numberOfWaveVectors; ++i) {
        double angle = uniformDist(gen);
        waveVectors[i] = Vector2D(cos(angle), sin(angle)); // Unit vectors in random directions
        phases[i] = uniformDist(gen);
    }

    // Generate particles
    std::vector<Particle> particles = generateGaussianParticles(N, meanPosition, stddevPosition, gen);

    // File output for logging
    std::ofstream varianceOutFile("variance_dataRunge.csv", std::ios::out);
    if (!varianceOutFile.is_open()) {
        std::cerr << "Failed to open variance data output file." << std::endl;
        return 1;
    }
    std::ofstream positionOutFile("particle_positionsRunge.csv", std::ios::out);
    if (!positionOutFile.is_open()) {
        std::cerr << "Failed to open particle positions output file." << std::endl;
        return 1;
    }


    // Simulation loop
    for (double currentTime = 0; currentTime <= endTime; currentTime += timeStep) {
        for (auto& particle : particles) {
            particle.applyBrownianStep(stddev, timeStep, diffusion, constantVelocityX, gen);
            particle.applyVelocityField(waveVectors, phases, timeStep);
            applyExtendedRungeKutta(particle, waveVectors, phases, timeStep, gen);
        }
        logParticlePositions(particles, currentTime, positionOutFile);

       
            calculateAndPrintStats(particles, N, currentTime, varianceOutFile);
        
    }

    varianceOutFile.close(); // Ensure to close the file properly
    positionOutFile.close();
    return 0;
}
//what should you find in the velocity field?? 
//plot the particles in the velocity field with Runj Kunta
//and show the result to Prof. Barros 
