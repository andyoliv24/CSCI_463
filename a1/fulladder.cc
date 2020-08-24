/**
 *
 * CSCI 463 Assignment 2
 *
 * Author: Andy Olivares
 * z-ID: z1860934
 * Date: 2/11/2020
 *
 * This program is a binary calculator, adds, subtracts,
 * checks signed and unsigned overflow, checks if the number is even
 * and negative
 */  

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <iomanip>

using namespace std;




/***************************************************************
Function: check_signedOverflow
Type: bool
Use: checks for signed overflow
Parameters: 1. string &check: number to be checked for signOverflow
* 			2. int carry: carry in number
* 			3. int crrOut: carry out number
Returns: true/false
***************************************************************/


bool check_signedOverflow(string num1, string num2, string result, int carryO){
	if((num1[0] == '0' && num2[0] == '0' && result[0] == '1') || (num1[0] == '1' && num2[0] == '1' && result[0] == '0' && carryO == 0))
		return true;
return false;	
}


/***************************************************************
Function: check_unsignedOverflow
Type: bool
Use: checks for unsigned overflow
Parameters: 1. int carry: carry In number
* 			2. int carry: carry Out number
Returns: true/false
***************************************************************/
bool check_unsignedOverflow (int carry, int crrOut){
	if(carry != crrOut){
		return true;
	}
	return false;
}


/***************************************************************
Function: isNegative
Type: bool
Use: checks if number is negative
Parameters: 1. const string &number: number to be checked
Returns: true/false
***************************************************************/
bool isNegative(const string number){
	//checks if first digit is a 1
	if(number[0] == '1')
		return true;
	return false;
}



/***************************************************************
Function: isZero
Type: bool
Use: const string &numberr
Parameters: none
Returns: nothing
***************************************************************/
bool isZero(const string number){
	if(atoi(number.c_str()) == 0)
    	return true;
	return false;
}


/***************************************************************
Function: isOdd
Type: string
Use: checks if number is odd or even
Parameters: 1. const string number: number to check if it's odd or even
Returns: odd/even (string)
***************************************************************/
string isOdd(const string number){
  int sum = 0;

  for(unsigned i = 0; i < number.length();i++){
    if(number[i]== '1')
      sum++;
  }
  if (sum % 2 == 0)
    return "even";
  else
    return "odd ";
}

/***************************************************************
Function: add
Type: tuple<string, bool, bool>
Use: adds binary numbers.
Parameters: 1. string num1: first number to be added 
* 			2. string num2: second number to be added
* 			3. int adder: will containing a carry out number
* 						  depending on the operation (add/subtract)
Returns: tuple<string, bool, bool>
***************************************************************/
tuple <string, bool, bool> add(string num1, string num2, int adder){
	
	tuple <string, bool, bool> values; //initializes tupple to store values
	string result ="";				   //variable to store number added
	int bit_1 = 0;
	int bit_2 = 0;
	int carryIn = 0;
	bool signedOverflow;
	bool unsignedOverflow;
	
	//checks if adder 1
	if(adder == 1){
		carryIn = adder;
	}
	
	//loops through number 1 and 2
	for(int i = num1.length()-1; i>=0; i--){

		if(num1[i]=='1'){
			bit_1 = 1;
		}
		else{
			bit_1 = 0;
		}
		
		
		if(num2[i]=='1'){
			bit_2 = 1;
		}
		else{
			bit_2 = 0;
		}
		
		
		//checks if carry out is 0
		if(bit_1 + bit_2 + carryIn == 0){
			result.push_back('0');
			carryIn = 0;
		}
		
		//checks if carry out is 1
		else if(bit_1 + bit_2 + carryIn == 1){
			result.push_back('1');
			carryIn = 0;
		}
		
		//checks if carry out is 2
		else if(bit_1 + bit_2 + carryIn == 2){
			result.push_back('0');
			carryIn = 1;
		}
		
		//checks if carry out is 3
		else if(bit_1 + bit_2 +carryIn == 3){
			result.push_back('1');
			carryIn = 1;
		}
	}
	
	
	
	
	//calls reverse function
	reverse(result.begin(), result.end()); 
	//calls unsigned overflow function
	unsignedOverflow = check_unsignedOverflow(adder, carryIn);
	//calls signed overflow function
	signedOverflow = check_signedOverflow(num1, num2, result, adder);

	//stores value of result, unsignedOverflow and signedOverflow
	values = make_tuple(result, unsignedOverflow, signedOverflow);
	
	//returns tupple (tuple<string, bool, bool>)
	return values;
}


/***************************************************************
Function: subtracting
Type: tuple<string, bool, bool>
Use: changes binary values, ex: 1 to 0, 0 to 1
Parameters: 1. first number to be swap
* 			2. second number to be swap
Returns: tuple<string, bool, bool>
***************************************************************/
tuple <string, int, int>  subtracting(string number1, string number2){
	for(unsigned i = 0; i < number2.length(); i++){
		//checks if digit equal to 1
		if(number2[i] == '1')
		  number2[i] = '0';
		else
		  number2[i] = '1';
  }
  return add(number1, number2, 1);
}




int main(){
	string v1;						//first number
	string v2;						//second number
	string flags = "----";			//flag holder
	string total_sum;				//variable to store sum
	string total_sub;				//variable to store subtraction
	bool is_negative_sub;			//holds return value from negative funciton (for sub)
	bool is_negative_sum;			//holds return value from negative function (for sum)
	
	while(cin >> v1 && cin >> v2){
		
		//stores addition result
		auto total_sum = add(v1,v2, 0);
		is_negative_sum = isNegative(get<0>(total_sum));
	
		//checks if negative function is true
		if(is_negative_sum == 1){
			flags.replace(3,1, "N");
		}
		
		//checks if signed overflow function is true
		if(get<2>(total_sum)){
			flags.replace(0,1, "S");
		}
		
		//checks if zero function is true
		if(isZero(get<0>(total_sum))){
			flags.replace(2,1, "Z");
		}
				
		//checks if unsigned overflow function is true	
		if(get<1>(total_sum)){
			flags.replace(1,1, "U");
		}
	

		cout << "v1" << "    " << isOdd(v1) << "        "  << v1 << endl;
		cout << "v2" << "    " << isOdd(v2) << "        "  << v2 << endl;
		cout << "sum" <<"   " << isOdd(get<0>(total_sum)) << "  " << flags<< "  " << get<0>(total_sum) << endl;
		
		//stores subtraction result
		auto total_sub = subtracting(v1,v2);
		is_negative_sub = isNegative(get<0>(total_sub));
		
		flags = "----";
		
		//checks if number is negative function is true
		if(is_negative_sub == 1){
			flags.replace(3,1, "N");
		}
		
		//checks if signed overflow function is true
		if(get<2>(total_sub)){
			flags.replace(0,1, "S");
		}
		
		//checks if zero function is true
		if(isZero(get<0>(total_sub))){
			flags.replace(2,1, "Z");
		}
		
		//checks if unsigned overflow function is true
		if(get<1>(total_sub)){
			flags.replace(1,1, "U");
		}
		

		cout << "diff" << "  "<< isOdd(get<0>(total_sub)) << "  "<<  flags << "  " << get<0>(total_sub) << endl;
		
		//resets flags holder
		flags = "----";
		cout << endl;
		cout << endl;
	}
	
	
	return 0;
}


