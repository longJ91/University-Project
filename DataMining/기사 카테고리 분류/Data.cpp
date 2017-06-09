
#include "stdafx.h"

using namespace std;

// Temporary storage for sorting
map<string, int> temp_count;
vector<pair<double,int>> temp_X2;


string::size_type lastPos;
string::size_type pos;

/*
Function the token
reference : https://wiki.kldp.org/HOWTO/html/C++Programming-HOWTO/standard-string.html
*/
void Tokenize(const string& str, map<string, double>& tokens, map<string, int> number_doc[8], int category_index, const string& delimiters = " ") {
	// Ignore if the first character is a delimiter
	lastPos = str.find_first_not_of(delimiters, 0);
	// Find the first non-delimiter
	pos = str.find_first_of(delimiters, lastPos);

	string word;

	// Cut one line by ""
	// Save the classified features in categories & one map without duplicates
	while (string::npos != pos || string::npos != lastPos) {
		word = str.substr(lastPos, pos - lastPos);

		if (tokens.count(word) == 0) tokens.insert(pair<string, double>(word, 0.0));
		
		number_doc[category_index].insert(pair<string, int>(word, 1));

		// Skip the delimiter. Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find the next non-delimiter character
		pos = str.find_first_of(delimiters, lastPos);
	}
}

//Function the token
void TokenizeC(const string& str, map<string, double>& tokens, map<string, int> number_doc[8], int category_index, const string& delimiters = " ") {\
	// Ignore if the first character is a delimiter
	lastPos = str.find_first_not_of(delimiters, 0);
	// Find the first non-delimiter
	pos = str.find_first_of(delimiters, lastPos);

	string word;

	// Cut one line by ""
	// Feature stores the total number of documents in each category
	while (string::npos != pos || string::npos != lastPos) {
		word = str.substr(lastPos, pos - lastPos);
		// Save the number of documents
		if (number_doc[category_index].count(word)>0) {
			if (temp_count.count(word) == 0) temp_count.insert(pair<string, int>(word, 1));
		}

		// Skip the delimiter. Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find the next non-delimiter character
		pos = str.find_first_of(delimiters, lastPos);
	}
}

//Function the token
void TokenizeR(const string& str, map<string, double>& feature, map<string, int>& multi_feature, const string& delimiters = " ") {
	// Ignore if the first character is a delimiter
	lastPos = str.find_first_not_of(delimiters, 0);
	// Find the first non-delimiter
	pos = str.find_first_of(delimiters, lastPos);

	int index;
	string word;
	double X2;

	map<string, double>::iterator it;

	// Search for saved Chi values and indexes and save them temporarily
	while (string::npos != pos || string::npos != lastPos) {
		word = str.substr(lastPos, pos - lastPos);
		if (feature.count(word) > 0) {
			X2 = feature.at(word);
			index = multi_feature.at(word);
			temp_X2.push_back(pair<double, int>(X2, index));
		}

		// Skip the delimiter. Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find the next non-delimiter character
		pos = str.find_first_of(delimiters, lastPos);
	}
}

