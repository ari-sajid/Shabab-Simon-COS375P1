#include <stdio.h>
#include "MemoryStore.h"
#include "RegisterInfo.h"
#include "EndianHelpers.h"
#include <fstream>
#include <iostream>
using namespace std; 

union REGS 
{
    RegisterInfo reg;
    uint32_t registers[32] {0};
};

union REGS regData;

// TODO: fill in the missing hex values of the OP_IDs (opcodes)
enum OP_IDS
{
    //R-type opcodes...
    OP_ZERO = 0x0, // zero
    //I-type opcodes...
    OP_ADDI = 0x8, // addi
    OP_ADDIU = 0x9, // addiu
    OP_ANDI = 0xc, // andi
    OP_BEQ = 0x4, // beq
    OP_BNE = 0x5, // bne
    OP_LBU = 0x24, // lbu
    OP_LHU = 0x25, // lhu
    OP_LUI = 0xf, // lui
    OP_LW = 0x23, // lw
    OP_ORI = 0xd, // ori
    OP_SLTI = 0xa, // slti
    OP_SLTIU = 0xb, // sltiu
    OP_SB = 0x28, // sb
    OP_SH = 0x29, // sh
    OP_SW = 0x2b, // sw
    OP_BLEZ = 0x6, // blez
    OP_BGTZ = 0x7, // bgtz
    //J-type opcodes...
    OP_J = 0x2, // j
    OP_JAL = 0x3 // jal
};

// TODO: fill in the missing hex values of FUNCT_IDs (function IDs)
enum FUNCT_IDS
{
    FUN_ADD = 0x20, // add
    FUN_ADDU = 0x21, // add unsigned (addu)
    FUN_AND = 0x24, // and
    FUN_JR = 0x8, // jump register (jr)
    FUN_NOR = 0x27, // nor
    FUN_OR = 0x25, // or
    FUN_SLT = 0x2a, // set less than (slt)
    FUN_SLTU = 0x2b, // set less than unsigned (sltu)
    FUN_SLL = 0x0, // shift left logical (sll)
    FUN_SRL = 0x2, // shift right logical (srl)
    FUN_SUB = 0x22, // substract (sub)
    FUN_SUBU = 0x23 // substract unsigned (subu)
};

// extract specific bits [start, end] from a 32 bit instruction
uint extractBits(uint32_t instruction, int start, int end)
{
    int bitsToExtract = start - end + 1;
    uint32_t mask = (1 << bitsToExtract) - 1;
    uint32_t clipped = instruction >> end;
    return clipped & mask;
}

// sign extend smol to a 32 bit unsigned int
uint32_t signExt(uint16_t smol) 
{
    uint32_t x = smol;
    uint32_t extension = 0xffff0000;
    return (smol & 0x8000) ? x ^ extension : x;
}

// zero extend smol to a 32 bit unsigned int
uint32_t zeroExt(uint16_t smol) 
{
    uint32_t x = smol;
    return x;
}

// performs slt instruction for a signed value
uint32_t sltComp(uint32_t R_rs, uint32_t R_rt) {
    uint8_t rsMostSig = extractBits(R_rs, 31, 31);
    uint8_t rtMostSig = extractBits(R_rt, 31, 31);

    cout << "rsMostSig: " << rsMostSig << '\n';
    cout << "rtMostSig: " << rtMostSig << '\n';

    if (rsMostSig == 0 && rtMostSig == 0) {
        return (R_rs < R_rt ) ? 1 : 0;
    }

    if (rsMostSig == 1 && rtMostSig == 1) {
        return (R_rs > R_rt) ?  1 : 0;
    }

    // return 1 if R_rs is less than R_rt, otherwise return 0
    return (rtMostSig == 0) ? 1 : 0;
}

// dump registers and memory
void dump(MemoryStore *myMem) {

    dumpRegisterState(regData.reg);
    dumpMemoryState(myMem);
}

