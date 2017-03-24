
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "Types1C.h"
#include "parser.h"
#include "analyser.h"


void InitParser(char* aText);
int yyparse(CCode** parse_res, CLexAnalyzer_1C* Lexer);
extern int yydebug;
extern int ObjCount;

CTypes1C Types;

void InitTypes()
{

	CType1C* ValList = new CType1C("��������������", NULL);
	ValList->AddProperty("�����", "Ref", "����������");
	ValList->AddProperty("�����", "Docum", "��������");
	Types.AddType("��������������", "ValueList", ValList);
	
	CType1C* Ref = new CType1C("����������", NULL);
	Ref->AddProperty("��������", "��������", "����������");
	Ref->AddProperty("��������", "��������", "����������");
	Ref->AddProperty("��������������", "��������������", "����������");
	Types.AddType("����������", "Reference", Ref);

	CType1C* Doc = new CType1C("��������", NULL);
	Doc->AddProperty("��������", "��������", "��������");
	Doc->AddProperty("���������������", "���������������", "��������.������");
	Types.AddType("��������", "Document", Doc);
}

void PrintLVal(CCode* Code)
{
	InitTypes();

	CModule Module(Code);
	//Module.DesiredLine = 5;
	//Module.DesiredCol = 40;
	Module.Analyse();
}

int main(int argc, char* argv[])
{
	char* locale = setlocale(LC_ALL, ".ACP");

	clock_t StartTime = clock();

	int sz = 1024 * 600;
	char *text = (char*)malloc(sz);
	
	char* fname = "test.1s";
	if( argc > 1) fname = argv[1];
	FILE *f = fopen(fname, "rt");
	int count = fread(text, 1, sz, f);
	printf("read: %i\n", count);
	fclose(f);

	{
	clock_t TotalTime = clock() - StartTime;
	printf("\nread Time: %f\n", (float)TotalTime / (float)CLOCKS_PER_SEC);
	}

	text[count] = '\0';
	CCode *parse_res = NULL;
	CLexAnalyzer_1C Lexer(text);
	yydebug = 1;
	yyparse(&parse_res, &Lexer);
	free(text);
	{
	clock_t TotalTime = clock() - StartTime;
	printf("\nparse Time: %f\n", (float)TotalTime / (float)CLOCKS_PER_SEC);
	}

	if( parse_res != NULL )
	{
		//printf("%s\n\n", parse_res->GetText());
		//parse_res->PrintTree();
		printf("\n");
		PrintLVal(parse_res);
		printf("\n\nobjects created %i\n", ObjCount);
		{
		clock_t TotalTime = clock() - StartTime;
		printf("\nanalyse Time: %f  (%i)\n", (float)TotalTime / (float)CLOCKS_PER_SEC, (int)CLOCKS_PER_SEC);
		}
		delete parse_res;
		{
		clock_t TotalTime = clock() - StartTime;
		printf("\ndel Time: %f  (%i)\n", (float)TotalTime / (float)CLOCKS_PER_SEC, (int)CLOCKS_PER_SEC);
		}
		printf("objects not deleted %i\n", ObjCount);
	}

	CMemCollector::Free();

	{
	clock_t TotalTime = clock() - StartTime;
	printf("\nTime: %f  (%i)\n", (float)TotalTime / (float)CLOCKS_PER_SEC, (int)CLOCKS_PER_SEC);
	}

	return 0;
}
