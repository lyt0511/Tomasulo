#include <iostream>
#include <conio.h>
#include <stdio.h>
#include "variables.h"

using namespace std;

static INST_Table inst[30];

const int fu_count = 7;

//��ʼ��״̬��
void init_statusTable()
{
	//��ʼ������վ�洢rs
	for (int i = 0; i < 30; i++)
	{
		rs[i] = 0;
	}
	//��ʼ����2
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			function_status[i][j] = 0;
		}
	}

	//��ʼ����3
	for (int i = 0; i < 30; i++)
		register_status[i] = 0;
}

//ָ������
int input_inst()
{
	int count = 0;
	char op[10] = "", r1[5] = "", r2[5] = "", r3[5] = "";
	cout << "������ָ��(��end����):" << endl;
	cin >> op;
	while (strcmp(op, "end") != 0)
	{
		cin >> r1 >> r2 >> r3;
		inst[++count].Input(op, r1, r2, r3);
		cout << "���������ָ��(��end����):" << endl;
		cin >> op;
	}
	return count;
}

int input_inst_exa()
{
	const int count = 6;
	char instr[count][50] = {
		"LD  F6  34  R2",
		"LD  F2  45  R3",
		"MULTD F1 F2 F4",
		"SUBD  F8 F6 F2",
		"DIVD  F10 F1  F6",
		"ADDD  F6  F8  F2"
	};	

	cout << "ָ�Ϊ��" << endl;
	char op[10] = "", r1[5] = "", r2[5] = "", r3[5] = "";
	for (int i = 0; i < count; i++)
	{
		char tmp_instr[50] = { 0 };
		strcpy(tmp_instr, instr[i]);
		cout << tmp_instr << endl;
		const char *split = " ";
		char *p;
		p = strtok(tmp_instr, split);
		strcpy(op, p);
		p = strtok(NULL, split);
		strcpy(r1, p);
		p = strtok(NULL, split);
		strcpy(r2, p);
		p = strtok(NULL, split);
		strcpy(r3, p);
		inst[i + 1].Input(op, r1, r2, r3);

	}
	return count;
}

//�ú��������޸ı�2�ͱ�3��״̬
void alter_table_j(FUNCTION_UNIT fu, int i, int count, bool flag = 0)
{
	inst[i].setFU(fu);
	//�޸�״̬��2
	function_status[fu][BUSY] = yes;
	function_status[fu][OP] = inst[i].ins->name;
	if (flag)
		function_status[fu][A] = inst[i].ins->sr1;
	else
		function_status[fu][A] = 0;
	//�޸�״̬��3
	register_status[inst[i].ins->dest] = fu;

	//����R1-4�Ĵ���ֱ�Ӷ�ȡ
	if (inst[i].ins->sr1 > 10)
	{
		rs[inst[i].ins->sr1 + count] = 1;
		function_status[fu][VJ] = inst[i].ins->sr1 + count;
	}
	else
	{
		//�����ɸüĴ���ֵ��ָ��δִ���꣬������QK���ȴ��ù��ܵ�Ԫִ����
		for (int j = 1; j <= 29; j++)
		{
			if (register_status[j] != 0 && register_status[j] <= fu_count)
			{
				if (j == inst[i].ins->sr1)
				{
					function_status[fu][QJ] = register_status[j];
					return;
				}
			}
		}

		int flag = 1;
		//ǰ��������Ե�ָ���Ѿ���ɣ���д�뱣��վ����ʹ�ö�Ӧ�ı���վ��ֵ
		for (int j = 1; j <= count; j++)
		{
			if (rs[j] != 0)
			{
				if (rs[j] == inst[i].ins->sr1)
				{
					function_status[fu][VJ] = j;
					flag = 0;
				}
			}
		}
		//��û��ָ������üĴ�����ֵ����ֱ�ӶԼĴ������ж�ȡ
		if (flag == 1)
		{
			rs[inst[i].ins->sr1 + count] = 1;
			function_status[fu][VJ] = inst[i].ins->sr1 + count;
		}
	}

}

