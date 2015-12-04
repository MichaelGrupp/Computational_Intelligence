/* Your algorithm reads a number of 3 - valued training examples : each such example consists of two inputs("x" and "y" value)
and a desired output value of + 1 or -1. The exact number of training examples is unknown, but you can safely assume you will read <= 1000.
At some point your program will find a training example '0,0,0\n', (note the desired output of zero, which is invalid!).
This indicates that the training data is completely read, and your program should start training the neuron.

After training, your program continues to read 2 - valued evaluation data : for each such example your program should report 
the corresponding class (+1 or -1) followed by return ('\n') as output.
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define DIM  3	//dimension of data
#define N DIM-1 //max array index
#define NUM_NEURONS 1 //number of neurons

double randomFloat() { 
	double r = (double)(0.1+(rand()-rand()) % 1);
	return r;
}

typedef struct Neuron {
	double w[DIM]; //weight vector {w0 ... wDIM}
	double eta; //learning rate
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

double scalarProd(const double* vec1, const double* vec2, int n) {
	double result = 0.0;
	for (int i = 0; i <= n; i++)
		result += vec1[i] * vec2[i];
	return result;
}
double* vectorProd(const double* vec1, const double* vec2, int n) {
	double* result = (double*)malloc(n*sizeof(double));
	for (int i = 0; i <= n; i++)
		result[i] = vec1[i] * vec2[i];
	return result;
}
double* sumVec(const double* vec1, const double* vec2, int n) {
	double* result = (double*)malloc(n*sizeof(double));
	for (int i = 0; i <= n; i++)
		result[i] = vec1[i] + vec2[i];
	return result;
}
double* multiplyVec(const double* vec, double scalar, int n) {
	double* result = (double*)malloc(n*sizeof(double));
	for (int i = 0; i <= n; i++)
		result[i] = vec[i] * scalar;
	return result;
}
double* tanhVec(const double* vec, int n) {
	double* result = (double*)malloc(n*sizeof(double));
	for (int i = 0; i <= n; i++)
		result[i] = tanh(vec[i]);
	return result;
}

//activation function
double f(Neuron* neuron, double* in) {
	return tanh(scalarProd(neuron->w, in, N));
}
//derivative of activation function 1/(cosh(x)^2)
double fdx(Neuron* neuron, double* in) {
	return 1 / pow(cosh(scalarProd(neuron->w, in, N)), 2);
}
//error
double error(double out, double t) {
	return t - out;
}

//training step
//the raw data has the form {x1, x2, t}
//convert to form x={x0, x1, x2} and t=t
double updateWeights(Neuron* neuron, const double* rawData) {
	int t = rawData[N]; //desired class (-1, +1)
	double x[DIM]; //actual input vector
	x[0] = 1; //dummy input for w0
	for (int i = 1; i <= N; i++) {
		x[i] = rawData[i - 1]; //copy feature vector from from rawData
	}
	//perceptron update step: w(k+1) = w(k) + eta*x*t
	//memcpy(neuron->w, sumVec(neuron->w, multiplyVec(multiplyVec(x, t), neuron->eta)), DIM*sizeof(double));
	//neuron update step: delta rule (w(k+1) = w(k) + (error*w(k) * (fdx(w(k)*in) * in)
	double out = f(neuron, x);
	double e = error(out, t);
	double f_dx = fdx(neuron, &x);
	double* a = vectorProd(multiplyVec(neuron->w, e, N), multiplyVec(neuron->w, f_dx, N), N);
	memcpy(neuron->w, sumVec(neuron->w, (a), N) , DIM*sizeof(double));
	return e;
}

//activate the neuron
double activate(Neuron* neuron, const double* rawData) {
	double x[DIM]; //actual input vector
	x[0] = 1; //dummy input for w0
	for (int i = 1; i <= N; i++) {
		x[i] = rawData[i-1]; //copy feature vector from from rawData
	}
	//perceptron: check on which side of the decision boundary
	/* double a = scalarProd(neuron->w, x);
	if (a >= 0)
		return 1;
	else
		return -1; */
	//neuron: out = f(net) = f(w*input) = tanh(w*input)
	return f(neuron, x);
}

double* parseTrainingInputLine(const char* line) {
	double result[DIM];
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

double* parseTestInputLine(const char* line) {
	double result[DIM - 1];
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
	neuron.eta = 0.5;

	FILE* file;
	file = fopen("C:\\Users\\Michael\\Source\\Repos\\Computational_Intelligence\\exercise_2\\testInput10A.txt", "r");
	if (file == NULL)
		return EXIT_FAILURE;

	//read training data ("x1,x2,t")
	char line[60];
	double trainingData[1000][DIM];
	double testData[1000][N];
	int i = 0; int trainingDataSize = 0; int testDataSize = 0;
	int trainingParsed = 0;
	while (1) {
		if (trainingParsed != 1 && fgets(line, 60, file)) { //fgets returns NULL at EOF (EOF in VS cmd line: enter ctrl+z enter)
			if (strcmp(line, "0,0,0\n") == 0) {
				trainingParsed = 1;
				i = 0;
			}
			else {
				memcpy(trainingData[i], parseTrainingInputLine(&line), DIM*sizeof(double));
				trainingDataSize++; i++;
			}
		}
		else if (fgets(line, 60, file) && trainingParsed == 1) {
			memcpy(testData[i], parseTestInputLine(&line), (DIM-1)*sizeof(double));
			testDataSize++; i++;
		}
		else
			break;
	}

	int trainingCycles = 0;
	double MSE = 1;
	double eSum = 0;
	while (MSE > 0.0001) {
		for (int j = 0; j < trainingDataSize; j++) {
			eSum += pow(updateWeights(&neuron, trainingData[j]), 2); //squared error
		}

		MSE = eSum * (0.5 / (NUM_NEURONS*trainingDataSize)); //mean square error (see BILHR doc by Peer)
		printf("MSE: %f\n", MSE);
		//printNeuron(&neuron);
		eSum = 0;
		trainingCycles++;
	}
	
	//read test data ("x1,x2")
	//neuron.w[0] = 0.2;
	for (int i = 0; i <= testDataSize; i++) {
		double res = activate(&neuron, testData[i]);
		if(res == 1)
			printf("+%f\n", res);
		else
			printf("%f\n", res);
	}


	return EXIT_SUCCESS;
}