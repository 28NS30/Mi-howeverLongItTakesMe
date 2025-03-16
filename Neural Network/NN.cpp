#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

vector<vector<vector<double>>> weights = {{{0.2, 0.6}, {0.4, 0.1}}};
vector<vector<double>> biases = {{0.7, 0.3}};
int epochs = 1;
double eta = 1e-9;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}
vector<vector<double>> forwardPass(vector<double>input){
    vector<vector<double>>res;
    res.push_back(input);
    double cur;
    for (int layer = 0; layer < weights.size(); layer ++){
        vector<double> newVector;
        for (int node = 0; node < weights[layer].size(); node++){
            cur = 0;
            for (int i = 0; i <res[layer].size(); i++){
                cur += res[layer][i]*weights[layer][node][i];
            }
            cur += biases[layer][node];
            cur = sigmoid(cur);
            newVector.push_back(cur);
        }
        res.push_back(newVector);
        newVector.clear();
    }
    return res;
}

void train(vector<vector<double>>inputs, vector<vector<double>> trueYs){
    vector<vector<double>> dError(weights.size(), vector<double>()); //derivative of the error with respect to the pre-actavtion weighted sum of a node
    // OUTPUT LAYER
    vector<vector<vector<double>>> predictions;
    for(auto tc: inputs){
        predictions.push_back(forwardPass(tc));
    }
    // OUTPUT LAYER
    for (int epoch = 0; epoch < epochs; epoch++){
        // Runs for set number of epochs/training cycle
        for (int dataPoint = 0; dataPoint < predictions.size(); dataPoint++){
            // Stochastic GD -> updates every data point
            for (int layer = weights.size()-1; layer>=0; layer--){
                double grad;
                if (layer == weights.size()-1){
                    // OUTPUT LAYER
                    for (int node = 0; node < weights[layer].size(); node++){
                        grad = (predictions[dataPoint].back()[node] - trueYs[dataPoint][node])   *   (predictions[dataPoint].back()[node] * (1 - predictions[dataPoint].back()[node]));
                        dError[layer].push_back(grad);
                        for (int weight = 0; weight < weights[layer][node].size(); weight++){
                                // Update Weight
                                weights[layer][node][weight] -= predictions[dataPoint][layer][node]*eta*grad;
                            }
                            // Update Bias
                            biases[layer][node] -= eta*grad;
                    } 
                }
                else{
                    // Calculate Gradient For Each node
                    for (int curNode = 0; curNode<weights[layer].size(); curNode++){
                        grad = 0;
                        for (int nextNode = 0; nextNode < weights[layer+1].size(); nextNode++){
                            grad += weights[layer+1][nextNode][curNode] * dError[layer+1][nextNode];
                            }
                        dError[layer].push_back(grad);
                    }
                    
                    
                    // Gradient = Derivative of the Loss with respect to the preactivation output of the node
                    for (int node = 0; node < weights[layer].size(); node++){
                        for (int weight = 0; weight < weights[layer][node].size(); weight++){
                            // Update Weight
                            weights[layer][node][weight] -= predictions[dataPoint][layer][node]*eta*dError[layer][node];
                        }
                        // Update Bias
                        biases[layer][node] -= eta*dError[layer][node];
                    }}
            }
        }
    }
}
int main(){

}