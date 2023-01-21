#ifndef _SIMPLE_HH_
#define _SIMPLE_HH_

#include<stdlib.h>
#include<stdint.h>

namespace simple
{
    const uint32_t 	N = 65536;		// 64 KiB memory
    uint8_t     	MEM[N];     		// memory is an array of N bytes
    uint32_t   	 	GPR[8];     		// 8 x 32-bit general purpose registers

    typedef enum
    {
	r0 = 0,
	r1 = 1,
	r2 = 2,
	r3 = 3,
	r4 = 4,
	r5 = 5,
	r6 = 6,
	r7 = 7
    } gprnum;					// valid GPR numbers

    struct
    {
	bool    LT;     			// less than
	bool    GT;     			// greater than
	bool    EQ;     			// equal to
    } Flags;					// flags

    uint32_t    CIA; 				// current instruction address
    uint32_t    NIA;    			// next instruction address

    uint64_t	instructions = 0;		// instruction counter
    uint64_t	cycles = 0;			// cycle counter

    void zeromem()
    {
	for (uint32_t i=0; i<N; i++) MEM[i] = 0;
    }

    void zeroctrs()
    {
	instructions = 0;
	cycles = 0;
    }

    void lbz(int RT, int RA)                	// load byte and zero-extend into a register
    {
	uint32_t EA = GPR[RA];
	GPR[RT] = MEM[EA];
    }

    void stb(int RS, int RA)                	// store byte from register
    {
	uint32_t EA = GPR[RA];
	MEM[EA] = GPR[RS] & 0xff;
    }

    void cmpi(int RA, int16_t SI)           	// compare the contents of a register with a signed integer
    {
	Flags.LT = false; Flags.GT = false; Flags.EQ = false;
	if      (GPR[RA] < SI) Flags.LT = true;
	else if (GPR[RA] > SI) Flags.GT = true;
	else   Flags.EQ = true;
    }

    void addi(int RT, int RA, int16_t SI)   	// add the contents of a register to a signed integer
    {
	GPR[RT] = GPR[RA] + SI;
    }

    bool beq(int16_t BD)                    	// branch if comparison resuts was "equal"
    {
	if (Flags.EQ) { NIA = CIA + BD; return true; }
	return false;
    }

    bool b(int16_t BD)                      	// unconditional branch
    {
	NIA = CIA + BD;
	return true;
    }
};

#define b(X)	if (b(0)) goto X
#define beq(X)	if (beq(0)) goto X

#endif
