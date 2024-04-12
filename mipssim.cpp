#include <map>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <bitset>
#include <iomanip>
#include <fstream>
using namespace std;

struct item{
        int i, rs, rt, rd, imm, opcode, valid,  instr_index, funct, hint, offset, sa, opp1, opp2, dest;
        unsigned int asUint;
        string instStr, binstr, binStrSpace;

    };
 bool didBreak = false;
 int preissue[4] = {0};
 int premem[2] = {0};
 int preALU[2] = {0};
 int postmem = {0};
 int postALU = {0};
 int aluValue = 0;
 int memValue = 0;
item MEM[500];
//map<int, item>MEM;
bool XBW( int rNum, int index ){
        for( int i = 0; i < index; i++ ) {
                if( preissue[i] !=0 && MEM[preissue[i]].dest == rNum) return true;
        }
        for( int i = 0; i < 2; i++ ) {
                if( premem[i] != 0 &&MEM[premem[i]].dest == rNum) return true;
        }
        for( int i = 0; i < 2; i++ ) {
                if( preALU[i] != 0 && MEM[preALU[i]].dest == rNum) return true;
        }
        if( postALU != 0 && MEM[postALU].dest == rNum) return true;
        if( postmem !=0 && MEM[postmem].dest == rNum) return true;
        return false;
}

