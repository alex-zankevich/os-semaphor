#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <string.h>
using namespace std;

class MyException{
public:
	string message;
	MyException(string msg):message(msg){}
	
};

DWORD WINAPI thread(LPVOID a){
	long* tmp = (long*)a;
	long n = tmp[1];
	for (int i = 3; i < n + 3; i++){
		long iEx = i;
		long ex = tmp[i];
		if (tmp[2] == 1){
			for (int j = i + 1; j < n + 3; j++){
				if (tmp[j] < ex){
					ex = tmp[j];
					iEx = j;
				}
			}
		}else{
			for (int j = i + 1; j < n + 3; j++){
				if (tmp[j] > ex){
					ex = tmp[j];
					iEx = j;
				}
			}
		}
		long temp = tmp[i];
		tmp[i] = tmp[iEx];
		tmp[iEx] = temp;
		ReleaseSemaphore((HANDLE)tmp[0], 1, NULL);
	}	
	return 0;
}

int main(){
	long *a;
	long n;
	long sortOrder;
	HANDLE hThread;
	DWORD IDThread;
	HANDLE hSemaphore;
	try{
		cout << "Enter array size :";
		if (!(cin >> n)){
			throw new MyException("Number format is not correct !");
		}
		cout << "Enter sorting order : \n 1 - up \n 2 - down \n";
	
		if (!(cin >> sortOrder)){
			throw new MyException("Number format is not correct !");
		}
		if (n < 0){
			throw new MyException("Number is < 0 !");
		}
		if (sortOrder != 1 && sortOrder != 2){
			throw new MyException("Sorting order is not correct !");
		}

		a = new long[n + 3];
		srand(time(0));
		for (int i = 3; i < n + 3; i++){
			a[i] = rand() % n;
		}

		hSemaphore = CreateSemaphore(NULL, 0, n, NULL);

		a[0] = (long)hSemaphore;
		a[1] = n;
		a[2] = sortOrder;

		long* showArray = new long[n];

		cout << "Source array :";
		for (int i = 3; i < n + 3; i++){
			cout << a[i] << " ";
			showArray[i - 3] = a[i];
		}

		cout << endl << endl;

		if (hSemaphore == NULL)
			return GetLastError();

		hThread = CreateThread(NULL, 0, thread, (void*)a, 0, &IDThread);

		if (hThread == NULL)
			return GetLastError();

		cout << "Sorting process :" << endl;
		for (int i = 3; i < n + 3; i++){
			cout << showArray[i - 3] << " ";
			Sleep(200);
			WaitForSingleObject(hSemaphore, INFINITE);
			cout << a[i] << endl;
			Sleep(200);
		}

		CloseHandle(hSemaphore);
		CloseHandle(hThread);
	}
	catch(MyException* e){
		cout << e->message.c_str();
	}
	return 0;

}