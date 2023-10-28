/*
Pranav Nambiar
CS-530
*/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>
#include <bitset>
#include <sstream>

using namespace std;

const static std::map<std::string, std::string> opsToMnemonics = {
    {"18", "ADD"}, {"58", "ADDF"}, {"90", "ADDR"}, {"40", "AND"}, {"B4", "CLEAR"}, {"28", "COMP"},
    {"88", "COMPF"}, {"A0", "COMPR"}, {"24", "DIV"}, {"64", "DIVF"}, {"9C", "DIVR"}, {"C4", "FIX"},
    {"C0", "FLOAT"}, {"F4", "HIO"}, {"3C", "J"}, {"30", "JEQ"}, {"34", "JGT"}, {"38", "JLT"},
    {"48", "JSUB"}, {"00", "LDA"}, {"68", "LDB"}, {"50", "LDCH"}, {"70", "LDF"}, {"08", "LDL"},
    {"6C", "LDS"}, {"74", "LDT"}, {"04", "LDX"}, {"D0", "LPS"}, {"20", "MUL"}, {"60", "MULF"},
    {"98", "MULR"}, {"C8", "NORM"}, {"44", "OR"}, {"D8", "RD"}, {"AC", "RMO"}, {"4C", "RSUB"},
    {"A4", "SHIFTL"}, {"A8", "SHIFTR"}, {"F0", "SIO"}, {"EC", "SSK"}, {"0C", "STA"}, {"78", "STB"},
    {"54", "STCH"}, {"80", "STF"}, {"D4", "STI"}, {"14", "STL"}, {"7C", "STS"}, {"E8", "STSW"},
    {"84", "STT"}, {"10", "STX"}, {"1C", "SUB"}, {"5C", "SUBF"}, {"94", "SUBR"}, {"B0", "SVC"},
    {"E0", "TD"}, {"F8", "TIO"}, {"2C", "TIX"}, {"B8", "TIXR"}, {"DC", "WD"}
};

const static std::map<std::string, bool> opsToFormat2 = {
    {"18", false}, {"58", false}, {"90", true}, {"40", false}, {"B4", true}, {"28", false},
    {"88", false}, {"A0", true}, {"24", false}, {"64", false}, {"9C", true}, {"C4", false},
    {"C0", false}, {"F4", false}, {"3C", false}, {"30", false}, {"34", false}, {"38", false},
    {"48", false}, {"00", false}, {"68", false}, {"50", false}, {"70", false}, {"08", false},
    {"6C", false}, {"74", false}, {"04", false}, {"D0", false}, {"20", false}, {"60", false},
    {"98", false}, {"C8", false}, {"44", false}, {"D8", false}, {"AC", true}, {"4C", false},
    {"A4", true}, {"A8", true}, {"F0", false}, {"EC", false}, {"0C", false}, {"78", false},
    {"54", false}, {"80", false}, {"D4", false}, {"14", false}, {"7C", false}, {"E8", true},
    {"84", false}, {"10", false}, {"1C", false}, {"5C", false}, {"94", false}, {"B0", false},
    {"E0", false}, {"F8", false}, {"2C", false}, {"B8", false}, {"DC", true}
};

std::string getFormat(const std::string& hexString) {
    if (hexString.size() != 1) {
        return "Invalid input";
    }

    //convert the hexadecimal character to an integer
    int hexDigit = std::stoi(hexString, nullptr, 16);
    

    //convert the integer to a binary string
    std::string binaryString = std::bitset<4>(hexDigit).to_string();

    
    char lastBit = binaryString[3];//find format
    if (lastBit == '0') {
        return "3";
    } else {
        return "4";
    }
}