bool WBR(int rNum, int index, item MEM[], int postalu, int postmem, int preissue[] ){
        for(int i=0; i < 4; i++){
            if (preissue[i] != 0 && MEM[preissue[i]].rs == rNum) return true;
            if (preissue[i] != 0 && MEM[preissue[i]].rt == rNum) return true;

            return false;
        }
        return false;
}

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
    ofstream simout(string (argv[4]) + "_pipeline.txt");
    /*
    struct item{
        int i, rs, rt, rd, imm, opcode, valid,  instr_index, funct, hint, offset, sa, opp1, opp2, dest;
        unsigned int asUint;
        string instStr, binstr, binStrSpace;

    };
    bool didBreak = false;
    */
    int addr = 96;
    int amt = 4;
    int dataStart, dataEnd;
    item MEM[500];
    while ( amt != 0 )
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
            I.binStrSpace = I.binstr;
            I.binStrSpace.insert(26, " "), I.binStrSpace.insert(21, " "), I.binStrSpace.insert(16, " "),
            I.binStrSpace.insert(11, " "), I.binStrSpace.insert(6, " "), I.binStrSpace.insert(1, " ");
            
            I.valid = asUint >> 31;
            I.opcode = asUint >> 26;
            I.funct = (asUint << 26) >> 26;
            
            I.rs = (asUint << 6)>>27;
            I.rt = (asUint << 11)>>27;
            I.imm = (asUint << 16) >> 16;
            I.rd = (asUint << 16) >> 27;
            I.sa = (asUint << 21) >> 27;

            I.instr_index = (asUint << 26) >> 24;
            I.offset = asUint << 20 >> 18;


                if (didBreak) 
                {
                    I.instStr = to_string(i);
                    I.binStrSpace = I.binstr;

                    disout << I.binstr << "\t" << I.instStr << endl;
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if (I.valid == 0){
                I.instStr = "Invalid Instruction";
                disout << I.binstr << "\t" << I.instStr << endl;
                cout << I.binstr << "\t" << I.instStr << endl;
                }
           
                else if (I.opcode == 34 ) {
                    I.instStr = "J\t#" + to_string(I.instr_index);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if (I.opcode == 32 && I.funct == 8) {
                    I.instStr = "JR\tR" + to_string(I.rs);
                    disout << I.binstr << " " << addr << "\t" <<I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if ( I.opcode == 33 && I.rt == 0) {
                    I.instStr = "BLTZ\tR" + to_string(I.rs) + ", #" + to_string(I.offset);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if ( I.opcode ==  36) {
                    I.instStr = "BEQ\tR" + to_string(I.rt) + ", R" + to_string(I.rs) + ", #" + to_string(I.offset);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if ( I.opcode == 32 && I.funct == 32) {
                    I.instStr = "ADD\tR" + to_string(I.rt) + ", R" + to_string(I.rs) + ", R" + to_string(I.rd);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if( I.opcode == 40 ){
                    if (I.imm > 300) {
                        I.imm = -1;
                    }
                    I.instStr = "ADDI\tR" + to_string(I.rt) + ", R" + to_string(I.rs) + ", #" + to_string(I.imm);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 60 && I.funct == 2){
                    I.instStr = "MUL\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct == 36) {
                    I.instStr = "And\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct ==37) {
                    I.instStr = "Or\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt);
                    disout << I.binstr << "\t" << I.instStr << endl;
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct == 5) {
                    I.instStr = "MOVZ\tR" + to_string(I.rd) + ", R" + to_string(I.rs)+ ", R" + to_string(I.rt);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.sa == 0 && I.funct == 0) {
                    I.instStr = "NOP";
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct == 34) {
                    I.instStr = "SUB\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt); 
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }

                else if(I.opcode == 43){
                    I.instStr = "SW\tR" + to_string(I.rt) + ", " + to_string(I.imm) + "(R" + to_string(I.rs) + ')';
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }

                else if(I.opcode == 35){
                    I.instStr = "LW\tR" + to_string(I.rt) + ", " + to_string(I.imm) + "(R" + to_string(I.rs) + ')';
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }

                else if(I.opcode == 32 && I.sa > 0 && I.funct == 0){
                    I.instStr = "SLL\tR" + to_string(I.rd) + ", R" + to_string(I.rt) + ", #" + to_string(I.sa);
                    // it shifts left
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }

                else if(I.opcode == 32 && I.funct == 2){
                    I.instStr = "SRL\tR" + to_string(I.rd) + ", R" + to_string(I.rt) + ", #" + to_string(I.sa);
                    //it shifts right
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                    }
                else if (I.opcode == 32 && I.funct == 13) {
                    I.instStr = "BREAK";
                    didBreak = true;
                    dataStart = addr + 4;
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                    
            
            

            MEM[addr] = I;
            addr+=4;
        }
    } 
    dataEnd = addr;
    
    
    // end of decode

    int PC = 96;
    int R[32] = {0};
    int cycle = 1;
    didBreak = false;
 //int preissue[4] = {0};
 //int premem[2] = {0};
 //int preALU[2] = {0};
 //int postmem = {0};
 //int postalu = {0};

// struct line {
//     int validBit, dirtyBit, tag, data;
// };struct entry {
//     line Line[2] = {0};
// };
// struct set {
//     int LRUbit;
//     entry Entry[2] = {0};

// };

// set cache[4] = {0};

 struct fetch{
    void run(int preissue[], bool didBreak, item MEM[], int PC, int R[]){
        while(!didBreak)
        for (int i = 0; i < 2; i++) {
            //checks if there is room at pre-issue
            if (preissue[3] != 0) return;
            //checks if a break instruction was fetched which stalls fetch by breaking
            if (didBreak) break;
            item I = MEM[PC];

            // if I is branch or jump, try to do it then break
            //BLTZ opcode 000001(1)
            if((I.opcode == 33 && R[I.rt] == 0)){
                //want to find target address and break and check if it is taken
                if (R[I.rs] > 0){
                    PC = I.offset; 
                }
                break;
            }
            // J opcode 000010(2)
            else if(I.opcode == 34){
                //goes here
                PC = I.instr_index;
                break;
            }
            //JR opcode 001000(8)
            else if (I.opcode == 32 && I.funct == 8){
                PC = R[I.rs];
                break;
            }
            //BREAK opcode 001101(13)
            else if (I.opcode == 32 && I.funct == 13){
                didBreak = true;
                break;
            }
            // else move I to next open spot in preissue
            else {
                for (int j = 0; j < 4; j++) {
                    if (preissue[j] == 0)
                        preissue[j] = I.asUint;
                        break;

                }
            }
        

        }
    }
 };
    fetch FETCH;
 
 // Depending on the conditions, it either continues or updates the `preissue` array.
//- It rearranges elements in the `preissue` array based on certain conditions.
    struct issue{
        void run(int preissue[], int preALU[], int premem[], item MEM[]){
            for(int i = 0; i < 4; i++){
                if (preissue[i] == 0) continue;
                item I = MEM[preissue[i]];
                if (XBW( I.opp1, i)) continue;
                if (XBW(I.opp2, i)) continue;
                if (XBW(I.dest, i)) continue;
                // WBR Check
                if (WBR(I.rs, i, MEM, postALU, postmem, preissue)) continue;                           
                if (WBR(I.rt, i, MEM, postALU, postmem, preissue)) continue;                           
                if (WBR(I.rd, i, MEM, postALU, postmem, preissue)) continue;                             
                if (I.opcode == 35 || I.opcode == 43) {
                    if (premem[1] != 0) continue;
                    //LW SW checcks
                    //issue
                    for(int j = 0; j < 2; j++){
                        if(premem[j] == 0){
                            premem[j] = I.asUint;
                        }
                    }
                    preissue[i] = 0;
                }
                else {
                    //
                    for(int k = 0; k < 2; k++){
                        if(preALU[k] == 0){
                            preALU[k] = I.asUint;
                        }
                    }
                    preissue[i] = 0;
                }
            }
            for (int k = 0; k < 4; k++)
                for (int i = 3; i > 0; i--){
                    if(preissue[i-1] == 0){
                        preissue[i-1] = preissue[i];
                        preissue[i] = 0;
                    }
                }
        }
    };

    issue ISSUE;
 
/*
 Defines a struct `alu` with a method `run()`.
- The `run()` method iterates through a loop of 2 elements.
- It handles moving elements from `preALU` to `postALU` based on certain conditions.
*/
   struct alu{
        void run(int preALU[], item MEM[], int PC, int R[], int postalu){
            if(preALU[0] != 0){
                for(int i = 0; i < 2; i++){
                    if(postALU != 0) break;
                    item I = MEM[preALU[i]];
                    //if there is nothing in the preALU, do nothing
                    // if there is something in the preALU-move it to post, unless post is full
                        //ADDI R[I.rt] = R[I.rs] + I.imm;
                        if(I.opcode == 40){
                            postALU = I.rt; // destination
                            aluValue = R[I.rs] + I.imm;
                        }
                        //ADD R[I.rd] = R[I.rs] + R[I.rt];
                        if (I.opcode == 32 && I.funct == 32) {
                            postALU = I.rd; //destination
                            aluValue = R[I.rs] + R[I.rt];
                        }
                }
                //need to clear out the instruction executed and move the next down
                preALU[0] = preALU[1];
                preALU[1] = 0;
            }
        }

    };

    alu ALU;

    struct mem{
        void run(int premem[], item MEM[], int PC, int R[], int postmem){
            if(premem[0] != 0){
                for(int i = 0; i < 2; i++){
                    if(postmem != 0) break;
                    item I = MEM[premem[i]];
                    //if SW
                    if(I.opcode == 43){
                        MEM[I.rs + I.imm].funct = R[I.rt];
                    }
                    //LW
                    if(I.opcode == 35){
                        postmem = I.rt; //destination
                        memValue = MEM[I.rs + I.imm].funct;
                    }
                }
            }
        }
    };

    mem MEMO;

    struct write {
        void run(item MEM[], int R[], int postalu, int postmem, int aluValue, int memValue) {
            if (postmem != 0) {
                R[postmem] = memValue;
            }
            else if (postalu != 0) {
                R[postalu] = aluValue;

            }
            return;
        }
    };
 
 write WB;

 while(!didBreak){
    WB.run(MEM, R, postALU, postmem, aluValue, memValue);
    MEMO.run(premem, MEM, PC, R, postmem);
    ALU.run(preALU, MEM, PC, R, postALU);
    ISSUE.run(preissue, preALU, premem, MEM);
    FETCH.run(preissue, didBreak, MEM, PC,R);
    //print state

    //print state

        simout << "--------------------\n";
        simout << "Cycle" << cycle << ": " << endl;

        simout << "Pre-issue buffer: " << endl;
                for(int i = 0; i < 4; i++){
                        simout << "\t" << "Entry " << i << ":"<< "\t" << MEM[preissue[i]].instStr << endl;
                }
        simout << "Pre_ALU Queue: " << endl;
                for(int i = 0; i < 2; i++){
                        simout << "\t" << "Entry " << i << ":" << "\t" << MEM[preALU[i]].instStr << endl;
                }

        simout << "Post_ALU Queue: " << endl;
        simout << "\t" << "Entry 0: " << "\t" << MEM[postALU].instStr << endl;


        simout << "Pre_MEM Queue: " << endl;
                for(int i = 0; i < 2; i++){
                        simout << "\t" << "Entry " << i << ": " << "\t" << MEM[premem[i]].instStr << endl;
                }

        simout << "Post_MEM Queue: ";
        simout << "\t" << "Entry 0: " << "\t" << MEM[postmem].instStr << endl << endl;

        simout << "Registers";
        for(int i = 0; i < 32; i++) {
                if( (i + 1) % 8 == 0){
                        simout << endl;
                        std::string numstr = to_string(i);
                        if (numstr.size() == 1) numstr = '0' + numstr;
                        simout << "R" << numstr << ":";
                }
                simout << "\t" << R[i];
        }

        simout << "Data" << endl;

        int count = 1;

        for (int i = dataStart; i <= dataEnd; i+=4) {
            if (i == dataStart)
                simout << i << ":";
            if (count % 8 == 0){
                simout << endl << i << ":";
            }
            simout << "\t" << MEM[i].asUint;
        }
        cycle += 1;
    }

    
 }





