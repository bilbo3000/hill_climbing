/*
 * Hill Climing algorithm to generate 
 * Sudoku Latin Squares for a given 
 * 9X9 square. 
 * 
 * Dongpu Jin
 * 8/26/2012
 */

#include <iostream> 
#include <time.h>
#include <stdlib.h>

using namespace std; 

/*
 * Function prototypes
 */
void init(int** square, int size);
int* randomize(int* array, int size);
void swap(int* row, int a, int b);
int fitness(int** square, int row, int a, int b, int size);
void myprint(int** square, int size);
int init_fitness(int** square, int size);
int evaluate(int* array, int size);

// find time difference
double diffclock(clock_t clock1,clock_t clock2)
{
	double diffticks=clock1-clock2;
	double diffms=(diffticks*1000)/CLOCKS_PER_SEC;
	return diffms;
} 

int main(int argc, char* argv[]){
	// initialize the random seed	
	srand(time(NULL));  
	
	// declare and initialize a 9x9 matrix
	int** square;
	square = new int*[9];
	init(square, 9);
	
	/*
	 * Print out the original matrix
	 */ 
	cout << "*** Original Random Square ***" << endl; 
	myprint(square, 9);
	cout << endl;
	
	// initialize current cost
	int curr_cost = init_fitness(square, 9); 
	
	int rand_row, rand_a, rand_b, old_fitness, delta; 
	int cnt = 0; 
	
	clock_t begin = clock(); 
	// swap within a random selected row until reach zero cost
	while(curr_cost != 0){
		cnt++; // increment the counter 
		// pick two random elements in a row to be swap
		rand_row = rand() % 9;
		rand_a = rand() % 9; 
		rand_b = rand() % 9; 
		
		old_fitness = fitness(square, rand_row, rand_a, rand_b, 9);
		swap(square[rand_row], rand_a, rand_b); 
		delta = fitness(square, rand_row, rand_a, rand_b, 9) - old_fitness;
		
		// bool badmove = false;
		// if(rand() % 10000 == 0){ // probability of badmove
			// badmove = true;
		// }
		//if(delta <= 0 || badmove){	// improvement
		if(delta <= 0){
			curr_cost += delta;  // update current cost
		}else{
			swap(square[rand_row], rand_b, rand_a); // undo swap
		}
	}
	clock_t end = clock(); 
	
	cout << "*** Sudoku Square ***" << endl; 
	myprint(square, 9);
	cout << endl; 
	cout << "Total iterations: " << cnt << endl; 
	cout << "Total time: " << diffclock(end, begin) << "ms" << endl; 
 
	return 0; 
}

/*
 * Initialize and randomize the square
 */
void init(int** square, int size){
	// create an empty array 
	for(int i = 0; i < size; i++){
		square[i] = new int[size];
		for (int j = 0; j < size; j++){
            square[i][j] = 0;                     
        }
	}
	
	// an array of 9 digits, use during initialization
	int digit[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	
	// initialize the matrix such that each row satisfies
 	// distinct 1 to 9 requirements
	int* arr;
	for (int i = 0; i < size; i++){
		arr = randomize(digit, size); 
		for (int j = 0; j < size; j++){
			square[i][j] = arr[j];			
		} 	
 	}
	
	return; 
}

/*
 * Calculate the fitness of the given square
 * a, b specify two column index and row
 * specifies the row
 * 
 * param square being looking at
 * param row, looking at which row
 * param a, b, two columns to be swapped
 * param size, size of the square
 * returns the cost difference (delta)
 */
int fitness(int** square, int row, int a, int b, int size){
	// original square
	int missing = 0; 
	int* temp; 
	temp = new int[size]; 
	
	// evaluate column a
	for(int i = 0; i < size; i++){
		temp[i] = square[i][a];
	}
	missing += evaluate(temp, size);
	
	// evaluate column b
	for(int j = 0; j < size; j++){
		temp[j] = square[j][b];
	}
	missing += evaluate(temp, size);
	
	// evaluate quadrants 
	int quad_a; 
	int quad_b; 
	int quad_row; 
	
	quad_a = a / 3; 
	quad_b = b / 3; 
	quad_row = row / 3; 
	
	int index = 0; 
	for(int m = quad_row * 3; m < quad_row * 3 + 3; m++){
		for(int n = quad_a * 3; n < quad_a * 3 + 3; n++){
			temp[index] = square[m][n]; 
			index++; 
		}
	}
	missing += evaluate(temp, size);
	
	index = 0; 
	for(int p = quad_row * 3; p < quad_row * 3 + 3; p++){
		for(int q = quad_b * 3; q < quad_b * 3 + 3; q++){
			temp[index] = square[p][q]; 
			index++; 
		}
	}
	missing += evaluate(temp, size); 
	
	return missing; 
}
/*
 * Evaluate the initial square
 */
 int init_fitness(int** square, int size){
	int init_cost = 0; 
	int* temp = new int[size]; 
	
	// evaluate each column
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			temp[j] = square[j][i]; 
		}
		init_cost += evaluate(temp, size); 
	}
		
	// evaluate each quadrant
	int index; 
	for(int quad_row = 0; quad_row < 3; quad_row++){
		for(int quad_col = 0; quad_col < 3; quad_col++){
			index = 0; 
			// one quadrant
			for(int m = quad_row * 3; m < quad_row * 3 + 3; m++){
				for(int n = quad_col * 3; n < quad_col * 3 + 3; n++){
					temp[index] = square[m][n]; 
					index++; 
				}
			}
			init_cost += evaluate(temp, size);
		}
	}
	
	return init_cost; 
 }

/*
 * Evaluate the given array to find out 
 * how many digits are missing
 */
int evaluate(int* array, int size){
	int* cnt;
	cnt = new int[size]; 
	
	// initialize cnt to zero
	for(int i = 0; i < size; i++){ 
		cnt[i] = 0; 
	}
	
	// find out what digits exist
	for(int j = 0; j < size; j++){ 
		cnt[array[j] - 1]++;  // notice the - 1 here!
	}
	
	// find out what digits are missing
	int cost = 0; 
	for(int k = 0; k < size; k++){
		if (cnt[k] == 0){
			cost++; 
		}
	}
	
	return cost; 
}

/*
 * Randomize a given array
 */
int* randomize(int* array, int size){
	int* random = NULL; 
	random = new int[size];
	
	// inizialize
	for(int i = 0; i < size; i++){
		random[i] = 0; 
	} 

	// randomize
	for(int j = 0; j < size; j++){
		int index = rand() % size; 
		while (random[index] != 0){
			index = rand() % size; 
		}	
		random[index] = array[j]; 
	}

	return random; 
}

/*
 * Swap two elements in a row
 */ 
void swap(int* row, int a, int b){
	int temp; 
	temp = row[a]; 
	row[a] = row[b]; 
	row[b] = temp; 	
}

/*
 * Print out the square
 */
void myprint(int** square, int size){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			cout << square[i][j] << " "; 
		}
		cout << endl; 
	}
}
