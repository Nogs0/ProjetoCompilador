#include "mycomp.h"
#include "listacodigo.h"

char instrucao[30];

int temp = -1;
int newTemp()
{
	return temp--;
}

int label = 0;
int newLabel()
{
	return label++;
}

int param = 0;
void newParam(char *name)
{
	if (param < 3)
		sprintf(name, "$a%d", param++);
}

void getName(int num, char *name)
{
	if (num >= 0)
		sprintf(name, "$s%d", num);
	else
		sprintf(name, "$t%d", -(num + 1));
}

void Atrib(struct no *Atrib, int $1, struct no $3)
{
	char name_dest[5];
	char name_orig[5];
	getName($3.place, name_orig);
	getName($1, name_dest);
	sprintf(instrucao, "\tmove %s,%s\n", name_dest, name_orig);
	create_cod(&Atrib->code);
	insert_cod(&Atrib->code, $3.code);
	insert_cod(&Atrib->code, instrucao);
}

void Li(struct no *Exp, int num)
{
	char name_dest[5];
	Exp->place = newTemp();
	create_cod(&Exp->code);
	getName(Exp->place, name_dest);
	sprintf(instrucao, "\tli %s,%d\n", name_dest, num);
	insert_cod(&Exp->code, instrucao);
}

void ExpAri(char *operacao, struct no *Exp, struct no Exp1, struct no Exp2)
{
	char name_reg1[5];
	char name_reg2[5];
	char name_temp[5];
	Exp->place = newTemp();

	create_cod(&Exp->code);
	insert_cod(&Exp->code, Exp1.code);
	insert_cod(&Exp->code, Exp2.code);
	getName(Exp1.place, name_reg1);
	getName(Exp2.place, name_reg2);
	getName(Exp->place, name_temp);
	sprintf(instrucao, "\t%s %s,%s,%s\n", operacao, name_temp,
			name_reg1, name_reg2);
	insert_cod(&Exp->code, instrucao);
}

void Div(struct no *Exp, struct no Exp1, struct no Exp2)
{
	char name_reg1[5];
	char name_reg2[5];
	char name_quociente[5];
	Exp->place = newTemp();

	create_cod(&Exp->code);
	insert_cod(&Exp->code, Exp1.code);
	insert_cod(&Exp->code, Exp2.code);

	getName(Exp1.place, name_reg1);
	getName(Exp2.place, name_reg2);
	getName(Exp->place, name_quociente);

	sprintf(instrucao, "\tdiv %s, %s\n", name_reg1, name_reg2);
	insert_cod(&Exp->code, instrucao);
	sprintf(instrucao, "\tmflo %s\n", name_quociente);
	insert_cod(&Exp->code, instrucao);
}

void ExpRel(char *branch, struct no *Exp, struct no Exp1, struct no Exp2)
{
	char name_reg1[5];
	char name_reg2[5];
	char name_temp[5];

	Exp->place = newTemp();
	getName(Exp->place, name_temp);
	create_cod(&Exp->code);
	insert_cod(&Exp->code, Exp1.code);
	insert_cod(&Exp->code, Exp2.code);

	getName(Exp1.place, name_reg1);
	getName(Exp2.place, name_reg2);
	getName(Exp->place, name_temp);

	sprintf(instrucao, "\tli %s,1\n", name_temp);
	insert_cod(&Exp->code, instrucao);
	newLabel();
	sprintf(instrucao, "\t%s %s,%s,L%d\n", branch, name_reg1, name_reg2, label);
	insert_cod(&Exp->code, instrucao);
	sprintf(instrucao, "\tli %s,0\n", name_temp);
	insert_cod(&Exp->code, instrucao);
	sprintf(instrucao, "L%d:\n", label);
	insert_cod(&Exp->code, instrucao);
}

void Println(struct no *Print, struct no Exp)
{
	char name_reg[10];
	create_cod(&Print->code);
	getName(Exp.place, name_reg);
	insert_cod(&Print->code, Exp.code);
	sprintf(instrucao, "\tli $v0, 1\n");
	insert_cod(&Print->code, instrucao);

	sprintf(instrucao, "\tmove $a0,%s\n", name_reg);
	insert_cod(&Print->code, instrucao);

	sprintf(instrucao, "\tsyscall\n");
	insert_cod(&Print->code, instrucao);

	sprintf(instrucao, "\tli $v0,11\n");
	insert_cod(&Print->code, instrucao);

	sprintf(instrucao, "\tli $a0,'\\n'\n");
	insert_cod(&Print->code, instrucao);

	sprintf(instrucao, "\tsyscall\n");
	insert_cod(&Print->code, instrucao);
}

void Read(struct no *Read_cmd, int reg)
{
	// falta adaptar a lista de codigos
	char name_reg[10];
	create_cod(&Read_cmd->code);
	sprintf(instrucao, "\tli $v0, 5\n");
	insert_cod(&Read_cmd->code, instrucao);

	getName(reg, name_reg);
	sprintf(instrucao, "\tsyscall\n");
	insert_cod(&Read_cmd->code, instrucao);

	sprintf(instrucao, "\tmove %s,$v0\n", name_reg);
	insert_cod(&Read_cmd->code, instrucao);
}