void alter_table_k(FUNCTION_UNIT fu, int i, int count, bool flag = 0)
{
	inst[i].setFU(fu);
	//�޸�״̬��2
	function_status[fu][BUSY] = yes;
	function_status[fu][OP] = inst[i].ins->name;
	if (flag)
		function_status[fu][A] = inst[i].ins->sr1;
	else
		function_status[fu][A] = 0;
	//�޸�״̬��3
	register_status[inst[i].ins->dest] = fu;

	//����R1-4�Ĵ���ֱ�Ӷ�ȡ
	if (inst[i].ins->sr2 > 10)
	{
		rs[inst[i].ins->sr2 + count] = 1;
		function_status[fu][VK] = inst[i].ins->sr2 + count;
	}
	else
	{
		//�����ɸüĴ���ֵ��ָ��δִ���꣬������QK���ȴ��ù��ܵ�Ԫִ����
		for (int j = 1; j <= 29; j++)
		{
			if (register_status[j] != 0 && register_status[j] <= fu_count)
			{
				if (j == inst[i].ins->sr2)
				{
					function_status[fu][QK] = register_status[j];
					return;
				}
			}
		}

		int flag = 1;
		//ǰ��������Ե�ָ���Ѿ���ɣ���д�뱣��վ����ʹ�ö�Ӧ�ı���վ��ֵ
		for (int j = 1; j <= count; j++)
		{
			if (rs[j] != 0)
			{
				if (rs[j] == inst[i].ins->sr2)
				{
					function_status[fu][VK] = j;
					flag = 0;
				}
			}
		}
		//��û��ָ������üĴ�����ֵ����ֱ�ӶԼĴ������ж�ȡ
		if (flag == 1)
		{
			rs[inst[i].ins->sr2 + count] = 1;
			function_status[fu][VK] = inst[i].ins->sr2 + count;
		}
	}

}

//ָ������׶ε��ж��Լ�״̬����޸�
void inst_issue(int i, int count)
{
	int p = i - 1;
	switch (inst[i].ins->name)
	{
	case LD:
		if ((function_status[LOAD1][BUSY] == no) && (inst[i].instruction_status[IS] == 0))
		{
			if ((p > 0 && (inst[p].instruction_status[IS] != 0) && (Cycle > inst[p].issue) && (Cycle != inst[p].writeback)) || p == 0)
			{
				alter_table_k(LOAD1, i, count, 1);
				break;
			}
			else
				return;
		}
		else if ((function_status[LOAD2][BUSY] == no) && (inst[i].instruction_status[IS] == 0))
		{
			if ((p > 0 && (inst[p].instruction_status[IS] != 0) && (Cycle > inst[p].issue) && (Cycle != inst[p].writeback)) || p == 0)
			{
				alter_table_k(LOAD2, i, count, 1);
				break;
			}
			else
				return;
		}
		else
			return;
	case MULTD:
		if ((function_status[MULT1][BUSY] == no) && (inst[i].instruction_status[IS] == 0))
		{
			if ((p > 0 && (inst[p].instruction_status[IS] != 0) && (Cycle > inst[p].issue) && (Cycle != inst[p].writeback)) || p == 0)
			{
				alter_table_j(MULT1, i, count);
				alter_table_k(MULT1, i, count);
				break;
			}
			else
				return;
		}
		else if ((function_status[MULT2][BUSY] == no) && (inst[i].instruction_status[IS] == 0))
		{
			if ((p > 0 && (inst[p].instruction_status[IS] != 0) && (Cycle > inst[p].issue) && (Cycle != inst[p].writeback)) || p == 0)
			{
				alter_table_j(MULT2, i, count);
				alter_table_k(MULT2, i, count);
				break;
			}
			else
				return;
		}
		else
			return;
	case SUBD:
		if ((function_status[ADD1][BUSY] == no) && (inst[i].instruction_status[IS] == 0))
		{
			if ((p > 0 && (inst[p].instruction_status[IS] != 0) && (Cycle > inst[p].issue) && (Cycle != inst[p].writeback)) || p == 0)
			{

				alter_table_j(ADD1, i, count);
				alter_table_k(ADD1, i, count);
				break;
			}
			else
				return;
		}
		else if ((function_status[ADD2][BUSY] == no) && (inst[i].instruction_status[IS] == 0))
		{
			if ((p > 0 && (inst[p].instruction_status[IS] != 0) && (Cycle > inst[p].issue) && (Cycle != inst[p].writeback)) || p == 0)
			{
				alter_table_j(ADD2, i, count);
				alter_table_k(ADD2, i, count);
				break;
			}
			else
				return;
		}
		else if ((function_status[ADD3][BUSY] == no) && (inst[i].instruction_status[IS] == 0))
		{
			if ((p > 0 && (inst[p].instruction_status[IS] != 0) && (Cycle > inst[p].issue) && (Cycle != inst[p].writeback)) || p == 0)
			{
				alter_table_j(ADD3, i, count);
				alter_table_k(ADD3, i, count);
				break;
			}
			else
				return;
		}
		else
			return;
	case DIVD:
		if ((function_status[MULT1][BUSY] == no) && (inst[i].instruction_status[IS] == 0))
		{
			if ((p > 0 && (inst[p].instruction_status[IS] != 0) && (Cycle > inst[p].issue) && (Cycle != inst[p].writeback)) || p == 0)
			{
				alter_table_j(MULT1, i, count);
				alter_table_k(MULT1, i, count);
				break;
			}
			else
				return;
		}
		else if ((function_status[MULT2][BUSY] == no) && (inst[i].instruction_status[IS] == 0))
		{
			if ((p > 0 && (inst[p].instruction_status[IS] != 0) && (Cycle > inst[p].issue) && (Cycle != inst[p].writeback)) || p == 0)
			{
				alter_table_j(MULT2, i, count);
				alter_table_k(MULT2, i, count);
				break;
			}
			else
				return;
		}
		else
			return;
	case ADDD:
		if ((function_status[ADD1][BUSY] == no) && (inst[i].instruction_status[IS] == 0))
		{
			if ((p > 0 && (inst[p].instruction_status[IS] != 0) && (Cycle > inst[p].issue) && (Cycle != inst[p].writeback)) || p == 0)
			{
				alter_table_j(ADD1, i, count);
				alter_table_k(ADD1, i, count);
				break;
			}
			else
				return;
		}
		else if ((function_status[ADD2][BUSY] == no) && (inst[i].instruction_status[IS] == 0))
		{
			if ((p > 0 && (inst[p].instruction_status[IS] != 0) && (Cycle > inst[p].issue) && (Cycle != inst[p].writeback)) || p == 0)
			{
				alter_table_j(ADD2, i, count);
				alter_table_k(ADD2, i, count);
				break;
			}
			else
				return;
		}
		else if ((function_status[ADD3][BUSY] == no) && (inst[i].instruction_status[IS] == 0))
		{
			if ((p > 0 && (inst[p].instruction_status[IS] != 0) && (Cycle > inst[p].issue) && (Cycle != inst[p].writeback)) || p == 0)
			{
				alter_table_j(ADD3, i, count);
				alter_table_k(ADD3, i, count);
				break;
			}
			else
				return;
		}
		else
			return;
	}

	//�޸�״̬��1
	inst[i].setIS(Cycle);
	inst[i].setSTATUS(IS, Cycle);
}

