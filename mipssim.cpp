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

        int FD = open(argv[2], O_RDONLY);
        ofstream disout(string (argv[4]) + "_dis.txt");
        ofstream simout(string (argv[4]) + "_sim.txt");

        struct item{
            int i, rs, rt, rd, imm, opcode, binstr, valid,  instr_index, funct, hint, offset, sa;
            unsigned int asUint;
            string instStr,


        };
        int addr = 96;
        int amt = 4;
        item MEM[500];
        while( amt != 0 )
        {
            
                amt = read(FD, buffer, 4);
                if(amt == 4)
                {
                        iPtr[0] = buffer[3];
                        iPtr[1] = buffer[2];
                        iPtr[2] = buffer[1];
                        iPtr[3] = buffer[0];
                        item I;
                        cout << "i = " <<hex<< i << dec << endl;
                
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

			cout << "valid bit: " << I.valid << endl;
			cout << "opcode: " << I.opcode << endl;
			cout << I.binstr << "\t"; 

            if (I.opcode == 34 ) {                                   // Jump
                I.instStr = "J\t#" + to_string(I.instr_index);
                disout << I.binstr << "\t" << I.instStr << endl;       
                cout << I.binstr << "\t" << I.instStr << endl;
            }
            else if (I.opcode == 32 && I.funct == 8) {              // Jump Register
                I.instStr = "JR\tR" + to_string(I.rs);
                disout << I.binstr << "\t" << I.instStr << endl;        
                cout << I.binstr << "\t" << I.instStr << endl;
            }
            else if ( I.opcode == 33 && I.rt == 0) {                // Branch on Less Than Zero
                I.instStr = "BLTZ\tR" + to_string(I.rs) + ", #" + to_string(I.offset);      
                disout << I.binstr << "\t" << I.instStr << endl;                            
                cout << I.binstr << "\t" << I.instStr << endl;
            }
            else if ( I.opcode ==  36) {                            // Branch on Equal
                I.instStr = "BEQ\tR" + to_string(I.rt) + ", R" + to_string(I.rs) + ", #" + to_string(I.offset);
                disout << I.binstr << "\t" << I.instStr << endl;                
                cout << I.binstr << "\t" << I.instStr << endl;
            }
            else if ( I.opcode == 32 && I.funct == 32) {            // ADD Word
                I.instStr = "ADD\tR" + to_string(I.rt) + ", R" + to_string(I.rs) + ", R" + to_string(I.rd);
                disout << I.binstr << "\t" << I.instStr << endl; 
                cout << I.binstr << "\t" << I.instStr << endl;
            }
			else if( I.opcode == 40 ){                              // ADD Immediate
				I.instStr = "ADDI\tR" + to_string(I.rt) + ", R" + to_string(I.rs) + ", #" + to_string(I.imm);
                disout << I.binstr << "\t" << I.instStr << endl; 
                cout << I.binstr << "\t" << I.instStr << endl;
			}
            
            else if(I.opcode == 60 && I.funct == 2){                // Multiply Word
	            I.instStr = "MUL\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt);
                disout << I.binstr << "\t" << I.instStr << endl; 
	            cout << I.binstr << "\t" << I.instStr << endl;
            }
            else if(I.opcode == 32 && I.funct == 36) {              // AND operation
	            I.instStr = "And\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt);
                disout << I.binstr << "\t" << I.instStr << endl; 
	            cout << I.binstr << "\t" << I.instStr << endl;
            }
            else if(I.opcode == 32 && I.funct ==37) {               // OR operation
	            I.instStr = "Or\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt);
                disout << I.binstr << "\t" << I.instStr << endl; 
	            cout << I.binstr << "\t" << I.instStr << endl;
            }
            else if(I.opcode == 32 && I.funct == 5) {               
	            I.instStr = "MOVZ\tR" + to_string(I.rd) + ", R" + to_string(I.rs)+ ", R" + to_string(I.rt);
                disout << I.binstr << "\t" << I.instStr << endl; 
	            cout << I.binstr << "\t" << I.instStr << endl;
            }
            else if(I.opcode == 32) {                               
	            I.instStr = "NOP";
                disout << I.binstr << "\t" << I.instStr << endl; 
	            cout << I.binstr << "\t" << I.instStr << endl;
            }


            MEM[addr] = I;
            addr+=4;
		}
        if (I.opcode == 0 && I.funct == 13) break;
    }
    // end of decode

    int PC = 96;
    int R[32] = {0};
    int cycle = 1;

    while( true ){

        item I = MEM[PC];
        while (I.valid == 0 ){
            PC += 4;
            I = MEM[PC];
        }
        if(I.opcode == 34) {
            PC = I.instr_index;
        }
        else if (I.opcode == 36){
            PC += I.offset;
        }
        else if (I.opcode == 32 && I.funct == 8) {
            PC = R[I.rs];
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
        cout << "==================\ncycle: " + to_string(cycle)
                + " " + to_string(PC) + "\t" + I.instStr + " " + " " + "\n\nregisters:\n"
                + to_string(R[0]) + " " + to_string (R[1]) + "\n";
        simout << "==================\ncycle: " + to_string(cycle)
                + " " + to_string(PC) + "\t" + I.instStr + " " + " " + "\n\nregisters:\n"
                + to_string(R[0]) + " " + to_string (R[1]) + "\n";

        PC += 4;
        cycle ++;

        if (I.opcode == 0 && I.funct == 13) break; 
    }



}
