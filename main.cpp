/**************************************
 *
 * University of Haifa
 *
 * Theory of compilation
 *
 * P machine compiler - code generation project
 *
 * Yosi Ben Asher
 * Email: yosibenasher@gmail.com
 *
 * Mariah Akree
 * Email: mariah.uni.haifa@gmail.com
 *
 **************************************/

#include "main.h"

extern int yydebug;
int switch_to_the_current_list = 1;
int case_max = 1000;
int* cases_counter = new int[1000];
int took_a_number = 0;
int inc_dec = 0;
//int case_counter = 1;
int switch_counter = 0;
int loop_counter = 1;
int ind_requered = 1;
int if_counter = 1;
int current_max = 1000;
int current_i = 0;
string** names = new string * [1000];
string** types = new string * [1000];
int* address = new int[1000];
int* space = new int[1000];
int* upper_bonds = new int[1000];
int* lower_bonds = new int[1000];
int* how_many_refs = new int[1000];
int* parameter_for_function2 = new int[1000];
int* by_ref_parameter = new int[1000];
int remember = 1;
string* current_Type = NULL;
int current_Dim = 0;
int current_size_of_type = 0;
string* current_Type_Name = NULL;
int last_bond_order = 0;
int for_def;
string* current_Type_for_ref;
int temp = 0;
int current_sub_part;
int first_rec_var = 0;
int size_of_rec = 0;
int record_ref = 0;
int declare_var = 0;
int refs = 0;
int refs_for_new = 0;;
int ref_number = 0;
int remember_Type = 1;


string** function_name = new string * [1000];//prog
int current_f = 0;//prog
int* function_upper_bond = new int[1000];//prog
int* function_static_link = new int[1000];//prog 
int* function_lower_bond = new int[1000];//prog
int* wrote_ssp = new int[1000];//porg 
int current_ssp = 0;//prog
int current_dec = 0;//prog 
int* parmeter_size = new int[1000];
int* function_parameter = new int[1000];
int* parameter_for_function = new int[1000];
int current_parameter =-1;
int in_parameter_list = 0;
int this_is_a_function =0 ;
int from_left_assigen= 0;
int test = 1;

void writeAST(Object* r, fstream& file)
{

	//Perform recursive tree print
	r->print(file);
}
void Pcode(Object* r, fstream& file)
{
	//Perform recursive tree print
	r->pcodegen(file);
}

Object* getTree(char* progFile)
{

	yydebug = 1;
	assert(progFile);

	// yyin is an external variable that been used in yyparse as pointer to the source file.
	yyin = fopen(progFile, "r");
	if (!yyin) {
		cerr << "Error: file " << progFile << " does not exst. Aborting ..." << endl;;
		exit(1);
	}
	assert(yyin);

	// yyparse is yacc function that parse the program, checks syntax and builds the program AST.
	yyparse();

	fclose(yyin);

	// root was initialized in yyparse while it was building the tree.
	// root is the pointer of the returning tree.
	assert(root);
	return(root);
}



int main(int argc, char* argv[])
{
	cout << "main1" << endl;
	for (int i = 0; i < current_max; i++)
	{
		names[i] = NULL;
		types[i] = NULL;
		address[i] = 0;
		space[i] = 0;
		lower_bonds[i] = 0;
		upper_bonds[i] = 0;
	}
	CodeGenerator codeGen;

	//check input arguments.
	if (argc < 2) {
		cerr << endl << "Input file is missing. Aborting ..." << endl;
		exit(1);
	}

	Object* theProgram = getTree(argv[1]);
	assert(theProgram == root);

	fstream treeFile(TREE_OUTPUT_TEXT_FILE, ios::out);
	fstream pcodeFile(OUTPUT_CODE_TEXT, ios::out);
	treeFile << AST_FILE_HEADER << endl;
	pcodeFile << AST_FILE_HEADER << endl;
	writeAST(theProgram, treeFile);
	Pcode(theProgram, pcodeFile);
	treeFile.close();
	cout << "current_i " << current_i << endl;
	for (int i = 0; i < current_i + 10; i++)
	{
		cout << "i = " << i << endl;
		cout << "name = ";
		if (names[i] == NULL)
		{
			cout << "NULL ";
		}
		else
		{
			cout << *names[i] << " ";
		}
		cout << "space = " << space[i] << " ";
		if (types[i] == NULL)
		{
			cout << "NULL ";

		}
		else
			cout << "Type = " << *types[i] << " ";
		cout << "lower_bond = " << lower_bonds[i] << " ";
		cout << "upper_bond = " << upper_bonds[i] << " ";
		cout << "refs = " << how_many_refs[i] << " ";
		cout << "by_ref = " << by_ref_parameter[i] << " ";
		cout << "address = " << address[i] << endl;
	}
	cout << "function" << endl;
	cout << "current_f " << current_f<<endl;
	for (int i = 0; i <= current_f + 10; i++)
	{
		cout << "i = " << i<<endl;
		if (function_name[i] != NULL)
			cout << " name = " << *function_name[i];
		else
			cout << "name = NULL";
		cout<<" parmeter_size = " << parmeter_size[i];
		cout << " function_lower = " << function_lower_bond[i];
		cout << " function_upper = " << function_upper_bond[i];
		cout << " function parameter = " << function_parameter[i];
		cout << " parameter_for_functoin = "<< parameter_for_function[i];
		cout << " static_link = " << function_static_link[i] << endl;
	}
	return (0);
}