void inst_readop(int i)
{
	int tmp = inst[i].getFU();
	int Qj = function_status[tmp][QJ];
	int Qk = function_status[tmp][QK];
	
	if (Qj != 0 || Qk != 0)
		return;
	else
	{
		inst[i].setRD(Cycle);
		inst[i].setEXC(1);
	}
}

void inst_execution(int i)
{
	//�޸�״̬��1
	inst[i].setEX(Cycle);
	inst[i].setSTATUS(EX, Cycle);
}

void inst_writeback(int i)
{
	//�޸�״̬��1
	inst[i].setWB(Cycle);
	inst[i].setSTATUS(WB, Cycle);
}

//ִ��ģ��
void execution(int count, int label = 0)
{
	int flag = 1;

	//ʱ������
	while (flag)
	{
		//ÿ��cycle����ÿ��ָ�����״̬����
		for (int i = 1; i <= count; i++)
		{
			if (inst[i].instruction_status[IS] == 0)
			{
				inst_issue(i, count);
			}
			else
			{
				if (inst[i].readop == 0)
				{
					inst_readop(i);
				}
				else
				{
					if (inst[i].instruction_status[EX] == 0 && Cycle - inst[i].readop == inst[i].ins->cycle)
					{
						inst_execution(i);
					}
					else
					{
						if (inst[i].instruction_status[EX] != 0 && inst[i].instruction_status[WB] == 0)
						{
							inst_writeback(i);
						}
					}
				}
			}


			//����ִ�����ָ��������excλ�ͱ�2��Ӧ��Busyλ��λ��0
			if (inst[i].instruction_status[WB] != 0 && inst[i].exc == 1)
			{
				int tmp_fu = inst[i].getFU();
				function_status[tmp_fu][BUSY] = no;
				inst[i].setEXC(0);
				//��ִ����ָ���Ŀ�ļĴ�������rs[i]
				rs[i] = inst[i].ins->dest;
				register_status[inst[i].ins->dest] = i + fu_count;
				//�㲥i֮��������ڵȴ���������ָ�����������Ե�ָ���V�ֶθ���Ϊrs[i]
				for (int j = i+1; j <= count; j++)
				{
					int tmp = inst[j].getFU();
					if (function_status[tmp][BUSY] == yes)
					{
						if (function_status[tmp][QJ] == tmp_fu)
						{
							function_status[tmp][QJ] = 0;
							function_status[tmp][VJ] = i;
						}
						if (function_status[tmp][QK] == tmp_fu)
						{
							function_status[tmp][QK] = 0;
							function_status[tmp][VK] = i;
						}
					}
				}
			}
		}



		//�����е�ָ���д�أ�����ֹ
		int cnt;
		for (cnt = 1; cnt <= count; cnt++)
		{
			if (inst[cnt].instruction_status[WB] == 0)
			{
				flag = 1;
				break;
			}
		}

		if (label != 0)
		{
			if (Cycle == label)
				return;
		}

		if (cnt > count)
			flag = 0;
		Cycle++;
	}
}

