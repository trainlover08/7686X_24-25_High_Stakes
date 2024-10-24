#ifndef OPTIMIZERS_HPP
#define OPTIMIZERS_HPP

#include "MatrixOperations.hpp"
#include <cmath>
#include <vector>
#include <stdexcept>

using Matrix = std::vector<std::vector<double>>;

// Adam Optimizer Class
class Adam {
private:
    double lr;  // Learning rate
    double beta1;
    double beta2;
    double epsilon;
    Matrix m;  // First moment vector (mean of gradients)
    Matrix v;  // Second moment vector (uncentered variance of gradients)
    int t;     // Time step (for bias correction)

    void init_m_and_v(const Matrix& gradients) {
        int rows = gradients.size();
        int cols = gradients[0].size();
        m = Matrix(rows, std::vector<double>(cols, 0.0));
        v = Matrix(rows, std::vector<double>(cols, 0.0));
    }

public:
    Adam(double lr_, double beta1_, double beta2_, double epsilon_ = 1e-8)
        : lr(lr_), beta1(beta1_), beta2(beta2_), epsilon(epsilon_), t(0) {}

    Matrix update(const Matrix& weights, const Matrix& gradients) {
        if (m.empty() || v.empty()) {
            init_m_and_v(gradients);
        }

        // Check for dimension match between weights and gradients
        if (weights.size() != gradients.size() || weights[0].size() != gradients[0].size()) {
            throw std::invalid_argument("Matrix dimensions do not match between weights and gradients.");
        }

        t++;  // Increment time step

        // Update biased first moment estimate
        m = add(elementWiseMultiply(m, {{beta1}}), elementWiseMultiply(gradients, {{1 - beta1}}));

        // Update biased second moment estimate
        Matrix gradientsSquared = elementWiseMultiply(gradients, gradients);
        v = add(elementWiseMultiply(v, {{beta2}}), elementWiseMultiply(gradientsSquared, {{1 - beta2}}));

        // Bias correction for first and second moments
        Matrix m_hat = elementWiseMultiply(m, {{1.0 / (1 - std::pow(beta1, t))}});
        Matrix v_hat = elementWiseMultiply(v, {{1.0 / (1 - std::pow(beta2, t))}});

        // Update the weights
        Matrix step_size = elementWiseMultiply(m_hat, {{lr}});
        step_size = elementWiseDivide(step_size, addScalar(elementWiseSqrt(v_hat), epsilon));

        return subtract(weights, step_size);  // Update weights
    }
};

// AdamW Optimizer Class
class AdamW {
private:
    double lr;        // Learning rate
    double beta1;
    double beta2;
    double epsilon;
    double weight_decay;
    Matrix m;         // First moment vector
    Matrix v;         // Second moment vector
    int t;            // Time step

    void init_m_and_v(const Matrix& gradients) {
        int rows = gradients.size();
        int cols = gradients[0].size();
        m = Matrix(rows, std::vector<double>(cols, 0.0));
        v = Matrix(rows, std::vector<double>(cols, 0.0));
    }

public:
    AdamW(double lr_, double beta1_, double beta2_, double epsilon_ = 1e-8, double weight_decay_ = 0.01)
        : lr(lr_), beta1(beta1_), beta2(beta2_), epsilon(epsilon_), weight_decay(weight_decay_), t(0) {}

    Matrix update(const Matrix& weights, const Matrix& gradients) {
        if (m.empty() || v.empty()) {
            init_m_and_v(gradients);
        }

        // Check for dimension match between weights and gradients
        if (weights.size() != gradients.size() || weights[0].size() != gradients[0].size()) {
            throw std::invalid_argument("Matrix dimensions do not match between weights and gradients.");
        }

        t++;  // Increment time step

        // Update biased first moment estimate
        m = add(elementWiseMultiply(m, {{beta1}}), elementWiseMultiply(gradients, {{1 - beta1}}));

        // Update biased second moment estimate
        Matrix gradientsSquared = elementWiseMultiply(gradients, gradients);
        v = add(elementWiseMultiply(v, {{beta2}}), elementWiseMultiply(gradientsSquared, {{1 - beta2}}));

        // Bias correction for first and second moments
        Matrix m_hat = elementWiseMultiply(m, {{1.0 / (1 - std::pow(beta1, t))}});
        Matrix v_hat = elementWiseMultiply(v, {{1.0 / (1 - std::pow(beta2, t))}});

        // Weight decay
        Matrix decayed_weights = elementWiseMultiply(weights, {{weight_decay}});

        // Update the weights with AdamW
        Matrix step_size = elementWiseMultiply(m_hat, {{lr}});
        step_size = elementWiseDivide(step_size, addScalar(elementWiseSqrt(v_hat), epsilon));

        return subtract(weights, add(step_size, decayed_weights));  // Update weights with weight decay
    }
};

#endif