void If(struct no *If_cmd, struct no Exp, struct no Compound)
{
	// adaptado a lista de codigos
	char reg_temp[10];

	create_cod(&If_cmd->code);
	insert_cod(&If_cmd->code, Exp.code);

	getName(Exp.place, reg_temp);
	newLabel();

	sprintf(instrucao, "\tbeq %s,0,END_IF%d\n", reg_temp, If_cmd->place);
	insert_cod(&If_cmd->code, instrucao);

	insert_cod(&If_cmd->code, Compound.code);

	sprintf(instrucao, "END_IF%d:\n", If_cmd->place);
	insert_cod(&If_cmd->code, instrucao);
}

void IfElse(struct no *If_cmd, struct no Exp, struct no Compound1, struct no Compound2)
{
	// adaptado a lista de codigos
	char reg_temp[10];
	int aux_label;

	create_cod(&If_cmd->code);
	insert_cod(&If_cmd->code, Exp.code);

	getName(Exp.place, reg_temp);
	newLabel();

	sprintf(instrucao, "\tbeq %s,0,L%d\n", reg_temp, label);
	insert_cod(&If_cmd->code, instrucao);

	insert_cod(&If_cmd->code, Compound1.code);

	aux_label = label;
	newLabel();
	sprintf(instrucao, "\tj L%d\n", label);
	insert_cod(&If_cmd->code, instrucao);

	sprintf(instrucao, "L%d:", aux_label);
	insert_cod(&If_cmd->code, instrucao);

	insert_cod(&If_cmd->code, Compound2.code);

	sprintf(instrucao, "L%d:", label);
	insert_cod(&If_cmd->code, instrucao);
}

void While(struct no *While_cmd, struct no Exp, struct no Compound)
{
	char reg_temp[10];
	int aux_label;
	aux_label = newLabel();

	create_cod(&While_cmd->code);
	sprintf(instrucao, "WHILE%d:\n", While_cmd->place);
	insert_cod(&While_cmd->code, instrucao);
	insert_cod(&While_cmd->code, Exp.code);
	getName(Exp.place, reg_temp);

	newLabel();
	sprintf(instrucao, "\tbeq %s, 0, END_WHILE%d\n", reg_temp, While_cmd->place);
	insert_cod(&While_cmd->code, instrucao);
	insert_cod(&While_cmd->code, Compound.code);
	sprintf(instrucao, "\tj WHILE%d\n", While_cmd->place);
	insert_cod(&While_cmd->code, instrucao);
	sprintf(instrucao, "END_WHILE%d:\n", While_cmd->place);
	insert_cod(&While_cmd->code, instrucao);
}

void DoWhile(struct no *DoWhile_cmd, struct no Compound, struct no Exp)
{
	char reg_temp[10];
	int aux_label;
	newLabel();
	int auxLabel = label;

	create_cod(&DoWhile_cmd->code);
	sprintf(instrucao, "L%d:\n", aux_label);
	insert_cod(&DoWhile_cmd->code, instrucao);

	insert_cod(&DoWhile_cmd->code, Compound.code);
	insert_cod(&DoWhile_cmd->code, Exp.code);
	getName(Exp.place, reg_temp);

	newLabel();
	sprintf(instrucao, "\tbeq %s, 0, L%d\n", reg_temp, label);
	insert_cod(&DoWhile_cmd->code, instrucao);

	sprintf(instrucao, "\tj L%d\n", aux_label);
	insert_cod(&DoWhile_cmd->code, instrucao);
	sprintf(instrucao, "L%d:\n", label);
	insert_cod(&DoWhile_cmd->code, instrucao);
}

void CallFunction(struct no *Func, int Id, struct no Arg)
{
	char reg_arg[5];
	getName(Arg.place, reg_arg);

	create_cod(&Func->code);
	sprintf(instrucao, "\tmove $a0, %s", reg_arg);
	sprintf(instrucao, "\tjal FUNC%d\n", Id);
	insert_cod(&Func->code, instrucao);
}

void Parameter(struct no *Arg, struct no Param)
{
	char reg_temp[5];
	char reg_param[5];

	getName(Param.place, reg_temp);
	newParam(reg_param);

	create_cod(&Arg->code);
	sprintf(instrucao, "\tmove %s, %s\n", reg_param, reg_temp);
	insert_cod(&Arg->code, instrucao);
}

void DeclArgs(struct no *Func, struct no Declps)
{
	char reg_arg[5];
	if (Declps.place != 0)
	{
		create_cod(&Func->code);
		getName(Declps.place, reg_arg);
		sprintf(instrucao, "\tmove %s, $a0\n", reg_arg);
		insert_cod(&Func->code, instrucao);
	}
}

void Function(struct no *Func, int Id, struct no Declps, struct no Compound)
{
	DeclArgs(Func, Declps);
	if (Func->place == 0)
	{
		insert_cod(&Func->code, Compound.code);
		sprintf(instrucao, "\tli $v0, 10\n\tsyscall\n");
		insert_cod(&Func->code, instrucao);
	}
	else
	{
		sprintf(instrucao, "FUNC%d:\n", Id);
		insert_cod(&Func->code, instrucao);
		insert_cod(&Func->code, Compound.code);
		sprintf(instrucao, "jr $ra\n");
		insert_cod(&Func->code, instrucao);
	}
}