#include <iostream>
#include<fstream>
using namespace std;

struct subList {
	int year;
	char session;
	char code[7];
	int credit;
	int subjectMark;
	char subStatus;
};

struct numName {
	int courseNum;
	char courseName[100];
};

// contains the information uploaded through the text file. This is linked to other structures which store
// additional information such as the subjects enrolled in and the course number and name
struct student {
	char name[30];
	int id;
	numName single;
	int enrolled;
	subList person[30];
	float GPA;
};

void getstudents(student database[], int&);

int main(){
	
	/*//strcpy(database[0].name, "Tessa Harris");
	int length = 0;
	
	ofstream outbin( "binary.bin", ios::binary );
	outbin.write(database[0].name, sizeof(database));
	ifstream binary;
	binary.open("test.bin", ios::ate | ios::binary);
	length = binary.tellg()/ sizeof(database);
	binary.close();
	binary.open("binary.bin", ios:: binary);
	
	for(int i = 0; i  < length; i++){
		binary.read((char * )& database, sizeof(database));
	}*/
	
	ifstream binary;
	int testLength = 0;
	student test[1000];
	binary.open("m.bin", ios::ate | ios::binary);
	testLength = binary.tellg()/ sizeof(student);
	
	cout << "TEST: " << testLength << endl;
	binary.close();
	binary.open("m.bin", ios::binary);
	for(int j = 0; j < testLength; j++){
	
		binary.read((char*)&test, sizeof(test));
		cout << "Name: " << test[j].name << endl;
		cout << "Course Name: " << test[j].single.courseName << endl;
	}
	
	
	
	cout << test[0].name << endl;
	
	
  	//outbin.write( reinterpret_cast <const char*> (&year), sizeof( year ) );
 	//outbin.close();
	
	
	return 0;
}

void getstudents(struct student database[], int & counter){

	return;
}