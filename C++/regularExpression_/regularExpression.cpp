#include <iostream>
#include <regex>
#include <string>
using namespace std;

void printMatches(string str, regex reg){
	// search matching for string
	smatch matches;
	cout << boolalpha;
	while(regex_search(str, matches, reg)){
		// cout << "Is there a match: " << matches.ready() << endl;
		// cout << "Are there no matches: " << matches.empty() << endl;
		// cout << "Number of matches: " << matches.size() << endl;
		cout << matches.str(1) << endl;
		str = matches.suffix().str();
		// cout << "Left: " << str << endl;
	}
	cout << endl;
}

void printMatches2(string str, regex reg){
	sregex_iterator currentMatch(str.begin(), str.end(), reg);
	sregex_iterator lastMatch;
	while(currentMatch != lastMatch){
		smatch match = *currentMatch;
		cout << match.str() << endl;
		currentMatch++;
	}
	cout << endl;
}

int main(){
	string str = "The ape was at the apex";
	// means want to get string that:
	// start with "ape" and
	// []: followed by
	// ^: not, ' ': space
	// ?: but [^ ] is not required to be a match
	regex reg("(ape[^ ]?)");
	printMatches(str, reg);

	string str2 = "I picked the pickle";
	// means want to get string that:
	// start with "pick" and
	// []: followed by
	// ^: not, ' ': space
	// +: for one or more letters
	// ?: but is not required to be a match
	regex reg2("(pick([^ ]+)?)");
	printMatches2(str2, reg2);

	string str3 = "Cat rat mat fat pat";
	// means want to get string that:
	// start with any char from: 'c', 'r', 'm', 'f', 'p'
	// followed by: "at"
	regex reg3("([crmfp]at)");
	printMatches2(str3, reg3);

	// means want to get string that:
	// start with any char from: 'C' to 'F' or 'c' to 'f'
	// followed by: "at"
	regex reg4("([C-Fc-f]at)");
	printMatches2(str3, reg4);

	regex reg5("([^Cr]at)");
	// means want to get string that:
	// not start with any char from: 'C' or 'r'
	// followed by: "at"
	printMatches2(str3, reg5);

	regex reg6("([Cr]at)");
	// means want to get string that:
	// start with any char from: 'C' or 'r'
	// followed by: "at"
	string owlFood = regex_replace(str3, reg6, "Owl");
	cout << owlFood << endl;

	string str4 = "F.B.I. I.R.S. CIA";
	// means want to get string that:
	// not start with space
	// followed by a dot: "\." and
	// followed by any char: "." and
	// followed by a dot: "\." and
	// followed by any char: "." and
	// followed by a dot: "\."
	regex reg7("([^ ]\..\..\.)");
	printMatches2(str4, reg7);

	// \b: backspace
	// \f: form feed
	// \r: carriage return
	// \t: tab
	// \v: vertical tab

	string str5 = "This is a\nmultiline string\n"
					"that has many lines";
	regex reg8("\n");
	string noLBstr = regex_replace(str5, reg8, " ");
	cout << noLBstr << endl;

	// equals to
	// \d: [0-9]
	// \D: [^0-9]
	string str6 = "12345";
	regex reg9("\\d");
	printMatches2(str6, reg9);

	string str7 = "123 12345 123456 1234567";
	// means want to get string that:
	// start with digits: "\\d"
	// that its length is 5 to 7: "{5,7}"
	regex reg10("\\d{5,7}");
	printMatches2(str7, reg10);

	// equals to
	// \w: [a-zA-Z0-9]
	// \W: [^a-zA-Z0-9]
	string str8 = "40-666-888";
	regex reg11("\\w{2}-\\w{3}-\\w{3}");
	printMatches2(str8, reg11);

	// equals to
	// \s: [\f\n\r\t\v]
	// \S: [^\f\n\r\t\v]
	string str9 = "Classroom of the Elite";
	regex reg12("\\w{2,20}\\s\\w{2,20}");
	printMatches2(str9, reg12);

	string str10 = "a as ape bug";
	regex reg13("a[a-z]+");
	printMatches2(str10, reg13);

	string str11 = "db@aol.com m@.com @apple.com dp@.com";
	// ----- PROBLEM -----
    // Create a Regex that matches email addresses 
    // from a list
    // 1. 1 to 20 lowercase and uppercase letters, numbers, plus ._%+-
    // 2. An @ symbol
    // 3. 2 to 20 lowercase and uppercase letters, numbers, plus .-
    // 4. A period
    // 5. 2 to 3 lowercase and uppercase letters
	regex reg14("[\\w._%+-]{1,20}@[\\w.-]{2,20}\.[a-zA-Z]{2,3}");
	printMatches2(str11, reg14);
}