// This function returns true if the first pair is "less"
// than the second one according to some metric
// In this case, we say the first pair is "less" if the first element of the first pair
// is less than the first element of the second pair
bool pairCompare(const pair<double, string>& firstElem, const pair<double, string>& secondElem) {
	return firstElem.first > secondElem.first;
}
bool pairCompareR(const pair<double, int>& firstElem, const pair<double, int>& secondElem) {
	return firstElem.first > secondElem.first;
}
/*
1. 1-4 Read training files one by one.
2. Open a file and read it one line at a time.
3. In the article you read, "?! '," Quot ;, and & quot; space & quot;
4. Divide the revised text by a space and read it as a single word.
5. Remove redundancy by storing the word in the map data structure.
6. Proceed through files 1 to 4 in order.
7. 1 ~ 4 files Read the file once again, store the category number based on the words in the category, and store the number of documents per category.
Also, store how many words in a category are displayed based on words that have been previously saved. (However, even if the same word occurs multiple times in one category, it is assumed to be number 1.)
8. Use the numbers above to find the ky square value (select MAX value).
9. Sort the words in descending order based on each word.
10. Finally, it reads 5 test files, extracts the words from the training file, and sorts them based on the chi-squared values ​​of the words.
*/
int main() {
	map <string, double> feature;
	map<string, int> multi_feature;
	map <string, int> number_doc[8];
	vector<string>::iterator new_end;
	vector<pair<double, string>> temp_feature;

	int total_each_category[8] = { 0, };
	int temp_each_category[8] = { 0, };
	double temp_X2_index[8] = { 0.0, };

	int N = 0;
	//Default file name
	string file_name = "HKIB-20000_000.txt";

	string doc = "@DOCUMENT";
	string text = "#TEXT";
	string category = "#CAT'03";
	string KW = "<KW>";

	int text_flag = 0;

	int category_index = 0;
	string temp_category;

	double A, B, C, D;

	//Save the features
	for (int i = 1; i < 5; i++) {
		char buffer[10];
		string temp_line;

		_itoa(i, buffer, 10);
		file_name[13] = buffer[0];

		//Read files
		ifstream file(file_name.data());

		//Check the file
		if (!file.is_open()) printf("Not file open!!!\n");

		//Read string of files
		//Emty check
		while (getline(file, temp_line)) {

			char *temp = new char[temp_line.size() + 1];
			strcpy(temp, temp_line.c_str());

			//Get the total number of each categorys
			if (strstr(temp, "#CAT'03")) {
				if (strstr(temp, "건강과 의학")) category_index = 0;
				else if (strstr(temp, "경제")) category_index = 1;
				else if (strstr(temp, "과학")) category_index = 2;
				else if (strstr(temp, "교육")) category_index = 3;
				else if (strstr(temp, "문화와 종교")) category_index = 4;
				else if (strstr(temp, "사회")) category_index = 5;
				else if (strstr(temp, "산업")) category_index = 6;
				else if (strstr(temp, "여가생활")) category_index = 7;

				total_each_category[category_index]++;
			}
			//Find #TEXT
			if (strstr(temp, "#TEXT")) {
				//Initial flag
				text_flag = 1;
				continue;
			}
			if (strstr(temp, "@DOCUMENT")) {
				text_flag = 0;
				continue;
			}
			if (strstr(temp, "<KW>")) {
				text_flag = 0;
				continue;
			}
			if (text_flag == 1) {
				//Case the empty
				if (strlen(temp) == 0) continue;
				//Change the specific word
				for (int i = 0; i < strlen(temp); i++) {
					if (temp[i] == '.' || temp[i] == '-' || temp[i] == ',' || temp[i] == '/'
						|| temp[i] == '<' || temp[i] == '>' || temp[i] == '{' || temp[i] == '}'
						|| temp[i] == '[' || temp[i] == ']' || temp[i] == '"' || temp[i] == '*'
						|| temp[i] == '(' || temp[i] == ')' || temp[i] == ':' || temp[i] == '!' || temp[i] == '?'
						|| temp[i] == '@') {
						temp[i] = ' ';
					}
				}
				//Split the line and get the number of each category of feature
				Tokenize(temp, feature, number_doc, category_index);
			}
		}
		file.close();
	}

	//Count the document of ecah feature
	for (int i = 1; i < 5; i++) {
		char buffer[10];
		string temp_line;

		_itoa(i, buffer, 10);
		file_name[13] = buffer[0];

		//Read files
		ifstream file_count(file_name.data());

		//Check the file
		if (!file_count.is_open()) printf("Not file open!!!\n");

		//Read string of files
		//Emty check
		while (getline(file_count, temp_line)) {

			char *temp = new char[temp_line.size() + 1];
			strcpy(temp, temp_line.c_str());

			//Get the total number of each categorys
			if (strstr(temp, "#CAT'03")) {
				if (strstr(temp, "건강과 의학")) category_index = 0;
				else if (strstr(temp, "경제")) category_index = 1;
				else if (strstr(temp, "과학")) category_index = 2;
				else if (strstr(temp, "교육")) category_index = 3;
				else if (strstr(temp, "문화와 종교")) category_index = 4;
				else if (strstr(temp, "사회")) category_index = 5;
				else if (strstr(temp, "산업")) category_index = 6;
				else if (strstr(temp, "여가생활")) category_index = 7;
			}
			//Find #TEXT
			if (strstr(temp, "#TEXT")) {
				//Initial flag
				text_flag = 1;
				continue;
			}
			if (strstr(temp, "@DOCUMENT")) {
				text_flag = 0;

				for (map<string, int>::iterator s = temp_count.begin(); s != temp_count.end(); s++) {
					if (number_doc[category_index].count(s->first) > 0) number_doc[category_index].at(s->first)++;
				}

				//Clear the temp_count
				temp_count.erase(temp_count.begin(), temp_count.end());
				continue;
			}
			if (strstr(temp, "<KW>")) {
				text_flag = 0;
				continue;
			}
			if (text_flag == 1) {
				//Case the empty
				if (strlen(temp) == 0) continue;
				//Change the specific word
				for (int i = 0; i < strlen(temp); i++) {
					if (temp[i] == '.' || temp[i] == '-' || temp[i] == ',' || temp[i] == '/'
						|| temp[i] == '<' || temp[i] == '>' || temp[i] == '{' || temp[i] == '}'
						|| temp[i] == '[' || temp[i] == ']' || temp[i] == '"' || temp[i] == '*'
						|| temp[i] == '(' || temp[i] == ')' || temp[i] == ':' || temp[i] == '!' || temp[i] == '?'
						|| temp[i] == '@') {
						temp[i] = ' ';
					}
				}
				//Split the line and get the number of each category of feature
				TokenizeC(temp, feature, number_doc, category_index);
			}
		}
		file_count.close();
	}

	//Get the totla number of documents
	for (int d = 0; d < 7; d++) N += total_each_category[d];

	int count = 0;

	//Write file
	ofstream ABCD_FILE("ABCD.txt");

	//Caculate the X2 statistic
	for (map<string, double>::iterator s = feature.begin(); s != feature.end(); s++) {
		double result = 0.0;
		vector<double> temp_result;

		for (int k = 0; k < 8; k++) {

			result = A = B = C = D = 0.0;

			if (number_doc[k].count(s->first) > 0) A = number_doc[k].at(s->first);

			for (int c = 0; c < 8; c++) {
				if (k == c) continue;
				if (number_doc[c].count(s->first) > 0) C += number_doc[c].at(s->first);
				D += total_each_category[c];
			}

			B = total_each_category[k] - A;
			D = D - C;

			if ((A + B)*(A + C)*(B + D)*(D + C) == 0) 
				result = 0.0;
			else
				result = ((double)N*(pow(((A*D) - (B*C)), 2.0))) / ((A + B)*(A + C)*(B + D)*(D + C));

			temp_result.push_back(result);

			//Write file ABCD
			ABCD_FILE << "CATE "<< k << " - " <<"A : " << A << " B : " << B << " C : " << C << " D : " << D <<  " feature : "<< s->first<<"\n";
		}
		//Sort the chi value
		sort(temp_result.begin(), temp_result.end());
		//Select the max chi value
		s->second = temp_result.back();
		//Select the max chi value
		temp_feature.push_back(pair<double, string>(s->second, s->first));
		//Clear the temp
		temp_result.erase(temp_result.begin(), temp_result.end());

		//Check count
		count++;
	}

	ABCD_FILE.close();
	//Sort the feature by chi value
	sort(temp_feature.begin(), temp_feature.end(), pairCompare);

	count = 0;

	//Write file
	ofstream FEATURE_INDEX("FEATURE_INDEX.txt");
	//Store the feature value
	for (vector<pair<double,string>>::iterator it = temp_feature.begin() ; it != temp_feature.end(); it++){
		multi_feature.insert(pair<string, int>(it->second, count++));
		FEATURE_INDEX << "Feature : " << it->second << " INDEX : " << count << " CHI : " << it->first << "\n";
	}
	FEATURE_INDEX.close();

	//Write file
	ofstream o_FILE("training.txt");

	int flag = 1;
	category_index = 0;

	//Write the training file
	for (int i = 1; i < 5; i++) {
		char buffer[10];
		string temp_line;

		_itoa(i, buffer, 10);
		file_name[13] = buffer[0];

		//Read files
		ifstream FILE(file_name.data());

		//Check the file
		if (!FILE.is_open()) printf("Not file open!!!\n");

		//Read string of files
		//Emty check
		while (getline(FILE, temp_line)) {

			char *temp = new char[temp_line.size() + 1];
			strcpy(temp, temp_line.c_str());

			//Get the total number of each categorys
			if (strstr(temp, "#CAT'03")) {
				if (strstr(temp, "건강과 의학")) category_index = 0;
				else if (strstr(temp, "경제")) category_index = 1;
				else if (strstr(temp, "과학")) category_index = 2;
				else if (strstr(temp, "교육")) category_index = 3;
				else if (strstr(temp, "문화와 종교")) category_index = 4;
				else if (strstr(temp, "사회")) category_index = 5;
				else if (strstr(temp, "산업")) category_index = 6;
				else if (strstr(temp, "여가생활")) category_index = 7;
			}
			//Find #TEXT
			if (strstr(temp, "#TEXT")) {
				//Initial flag
				text_flag = 1;
				continue;
			}
			if (strstr(temp, "@DOCUMENT")) {
				text_flag = 0;
				if (flag == 1) {
					flag = 0;
					continue;
				}
				sort(temp_X2.begin(), temp_X2.end(), pairCompareR);

				map<int, double>M;

				//Stroe the sorted feature index and feature chi value by chi value 
				for (vector<pair<double, int>>::iterator it = temp_X2.begin(); it != temp_X2.end(); it++) {
					M.insert(pair<int, double>(it->second, it->first));
				}
				//Write the category, index and chi value by format
				o_FILE << category_index + 1 << " ";
				for (map<int, double>::iterator it = M.begin(); it != M.end(); it++) {
					o_FILE << it->first + 1 << ":" << it->second << " ";
				}
				o_FILE << "\n";

				temp_X2.erase(temp_X2.begin(), temp_X2.end());

				continue;
			}
			if (strstr(temp, "<KW>")) {
				text_flag = 0;
				continue;
			}
			if (text_flag == 1) {
				//Case the empty
				if (strlen(temp) == 0) continue;
				//Change the specific word
				for (int i = 0; i < strlen(temp); i++) {
					if (temp[i] == '.' || temp[i] == '-' || temp[i] == ',' || temp[i] == '/'
						|| temp[i] == '<' || temp[i] == '>' || temp[i] == '{' || temp[i] == '}'
						|| temp[i] == '[' || temp[i] == ']' || temp[i] == '"' || temp[i] == '*'
						|| temp[i] == '(' || temp[i] == ')' || temp[i] == ':' || temp[i] == '!' || temp[i] == '?'
						|| temp[i] == '@') {
						temp[i] = ' ';
					}
				}
				//Write the X2 to training file
				TokenizeR(temp, feature, multi_feature);
			}
		}
		FILE.close();
	}
	o_FILE.close();

	//Write test file
	ofstream o_FILE_test("test.txt");

	int flag = 1;

	//Write the test file
	for (int i = 5; i < 6; i++) {
		char buffer[10];
		string temp_line;

		_itoa(i, buffer, 10);
		file_name[13] = buffer[0];

		//Read files
		ifstream FILE_test(file_name.data());

		//Check the file
		if (!FILE_test.is_open()) printf("Not file open!!!\n");

		//Read string of files
		//Emty check
		while (getline(FILE_test, temp_line)) {

			char *temp = new char[temp_line.size() + 1];
			strcpy(temp, temp_line.c_str());

			//Get the total number of each categorys
			if (strstr(temp, "#CAT'03")) {
				if (strstr(temp, "건강과 의학")) category_index = 0;
				else if (strstr(temp, "경제")) category_index = 1;
				else if (strstr(temp, "과학")) category_index = 2;
				else if (strstr(temp, "교육")) category_index = 3;
				else if (strstr(temp, "문화와 종교")) category_index = 4;
				else if (strstr(temp, "사회")) category_index = 5;
				else if (strstr(temp, "산업")) category_index = 6;
				else if (strstr(temp, "여가생활")) category_index = 7;
			}
			//Find #TEXT
			if (strstr(temp, "#TEXT")) {
				//Initial flag
				text_flag = 1;
				continue;
			}
			if (strstr(temp, "@DOCUMENT")) {
				text_flag = 0;
				if (flag == 1) {
					flag = 0;
					continue;
				}
				sort(temp_X2.begin(), temp_X2.end(), pairCompareR);

				map<int, double>M;
				//Stroe the sorted feature index and feature chi value by chi value 
				for (vector<pair<double, int>>::iterator it = temp_X2.begin(); it != temp_X2.end(); it++) {
					M.insert(pair<int, double>(it->second, it->first));
				}
				//Write the category, index and chi value by format
				o_FILE_test << category_index + 1 << " ";
				for (map<int, double>::iterator it = M.begin(); it != M.end(); it++) {
					o_FILE_test << it->first + 1 << ":" << it->second << " ";
				}
				o_FILE_test << "\n";

				temp_X2.erase(temp_X2.begin(), temp_X2.end());

				continue;
			}
			if (strstr(temp, "<KW>")) {
				text_flag = 0;
				continue;
			}
			if (text_flag == 1) {
				//Case the empty
				if (strlen(temp) == 0) continue;
				//Change the specific word
				for (int i = 0; i < strlen(temp); i++) {
					if (temp[i] == '.' || temp[i] == '-' || temp[i] == ',' || temp[i] == '/'
						|| temp[i] == '<' || temp[i] == '>' || temp[i] == '{' || temp[i] == '}'
						|| temp[i] == '[' || temp[i] == ']' || temp[i] == '"' || temp[i] == '*'
						|| temp[i] == '(' || temp[i] == ')' || temp[i] == ':' || temp[i] == '!' || temp[i] == '?'
						|| temp[i] == '@') {
						temp[i] = ' ';
					}
				}
				//Write the X2 to training file
				TokenizeR(temp, feature, multi_feature);
			}
		}
		FILE_test.close();
	}
	o_FILE_test.close();

	return 0;
}