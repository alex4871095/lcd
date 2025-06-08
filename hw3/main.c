#define SIZE 10

void array_fill(int array[], int n);
void array_output(int array[], int n);
void array_sort(int array[], int n);
int array_write(int array[], int n);
int array_read(int array[], int n);

int arr[SIZE];

int main(int argc, char* argv)
{
	array_fill(arr, SIZE);
	array_write(arr, SIZE);
	array_read(arr, SIZE);
	array_output(arr, SIZE);
	array_sort(arr, SIZE);
	array_output(arr, SIZE);
	return 0;
}

