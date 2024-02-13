#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <bitset>
#include <iomanip>
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
            int i, rs, rt, rd, imm, opcode, binstr, valid, instStr;
            
        }
        int addr = 96;
        int amt = 4;
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
            I.asUint = asUint

			bitset<32> b( i );
            I.binstr = b.to_string();
            
            I.valid = asUint >> 31;
            I.opcode = asUint >> 26
            
            I.rs = (asUint << 6)>>27;
            I.rt = (asUint << 11)>>27;
            I.imm = (i << 16) >> 16;

			cout << "valid bit: " << I.valid << endl;
			cout << "opcode: " << I.opcode << endl;
			cout << I.binstr << "\t"; 

            if (I.opcode == 2 ) {
                I.instr = "J\t#" + to_string (I.rt + I.rs + I.imm);
                cout << I.binstr << "\t" << I.instStr << endl;
            }
			else if( I.opcode == 40 ){
				I.instStr = "ADDI\tR" + to_string(I.rt) + ", R" + to_string(I.rs) + ", #" + to_string(I.imm);
                cout << I.binStr << "\t" << I.instStr << endl;
			}

            MEM[addr] = I;
            addr+=4;
		}
    } // end of decode

    int PC = 96;
    int R[32] = {0};
    int cycle = 1;

    while( true ){

        item I = MEM[PC];
        while (I.valid == 0 ){
            PC += 4;
            I = MEM[PC];
        }
        if(I.opcode == 2) {
            PC = I.rs + I.rt + I.imm;
        }
        if(I.opcode == 40){
            R[I.rt] = R[I.rs] + I.imm;
        }
        cout << "==================\ncycle: " + to_string(cycle)
                + " " + to_string(PC) + "\t" + I.instStr + " " + " " + "\n\nregisters:\n"
                + to_string(R[0]) + " " + to_string (R[1]) + "\n";

        PC += 4;
        cycle ++;

        if (cycle >= 2) break; 
    }



}