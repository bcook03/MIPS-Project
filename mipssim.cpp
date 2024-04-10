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
    ofstream simout(string (argv[4]) + "_pipeline.txt");

    struct item{
        int i, rs, rt, rd, imm, opcode, valid,  instr_index, funct, hint, offset, sa, opp1, opp2, dest;
        unsigned int asUint;
        string instStr, binstr, binStrSpace;

    };
    bool didBreak = false;
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


                if (didBreak) {
                {
                    I.instStr = to_string(i);
                    I.binStrSpace = I.binstr;
                }
                if (I.valid == 0){
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
    breakVal = true;
    while( breakVal && cycle <= 144 ){

        item I = MEM[PC];
        if (I.valid == 0 && cycle <= 144 ){
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
                if (R[I.rs] < 0) {
                    PC = I.offset;
                }
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
                MEM[I.rs + I.imm].funct = R[I.rt];
            }

            else if(I.opcode == 35){
                R[I.rt] = MEM[I.rs + I.imm].funct;
            }

            else if(I.opcode == 32 && I.sa > 0 && I.funct == 0){
                R[I.rd] = R[I.rt] << I.sa;
            }
            
            else if(I.opcode == 32 && I.funct == 2){
                R[I.rd] = R[I.rt] >> I.sa;
            }
            else if (I.opcode == 32 && I.funct == 13){
                breakVal = false;
            }

        cout << "==================\ncycle: " + to_string(cycle)
                + " \t" + to_string(PC) + "\t" + I.instStr + " " + " " + "\n\nregisters:\nr00:\t";
                for (int i = 1; i <= 32; i++) {
                    cout << to_string(R[i-1]) << "\t";
                        if (i % 8 == 0 && i!= 32 ) {
                            cout << endl;
                            if (i < 10) {
                                cout << "r0" << to_string(i) << ":\t";
                            }
                            else {
                                cout << "r" << to_string(i) << ":\t";
                            }
                        }
                }
        cout << "\n\ndata:" << endl;
            for (int i = 172; i <= 264; i+=4 ){
                if (i == 172) {
                    cout << "172:\t";
                }
                else if (i == 204) {
                    cout << endl << "204:\t";
                }
                else if (i == 236) {
                    cout << endl << "236:\t";
                }
                if (MEM[i].valid == 1)
                cout << to_string(MEM[i].funct - 64) << "\t";
                else 
                cout << to_string(MEM[i].funct) << "\t";

            
        }
        cout << endl << endl;

                
        simout << "==================\ncycle: " + to_string(cycle)
                + " " + to_string(PC) + "\t" + I.instStr + " " + " " + "\n\nregisters:\n";
                for (int i = 1; i <= 32; i++) {
                    simout << to_string(R[i-1]) << "\t";
                    if (i % 8 == 0){
                        simout << endl;
                        if (i < 10) {
                            simout << "r0" << to_string(i) << ":\t";
                        }
                        else {
                            simout << "r" << to_string(i) << ":\t";
                        }
                    }
       
                }
        simout << "\n\ndata:" << endl;
            for (int i = dataStart; i <= dataEnd; i+=4 ){
                if (i == 172) {
                    simout << "172:\t";
                }
                else if (i == 204) {
                    simout << endl << "204:\t";
                }
                else if (i == 236) {
                    simout << endl << "236:\t";
                }
                if (MEM[i].valid == 1)
                simout << to_string(MEM[i].funct - 64) << "\t";
                else 
                simout << to_string(MEM[i].funct) << "\t";
                
            }
        simout << endl << endl;
        if (breakVal == false)
            break;
        PC += 4;
        cycle += 1;
        }
        if (breakVal == false)
            break;
    }
       


 didBreak = false;
 int preissue[4] = {0};
 int premem[2] = {0};
 int preALU[2] = {0};
 int postmem = {0};
 int postalu = {0};

struct line {
    int validBit, dirtyBit, tag, data;
};struct entry {
    line Line[2] = {0};
};
struct set {
    int LRUbit;
    entry Entry[2] = {0};

};

set cache[4] = {0};

 struct fetch{
    void run(int preissue[], bool didBreak, item MEM[], int PC, int R[], bool RBW_ERRORFOUND){
        while(!RBW_ERRORFOUND)
        for (int i = 0; i < 2; i++) {
            //checks if there is room at pre-issue
            if (preissue[3] != 0) break;
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
    bool checkRBW(int premem[], int preALU[], int preIssue[]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 2; j++) {
                    if (preIssue[i] == premem[j] || preIssue[i] == preALU[j]){
                        return true;
                    }
                    else {
                    return false;
                }
            }
        }
    }

    /*bool XBW( int rNum, int index, item MEM[], int preissue[], int preALU[], int premem[], int postALU ){
        for(int i=0; i < 4; i++){
            if(MEM[preissue[i]].dest == rNum) return true;
        }
        for (int i = 0; i < 2; i++){
            if(MEM[preALU[i]].dest == rNum) return true;
        }
        for (int i = 0; i < 2; i++){
            if(MEM[premem[i]].dest == rNum)
        }
        if (postALU != 0 && MEM[])
        
    }
    */

};
 struct issue {
    void run(int preissue[], int preALU[], int premem[]) {
        for(int i = 0; i < 4; i++) {
            int preissue_opcode = preissue[i] >> 26;
            if (preALU[1] != 0 && premem[1] != 0) break;
            for (int j = 0; j < 2; j++) {
                if ((preissue_opcode == 35 || preissue_opcode == 43) && premem[j] == 0) {
                    premem[j] == preissue[i];
                }
                else if (preALU[j] == 0) {
                    preALU[j] == preissue[i];
                }
            }
        }
    };
    
 };
 /*
    struct issue{
        void run(){
            for(int i = 0; i < 4; i++){
                if (preissue[i] == 0) continue;
                item I = MEM[preissue[i]];
                if (XBW( I.src1, i)) continue;
                if (XBW(I.src2, i)) continue;
                if (XBW(I.dest, i)) continue;
                // WBR Check
                if (LW or SW) {
                    if (premem[1] != 0) continue;
                    LW SW checcks
                    issue
                    preissue[i] = 0;
                }
                else {
                    //
                    issue
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
    }

 */
 
 fetch FETCH;
 issue ISSUE;

bool RBW_ErrorFound;
 while(!didBreak){
    // WB.run();
    // MEM.run();
    // ALU.run();
    ISSUE.run(preissue, preALU, premem);
    RBW_ErrorFound = FETCH.checkRBW(premem, preALU, preissue);
    FETCH.run(preissue, didBreak, MEM, PC,R, RBW_ErrorFound);
    //print state
    /*
    if(entry.is_empty){
        cout << 
    }
    







    */
    
 }
}

