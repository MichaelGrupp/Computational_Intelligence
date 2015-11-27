/* Your algorithm reads a number of 3 - valued training examples : each such example consists of two inputs("x" and "y" value) and a desired output value of + 1 or -1. The exact number of training examples is unknown, but you can safely assume you will read <= 1000.
At some point your program will find a training example '0,0,0\n', (note the desired output of zero, which is invalid!).This indicates that the training data is completely read, and your program should start training the neuron.

After training, your program continues to read 2 - valued evaluation data : for each such example your program should report the corresponding class (+1 or -1) followed by return ('\n') as output.
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define DIM  3	//dimension of data
#define N DIM-1 //max array index

float randomFloat() { 
	float r = (float)((rand()-rand()) % 5);
	return r;
}

typedef struct Neuron {
	float w[DIM]; //weight vector {w0 ... wDIM}
	float eta; //learning rate
} Neuron;
void initWeights(Neuron* neuron) {
	for (int i = 0; i <= N; i++)
		neuron->w[i] = randomFloat();
}
void printNeuron(Neuron* neuron) {
	printf("weights: ");
	for (int i = 0; i <= N; i++)
		printf("%f ", neuron->w[i]);
	printf("\nlearning rate: %f \n", neuron->eta);
}

float scalarProd(const float* vec1, const float* vec2, int n) {
	float result = 0.0;
	for (int i = 0; i <= n; i++)
		result += vec1[i] * vec2[i];
	return result;
}
float* sumVec(const float* vec1, const float* vec2, int n) {
	float* result = (float*)malloc(n*sizeof(float));
	for (int i = 0; i <= n; i++)
		result[i] = vec1[i] + vec2[i];
	return result;
}
float* multiplyVec(const float* vec, float scalar, int n) {
	float* result = (float*)malloc(n*sizeof(float));
	for (int i = 0; i <= n; i++)
		result[i] = vec[i] * scalar;
	return result;
}

//training step
//the raw data has the form {x1, x2, t}
//convert to form x={x0, x1, x2} and t=t
void updateWeights(Neuron* neuron, const float* rawData) {
	int t = rawData[N]; //desired class (-1, +1)
	float x[DIM]; //actual input vector
	x[0] = 1; //dummy input for w0
	for (int i = 1; i <= N; i++) {
		x[i] = rawData[i - 1]; //copy feature vector from from rawData
	}
	//update step: w(k+1) = w(k) + eta*x*t
	memcpy(neuron->w, sumVec(neuron->w, multiplyVec(multiplyVec(x, t, N), neuron->eta, N), N), DIM*sizeof(float));
}

//activation function for classification
int activate(Neuron* neuron, const float* rawData) {
	float x[DIM]; //actual input vector
	x[0] = 1; //dummy input for w0
	for (int i = 1; i <= N; i++) {
		x[i] = rawData[i-1]; //copy feature vector from from rawData
	}
	//check on which side of the decision boundary
	float a = scalarProd(neuron->w, x, N);
	if (a >= 0)
		return 1;
	else
		return -1;
}

float* parseTrainingInputLine(const char* line) {
	float result[DIM];
	int k = 0;
	for (int i = 0; i <= N; i++) {
		int j = 0;
		char tmp[20];
		while (line[k] != ',' && j < 20) {
			tmp[j] = line[k];
			j++; k++;
		};
		result[i]=strtof(tmp, NULL);
		if (line[j] == '\n')
			break;
		else
			k++;
	}
	return result;
}

float* parseTestInputLine(const char* line) {
	float result[DIM - 1];
	int k = 0;
	for (int i = 0; i <= N-1; i++) {
		int j = 0;
		char tmp[20];
		while (line[k] != ',' && j < 20) {
			tmp[j] = line[k];
			j++; k++;
		};
		result[i] = strtof(tmp, NULL);
		if (line[j] == '\n')
			break;
		else
			k++;
	}
	return result;
}

int main() {
	srand(time(NULL));
	Neuron neuron;
	initWeights(&neuron);
	neuron.eta = 0.3;

	FILE* file;
	file = fopen("C:\\Users\\Michael\\Source\\Repos\\Computational_Intelligence\\exercise_2\\testInput10A.txt", "r");
	if (file == NULL)
		return EXIT_FAILURE;

	//read training data ("x1,x2,t")
	char line[60];
	float trainingData[1000][DIM];
	float testData[1000][N];
	int i = 0; int trainingDataSize = 0; int testDataSize = 0;
	int trainingParsed = 0;
	while (1) {
		if (trainingParsed != 1 && fgets(line, 60, file)) { //fgets returns NULL at EOF (EOF in VS cmd line: enter ctrl+z enter)
			if (strcmp(line, "0,0,0\n") == 0) {
				trainingParsed = 1;
				i = 0;
			}
			else {
				memcpy(trainingData[i], parseTrainingInputLine(&line), DIM*sizeof(float));
				trainingDataSize++; i++;
			}
		}
		else if (fgets(line, 60, file) && trainingParsed == 1) {
			memcpy(testData[i], parseTestInputLine(&line), (DIM-1)*sizeof(float));
			testDataSize++; i++;
		}
		else
			break;
	}

	int trainingCycles = 100;
	for (int i = 0; i <= trainingCycles; i++) {
		for (int j = 0; j < trainingDataSize; j++) {
			updateWeights(&neuron, trainingData[j]);
		}
		printNeuron(&neuron);
	}
	
	//read test data ("x1,x2")
	//neuron.w[0] = 0.2;
	for (int i = 0; i <= testDataSize; i++) {
		int res = activate(&neuron, testData[i]);
		if(res == 1)
			printf("+%d\n", res);
		else
			printf("%d\n", res);
	}


	return EXIT_SUCCESS;
}