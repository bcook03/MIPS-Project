#include <map>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <bitset>
#include <iomanip>
#include <fstream>
using namespace std;

int main(int argc, char* argv[] )
{
    // ./mipssim -i test1.bin -o  x1
    // [0]       [1]    [2]
    char buffer[4];
    int i;
    char * iPtr;
    iPtr = (char*)(void*) &i;
    bool breakVal = true;

    int FD = open(argv[2], O_RDONLY);
    ofstream disout(string (argv[4]) + "_dis.txt");
    ofstream simout(string (argv[4]) + "_sim.txt");

    struct item{
        int i, rs, rt, rd, imm, opcode, valid,  instr_index, funct, hint, offset, sa;
        unsigned int asUint;
        string instStr, binstr;


    };
    int addr = 96;
    int amt = 4;
    item MEM[500];
    while( breakVal )
    {
        
        amt = read(FD, buffer, 4);
        if(amt == 4)
        {
            iPtr[0] = buffer[3];
            iPtr[1] = buffer[2];
            iPtr[2] = buffer[1];
            iPtr[3] = buffer[0];
            item I;
            
            
            unsigned int asUint = (unsigned int) i;
            I.asUint = asUint;

            bitset<32> b( i );
            I.binstr = b.to_string();
            
            I.valid = asUint >> 31;
            I.opcode = asUint >> 26;
            I.funct = (asUint << 26) >> 26;
            
            I.rs = (asUint << 6)>>27;
            I.rt = (asUint << 11)>>27;
            I.imm = (asUint << 16) >> 16;
            I.rd = (asUint << 16) >> 27;
            I.sa = (asUint << 21) >> 27;

            I.instr_index = (asUint << 6) >> 6;
            I.offset = (asUint << 16) >> 16;

            if (I.valid == 0){
                I.instStr = "Invalid Instruction";
                disout << I.binstr << "\t" << I.instStr << endl;
                cout << I.binstr << "\t" << I.instStr << endl;
            }
            else{
                if (I.opcode == 34 ) {
                    I.instStr = "J\t#" + to_string(I.instr_index);
                    disout << I.binstr << "\t" << I.instStr << endl; 
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if (I.opcode == 32 && I.funct == 8) {
                    I.instStr = "JR\tR" + to_string(I.rs);
                    disout << I.binstr << "\t" << I.instStr << endl; 
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if ( I.opcode == 33 && I.rt == 0) {
                    I.instStr = "BLTZ\tR" + to_string(I.rs) + ", #" + to_string(I.offset);
                    disout << I.binstr << "\t" << I.instStr << endl;
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if ( I.opcode ==  36) {
                    I.instStr = "BEQ\tR" + to_string(I.rt) + ", R" + to_string(I.rs) + ", #" + to_string(I.offset);
                    disout << I.binstr << "\t" << I.instStr << endl;                
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if ( I.opcode == 32 && I.funct == 32) {
                    I.instStr = "ADD\tR" + to_string(I.rt) + ", R" + to_string(I.rs) + ", R" + to_string(I.rd);
                    disout << I.binstr << "\t" << I.instStr << endl; 
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if( I.opcode == 40 ){
                    I.instStr = "ADDI\tR" + to_string(I.rt) + ", R" + to_string(I.rs) + ", #" + to_string(I.imm);
                    disout << I.binstr << "\t" << I.instStr << endl; 
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 60 && I.funct == 2){
                    I.instStr = "MUL\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt);
                    disout << I.binstr << "\t" << I.instStr << endl;
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct == 36) {
                    I.instStr = "And\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt);
                    disout << I.binstr << "\t" << I.instStr << endl;
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct ==37) {
                    I.instStr = "Or\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt);
                    disout << I.binstr << "\t" << I.instStr << endl;
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct == 5) {
                    I.instStr = "MOVZ\tR" + to_string(I.rd) + ", R" + to_string(I.rs)+ ", R" + to_string(I.rt);
                    disout << I.binstr << "\t" << I.instStr << endl;
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.sa == 0 && I.funct == 0) {
                    I.instStr = "NOP";
                    disout << I.binstr << "\t" << I.instStr << endl;
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct == 34) {
                    I.instStr = "SUB\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt); 
                    disout << I.binstr << "\t" << I.instStr << endl; 
                    cout << I.binstr << "\t" << I.instStr << endl;
                }

                else if(I.opcode == 43){
                    I.instStr = "SW\tR" + to_string(I.rt) + ", " + to_string(I.offset);
                    disout << I.binstr << "\t" << I.instStr << endl; 
                    cout << I.binstr << "\t" << I.instStr << endl;
                }

                else if(I.opcode == 35){
                    I.instStr = "LW\tR" + to_string(I.rt) + ", " + to_string(I.offset);
                    disout << I.binstr << "\t" << I.instStr << endl; 
                    cout << I.binstr << "\t" << I.instStr << endl;
                }

                else if(I.opcode == 32 && I.sa > 0 && I.funct == 0){
                    I.instStr = "SLL\tR" + to_string(I.rd) + ", R" + to_string(I.rt) + ", #" + to_string(I.sa);
                    // it shifts left
                    disout << I.binstr << "\t" << I.instStr << endl; 
                    cout << I.binstr << "\t" << I.instStr << endl;
                }

                else if(I.opcode == 32 && I.funct == 2){
                    I.instStr = "SRL\tR" + to_string(I.rd) + ", R" + to_string(I.rt) + ", #" + to_string(I.sa);
                    //it shifts right
                    disout << I.binstr << "\t" << I.instStr << endl; 
                    cout << I.binstr << "\t" << I.instStr << endl;
                    }
                else if (I.opcode == 32 && I.funct == 13) {
                    breakVal = false;
                    I.instStr = "BREAK";
                    disout << I.binstr << "\t" << I.instStr << endl; 
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                    

            }

            MEM[addr] = I;
            addr+=4;
        }
    } 

    // end of decode

    int PC = 96;
    int R[32] = {0};
    int cycle = 1;
    breakVal = true;
    while( breakVal ){

        item I = MEM[PC];
        if (I.valid == 0 ){
            PC += 4;
            I = MEM[PC];
        }
        else {
            if(I.opcode == 34) {
                PC = I.instr_index;
            }
            else if (I.opcode == 36){
                PC += I.offset;
            }
            else if (I.opcode == 32 && I.funct == 8) {
                PC = R[I.rs];
            }
            else if ( I.opcode == 33 && R[I.rt] == 0) {
                PC += I.offset;
            }
            else if (I.opcode == 32 && I.funct == 32) {
                R[I.rd] = R[I.rs] + R[I.rt];
            }
            else if(I.opcode == 40){
                R[I.rt] = R[I.rs] + I.imm;
            }
            else if(I.opcode == 60 && I.funct == 2){
	        R[I.rd] = R[I.rs] * R[I.rt];
            }
            else if(I.opcode == 32 && I.funct == 36) {
                if(R[I.rs] == 1 && R[I.rt] == 1){
                    R[I.rd] = 1;
                }
                else {
                    R[I.rd] = 0;
                }
            }
            else if(I.opcode == 32 && I.funct ==37) {
                if(R[I.rs] == 1 || R[I.rt] == 1){
                    R[I.rd] = 1;
                }
                else {
                    R[I.rd] = 0;
                }
            }
            else if(I.opcode == 32 && I.funct == 5) {
                if(R[I.rt] == 0) {
                    R[I.rd] = R[I.rs];
                }
            }
            else if(I.opcode == 32 && I.funct == 34) {
            R[I.rd] = R[I.rs] - R[I.rt];
            }
            else if(I.opcode == 43){
                MEM[I.rs + I.offset].rt = R[I.rt];
            }

            else if(I.opcode == 35){
                R[I.rt] = MEM[I.rs + I.offset].rt;
            }

            else if(I.opcode == 32 && I.sa > 0 && I.funct == 0){
                R[I.rd] = R[I.rt] << I.sa;
            }
            
            else if(I.opcode == 32 && I.funct == 2){
                R[I.rd] = R[I.rt] >> I.sa;
            }
            else if (I.opcode == 0 && I.funct == 13){
                breakVal = false;
            }

        cout << "==================\ncycle: " + to_string(cycle)
                + " " + to_string(PC) + "\t" + I.instStr + " " + " " + "\n\nregisters:\n";
        for (int i = 0; i < 32; i++) {
            cout << to_string(R[i]) << "\t";
            if (i % 7 == 0) {
                cout << endl;
            }
            }
        simout << "==================\ncycle: " + to_string(cycle)
                + " " + to_string(PC) + "\t" + I.instStr + " " + " " + "\n\nregisters:\n";
                for (int i = 0; i < 32; i++) {
                    cout << to_string(R[i]) << "\t";
                    if (i % 7 == 0){
                        cout << endl;
                    }
                }
        }
        PC += 4;
        cycle ++;
        }
       
    }