int main(int argc, char** argv) {

    // open instruction file
    ifstream infile;
    infile.open(argv[1], ios::binary | ios::in);

    // get length of the file and read instruction file into a buffer
    infile.seekg(0, ios::end);
    int length = infile.tellg();
    infile.seekg (0, ios::beg);

    char *buf = new char[length];
    infile.read(buf, length);
    infile.close();

    // initialize memory store with buffer contents
    MemoryStore *myMem = createMemoryStore();
    int memLength = length / sizeof(buf[0]);
    int i;
    for (i = 0; i < memLength; i++) {
        myMem->setMemValue(i * BYTE_SIZE, buf[i], BYTE_SIZE);
    }

    // initialize registers and our program counter
    regData.reg = {};
    uint32_t PC = 0;
    bool err = false;
    
    // variables to handle branch delay slot execution
    bool encounteredBranch = false;
    bool executedDelaySlot = false; 
    uint32_t savedBranch = 0;       // saved (delayed) branch instruction
    uint32_t savedPC = 0;           // PC when the branch wa encountered (PC for the instruction in memory after the branch instruction)
    
    // start simulation
    // TODO: complete simulation loop and implement branch delay logic
    while (!err) {

        // once the delay slot is completed, jump to the branch target address
        if (executedDelaySlot) {
            PC = savedBranch;
            executedDelaySlot = false;
        }

        // handle the branch instruction: execute the instruction immediately after it (delay slot)
        if (encounteredBranch) {
            PC = savedPC; 
            executedDelaySlot = true; 
            encounteredBranch = false; 
        }

        // fetch current instruction
        uint32_t instruction;
        myMem->getMemValue(PC, instruction, WORD_SIZE);

        // increment PC & reset zero register
        PC += 4;
        regData.registers[0] = 0;

        // check for halt instruction
        if (instruction == 0xfeedfeed) {
            dump(myMem);
            return 0;
        }

        // TODO: parse instruction by completing function calls to extractBits()
        // and set operands accordingly
        uint32_t opcode = extractBits(instruction, 31, 26);
        uint32_t rs = extractBits(instruction, 25, 21);
        uint32_t rt = extractBits(instruction, 20, 16);
        uint32_t rd = extractBits(instruction, 15, 11);
        uint32_t shamt = extractBits(instruction, 10, 6);
        uint32_t funct = extractBits(instruction, 5, 0);
        uint16_t immediate = extractBits(instruction, 15, 0);
        uint32_t address = extractBits(instruction, 25, 0);

        int32_t signExtImm = signExt(immediate);
        uint32_t zeroExtImm = zeroExt(immediate);

        uint32_t branchAddr = signExt(immediate) << 2;

        uint32_t left = extractBits(PC, 31, 28) << 28;
        uint32_t shiftAddr = address << 2;
        uint32_t jumpAddr = left | shiftAddr;

        switch(opcode) {
            uint32_t rsData = regData.registers[rs];
            uint32_t rtData = regData.registers[rt];
            case OP_ZERO: // R-type instruction 
                switch(funct) {

                    case FUN_ADD:                         
                        regData.registers[rd] = rsData + rtData;
                        break;
                    case FUN_ADDU: 
                        regData.registers[rd] = rsData + rtData;
                        break;
                    case FUN_AND: 
                        regData.registers[rd] = rsData & rtData;
                        break;
                    case FUN_JR: 
                        PC = rsData;
                        break;
                    case FUN_NOR: 
                        regData.registers[rd] = ~(rsData + rtData);
                        break;
                    case FUN_OR:
                        regData.registers[rd] = rsData | rtData;
                        break;
                    case FUN_SLT: 
                        regData.registers[rd] = sltComp(rsData, rtData);
                        break;
                    case FUN_SLTU: 
                        regData.registers[rd] = (rsData < rtData) ? 1 : 0;
                        break;
                    case FUN_SLL: 
                        regData.registers[rd] = rtData << shamt;
                        break;
                    case FUN_SRL: 
                        regData.registers[rd] = rtData >> shamt;
                        break;
                    case FUN_SUB:  
                        regData.registers[rd] = rsData - rtData;
                        break;
                    case FUN_SUBU: 
                        regData.registers[rd] = rsData - rtData;
                        break;
                    default:
                        fprintf(stderr, "\tIllegal operation...\n");
                        err = true;
                }
                break;

            case OP_ADDI: 
                regData.registers[rt] = rsData + signExtImm;
                break;
            case OP_ADDIU: 
                regData.registers[rt] = rsData + signExtImm;
                break;
            case OP_ANDI: 
                regData.registers[rt] = rsData & zeroExtImm;
                break;
            case OP_BEQ: 
                if (regData.registers[rt] == rsData) {
                    encounteredBranch = true;
                    savedPC = PC;
                    PC += branchAddr;
                    savedBranch = PC;
                }
                break;
            case OP_BNE:
                if (rtData != rsData) {
                    encounteredBranch = true;
                    savedPC = PC;
                    PC += branchAddr;
                    savedBranch = PC;
                }
                break;
            case OP_BLEZ:
                if ((rsData == 0 || extractBits(rsData, 31, 31)== 1) ? true : false) {
                    encounteredBranch = true;
                    savedPC = PC;
                    PC += branchAddr;
                    savedBranch = PC;
                }
                break;
            case OP_BGTZ: 
                if (!(rsData == 0 || extractBits(rsData, 31, 31)== 1) ? true : false) {
                    encounteredBranch = true;
                    savedPC = PC;
                    PC += branchAddr;
                    savedBranch = PC;
                }
                break;
            case OP_J: 
                encounteredBranch = true;
                savedPC = PC;
                PC = jumpAddr;
                savedBranch = PC;
                break;
            case OP_JAL: 
                encounteredBranch = true;
                savedPC = PC;
                regData.registers[31] = PC + 4;
                PC = jumpAddr;
                savedBranch = PC;
                break;
            case OP_LBU: 
                myMem->getMemValue(rsData + signExtImm, rtData, BYTE_SIZE);
                break; 
            case OP_LHU: 
                myMem->getMemValue(rsData + signExtImm, rtData, HALF_SIZE);
                break;
            case OP_LUI: 
                rtData = immediate << 16;
                break;
            case OP_LW: 
                myMem->getMemValue(rsData + signExtImm, rtData, WORD_SIZE);
                break;
            case OP_ORI: 
                rtData = rsData | zeroExtImm;
                break;
            case OP_SLTI: 
                rtData = sltComp(rsData, signExtImm);
                break;
            case OP_SLTIU: 
                rtData = (rsData < signExtImm ) ? 1 : 0;
                break;
            case OP_SB: 
                myMem->setMemValue(rsData + signExtImm, rtData, BYTE_SIZE);
                break;
            case OP_SH: 
                myMem->setMemValue(rsData + signExtImm, rtData, HALF_SIZE);
                break;
            case OP_SW: 
                myMem->setMemValue(rsData + signExtImm, rtData, WORD_SIZE);
                break; 
            default:
                fprintf(stderr, "\tIllegal operation...\n");
                err = true;
         }  

} 
        
    // dump and exit with error
    dump(myMem);
    exit(127);
    return -1;  
}






