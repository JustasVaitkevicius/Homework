#include <iostream>
#include <string>
#include <fstream>
#include <chrono> //milliseconds
#include <thread> //sleep until
#include <vector>

using namespace std;
using namespace this_thread; 
using namespace chrono; 

void do_things()
{
	ifstream inputfile("/remote_server/share/file.txt"); //input file.txt path
	bool reverseline = false; //status when found pattern
	string line, backupline; //line - for reading from file, backupline - save last scanned value if the connection closed
	vector <string> failoutput; //all scanned values, when there is no connetion with server

	while (getline(inputfile, line)) {
		ofstream outputfile("/remote_server2/share/file.txt", ios::app); //output file.txt path

		if (reverseline == true) {
			reverse(line.begin(), line.end()); //reverse line
			reverseline = false;
		}

		if (outputfile.is_open()) { //when output file.txt is open

			while (!failoutput.empty()) { //and vector isn't empty
				cout << "Sent to server -> " << failoutput.front() << endl;
				outputfile << failoutput.front() << endl; //first vector member pushed to output file.txt
				failoutput.erase(failoutput.begin(), failoutput.begin() + 1); //remove first vector member from vector begin
			}

			outputfile << line << "\n"; //put string in output file.txt                                       

			size_t found = line.find("ZZ.ZZ.ZZ"); //found pattern
			if (found != std::string::npos) {

				reverseline = true;
			}
			sleep_until(system_clock::now() + milliseconds(1000)); //sleep until
		}

		else {  //when output file.txt isn't open
			size_t found = line.find("ZZ.ZZ.ZZ"); //found pattern
			if (found != std::string::npos) {

				reverseline = true;
			}
			if (failoutput.empty()) //and it's empty
				failoutput.push_back(backupline); //last line wrote in to vector

			//cout << "No Connection!" << "\n";
			failoutput.push_back(line); //also new line wrote in vector
			sleep_until(system_clock::now() + milliseconds(1000)); //sleep until
		}
		if (inputfile.eof() == true) { //when the end of the file is reached
			outputfile.close(); 
		}
		backupline = line; 
	}
	inputfile.close();
}
int main() {
	do_things();
	return 0;
}