//״̬�����
void output_statusTable(int count)
{
	cout << '\n';

	cout << "ָ��״̬��" << endl;
	cout << "******************************************************************************" << endl;
	for (int i = 1; i <= count; i++)
	{
		for (int j = 1; j < 4; j++)
		{
			if (inst[i].instruction_status[j] == 0)
				cout << " " << '\t';
			else
				cout << inst[i].instruction_status[j] << '\t';
		}
		cout << '\n';
	}
	cout << "******************************************************************************" << endl << endl;
	cout << "���ܵ�Ԫ״̬��" << endl;
	cout << "******************************************************************************" << endl;
	cout << "BUSY" << '\t' << "OP" << '\t' << "Vj" << '\t' << "Vk" << '\t' << "Qj" << '\t' << "Qk" << '\t' << "A" << '\t' << '\n';
	for (int i = 1; i <= 7; i++)
	{
		for (int j = 1; j <= 7; j++)
		{
			switch (j)
			{
			case 1:
				if (function_status[i][j])
					cout << "yes" << '\t';
				else
					cout << "no" << '\t';
				break;
			case 2:
				switch (function_status[i][j])
				{
				case 0:
					cout << "no" << '\t';
					break;
				case LD:
					cout << "LD" << '\t';
					break;
				case MULTD:
					cout << "MULTD" << '\t';
					break;
				case DIVD:
					cout << "DIVD" << '\t';
					break;
				case ADDD:
					cout << "ADDD" << '\t';
					break;
				case SUBD:
					cout << "SUBD" << '\t';
					break;
				}
				break;
			case 3: case 4:
				if (function_status[i][j] == 0)
					cout << "0" << '\t';
				else
				{
					if (function_status[i][j] > count)
						cout << "Reg" << function_status[i][j] - count << '\t';
					else
						cout << "Inst" << function_status[i][j] << '\t';
				}
				break;
			case 5: case 6:
				if (function_status[i][j] == 0)
					cout << "0" << '\t';
				else
				{
					switch (function_status[i][j])
					{
					case 0:
						cout << "no" << '\t';
						break;
					case LOAD1:
						cout << "LOAD1" << '\t';
						break;
					case LOAD2:
						cout << "LOAD2" << '\t';
						break;
					case ADD1:
						cout << "ADD1" << '\t';
						break;
					case ADD2:
						cout << "ADD2" << '\t';
						break;
					case ADD3:
						cout << "ADD3" << '\t';
						break;
					case MULT1:
						cout << "MULT1" << '\t';
						break;
					case MULT2:
						cout << "MULT2" << '\t';
						break;
					}
				}
				break;
			case 7:
				cout << function_status[i][j] << '\t';
				break;

			}
		}
		cout << '\n';
	}
	cout << "******************************************************************************" << endl << endl;

	cout << "�Ĵ���״̬��" << endl;
	cout << "******************************************************************************" << endl;
	for (int i = 0; i <= 29; i++)
	{
		if (register_status[i] != 0)
		{
			if (register_status[i] > fu_count)
			{
				cout << "F" << i << ":Inst" << register_status[i] - fu_count << '\t';
			}
			else
			{
				switch (register_status[i])
				{
				case LOAD1:
					cout << "F" << i << ":LOAD1" << '\t';
					break;
				case LOAD2:
					cout << "F" << i << ":LOAD2" << '\t';
					break;
				case ADD1:
					cout << "F" << i << ":ADD1" << '\t';
					break;
				case ADD2:
					cout << "F" << i << ":ADD2" << '\t';
					break;
				case ADD3:
					cout << "F" << i << ":ADD3" << '\t';
					break;
				case MULT1:
					cout << "F" << i << ":MULT1" << '\t';
					break;
				case MULT2:
					cout << "F" << i << ":MULT2" << '\t';
					break;
				}
			}
			
		}
		else
			cout << "F" << i << ":0" << '\t';

	}
	cout << '\n';
	cout << "******************************************************************************" << endl << endl;
	cout << '\n';
}

int main()
{
	init_statusTable();
	int count = input_inst_exa();
	execution(count);
	output_statusTable(count);

	system("pause");
	return 0;
}