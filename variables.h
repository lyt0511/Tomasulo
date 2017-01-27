#pragma once
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

# define yes 1
# define no 0

enum REGISTER { F1 = 1, F2, F3, F4, F5, F6, F7, F8, F9, F10, R1, R2, R3, R4 };

enum OP { LD = 1, MULTD, DIVD, ADDD, SUBD };

enum STAGE { IS = 1, EX, WB };

enum FUNCTION_UNIT { LOAD1 = 1, LOAD2, ADD1, ADD2, ADD3, MULT1, MULT2 };

//我们对VJ和VK的值进行特别的说明：
//这两个字段存储的都是保留站，对于指令写回重命名得到的保留站，该值表示为第 几 条指令写回的保留站
//对于直接存储器重命名得到的保留站，该值表示的是寄存器 编号+指令数
enum FU_LABEL { BUSY = 1, OP, VJ, VK, QJ, QK, A };

//定义rs为重命名后保留站编号存储的位置，我们做如下分配(假设指令数为count)：
//首先对于每条指令完成后写会的重命名，我们使用rs[1-count]进行存储，将指令编号设置为rs的下标，将写会寄存器的编号设置为对应的rs值，即rs[inst_i]=Fx
//然后对于可直接读取的寄存器的重命名，我们使用rs[count+1-29]进行存储，将寄存器编号+count设置为rs的下标，将其对应的值设置为1，即rs[Fx]=1
extern int rs[30];

extern int function_status[8][8];

extern int register_status[30];

extern int Cycle;

struct INSTRUCTION
{
	int name;
	int cycle;
	int dest;
	int sr1;
	int sr2;
};

class INST_Table
{
public:
	INSTRUCTION *ins;

	int instruction_status[4];
	int fu;
	int exc;
	int issue;
	int readop;
	int exeution;
	int writeback;

	INST_Table()
	{
		this->ins = new INSTRUCTION;

		this->instruction_status[0] = 0;
		this->instruction_status[1] = 0;
		this->instruction_status[2] = 0;
		this->instruction_status[3] = 0;

		this->fu = 0;
		this->exc = 0;
		this->issue = 0;
		this->readop = 0;
		this->exeution = 0;
		this->writeback = 0;
	}

	void setSTATUS(int i, int val);
	void setFU(FUNCTION_UNIT val);
	void setEXC(int val);
	void setIS(int val);
	//只留readop而不留function_unit中的RD字段
	void setRD(int val);
	void setEX(int val);
	void setWB(int val);
	int getFU();
	void Input(char name[], char rd[], char rs1[], char rs2[]);

	~INST_Table()
	{
		delete ins;
	}
};
