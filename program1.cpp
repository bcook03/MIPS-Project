#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <bitset>
#include <iomanip>
using namespace std;

int main()
{
        char buffer[4];
        int i;
        char * iPtr;
        iPtr = (char*)(void*) &i;

        int FD = open("test1.bin", O_RDONLY);

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

            if ( I.opcode == 2 ){
                
            }
			if( I.opcode == 40 ){
				I.instStr = "ADDI\tR" + to_string(rt) + ", R" + to_string(rs) + ", #" + to_string(imm);
                cout << I.binStr << "\t" << instStr << endl;
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