std::string getTAAM(const std::string& hexString) {
    //convert the hexadecimal string to an integer
    int hexNumber = std::stoi(hexString, nullptr, 16);

    //convert the integer to a binary string
    std::string binaryString = std::bitset<4>(hexNumber).to_string();

    //determine the addressing mode based on the bits
    char xBit = binaryString[0];
    char bBit = binaryString[1];
    char pBit = binaryString[2];
    char eBit = binaryString[3]; 

    if (xBit == '1' && pBit == '1') {
        return "pc_indexed";
    } else if (xBit == '1' && bBit == '1') {
        return "base_indexed";
    } else if (xBit == '0' && bBit == '0' && pBit == '0') {
        return "absolute";
    }
    else if (xBit == '0' && bBit == '1' && pBit == '0') {
        return "base";
    }
    else if (xBit == '0' && bBit == '0' && pBit == '1') {
        return "pc";
    }  else {
        return "Invalid input"; // Invalid combination
    }
}


std::string getOAT(const std::string& hexString) {

    //convert the hexadecimal string to an integer
    int hexNumber;
    try {
        hexNumber = std::stoi(hexString, nullptr, 16);
    } catch (...) {
        return "Invalid input"; 
    }

    //extract the last 2 bits
    int lastTwoBits = hexNumber & 0x03;

    std::string result;
    if (lastTwoBits == 0) {
        result = "Simple";
    } else if (lastTwoBits == 1) {
        result = "Immediate";
    } else if (lastTwoBits == 2) {
        result = "Indirect";
    } else {
        result = "Simple";
    }

    return result;
}


std::string getOpCode(const std::string& hexString) {
    //convert the hexadecimal string to an integer
    std::stringstream ss;
    ss << std::hex << hexString;
    unsigned int hexNumber;
    ss >> hexNumber;
  
    if (hexNumber > 0xFF) {
        return "Invalid input";
    }

   
    hexNumber = hexNumber & 0xFC; //set last 2 bits to 0

    std::stringstream result;
    result << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << hexNumber;
    return result.str();
}

 /*void processHeader(string line){

 }*/

 void processTextRecord(string line, ofstream& output){
    std::string lengthHex = std::string(1, line[7]) + std::string(1, line[8]);
    int textLength = std::stoi(lengthHex, nullptr, 16);
    textLength = textLength *2;
     int digit = 1;
    line = line.substr(9);  //0500000320146910178D1BA0101BC0002F20073B2FF40F102EFD9041

     for(int i = 0;i<=textLength;i++){
        if(digit%2 == 0){
           digit = 0;
           string opCode; 
           string INSTR;
           string OAT;
           string TAAM;
           string Format;
           string objectCode;
           auto it = opsToFormat2.find(line.substr(i-1,2));
           if (it != opsToFormat2.end() && it->second){ 
            opCode = line.substr(i-1,2);
            INSTR = opsToMnemonics.at(opCode);
            OAT = "";
            TAAM ="";
            Format = "2";
            objectCode = line.substr(i-1,4);
            i+=2;


           }
           else{
           opCode = getOpCode(line.substr(i-1,2));
            INSTR = opsToMnemonics.at(opCode);
            OAT = getOAT(line.substr(i,1));
            TAAM = getTAAM(line.substr(i+1,1));
            Format =  getFormat(line.substr(i+1,1));
            
           
           if(Format == "3"){
            objectCode = line.substr(i-1,6);
            i+=4;
           }
           else{
            objectCode = line.substr(i-1,8);
            i+=6;
           }
           }
        
            output << left << setw(16) << INSTR << setw(16) << Format << setw(16) << OAT << setw(16) << TAAM << setw(16) << objectCode << endl;    
        }
        digit++;
     }
    

 }

 void writeHeader(ofstream& output) {
    output << left << setw(16) << "INSTR" << setw(16) << "FORMAT" << setw(16) << "OAT" << setw(16) << "TAAM" << setw(16) << "OBJ" << endl;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_filename>" << endl;
        return 1;
    }

    ifstream input(argv[1]);
    if (!input) {
        cerr << "Error: Could not open input file." << endl;
        return 1;
    }

    ofstream output("obj_struct.txt");
    if (!output) {
        cerr << "Error: Could not create output file." << endl;
        return 1;
    }

     if (input.is_open() && output.is_open()) {
        writeHeader(output);
        
        string line;
        while (getline(input, line)) {
            if (line[0] == 'T') {
                processTextRecord(line, output);
            }
        }
    }

    input.close();
    output.close();

    return 0;
}
