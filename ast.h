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
 /*   PROGRAM = 258,
	  BBEGIN = 259,
	  END = 260,
	  DECLARE = 261,
	  PROCEDURE = 262,
	  FUNCTION = 263,
	  LABEL = 264,
	  INTEGER = 265,
	  REAL = 266,
	  RECORD = 267,
	  BOOLEAN = 268,
	  ARRAY = 269,
	  OF = 270,
	  ASSIGN = 271,
	  LC = 272,
	  RC = 273,
	  IF = 274,
	  THEN = 275,
	  ELSE = 276,
	  WHILE = 277,
	  REPEAT = 278,
	  FI = 279,
	  DO = 280,
	  OD = 281,
	  READ = 282,
	  WRITE = 283,
	  TRUE = 284,
	  FALSE = 285,
	  ADD = 286,
	  MIN = 287,     d u
	  MUL = 288,     d
	  DIV = 289, d
	  GOTO = 290,
	  MOD = 291,
	  LES = 292, d
	  LEQ = 293, d
	  EQU = 294, d
	  NEQ = 295, d
	  GRE = 296, d
	  GEQ = 297, d
	  AND = 298, d
	  OR = 299, d
	  NOT = 300, u
	  CASE = 301,
	  FOR = 302,
	  FIN = 303,
	  IDENTICAL = 304,
	  FROM = 305,
	  BY = 306,
	  TO = 307,
	  NEW = 308,
	  INTCONST = 309,
	  IDE = 310,
	  REALCONST = 311,
	  STRING = 312,
	  DUMMY = 313*/

#ifndef AST_H 
#define AST_H

#include <iostream>
#include <assert.h>
#include <string>

using namespace std;

/**
 * classes
 */
 // base class for tree nodes
class Object {
public:
	virtual void print(ostream& os) = 0;
	virtual void pcodegen(ostream& os) = 0;
	virtual Object* clone() const { return NULL; }
	virtual ~Object() {}
};

class Expr : public Object {
public:
	// Unary operations
	Expr(int op, Object* atom) : op_(op), atom_(atom), left_(NULL), right_(NULL), unary_(true) {}
	// Binary operations
	Expr(int op, Object* left, Object* right) : op_(op), left_(left), right_(right), atom_(NULL), unary_(false) {}

	Expr(const Expr& exp) {
		unary_ = exp.unary_;
		op_ = exp.op_;
		left_ = exp.left_->clone();
		right_ = exp.right_->clone();
		atom_ = exp.atom_->clone();
	}

	virtual ~Expr() {
		if (left_) delete left_;
		if (right_) delete right_;
		if (atom_) delete atom_;
	}


	void print(ostream& os) {/*expr*/
		os << "Node name : Expr" << endl;
		assert(op_);
		if (unary_) {
			os << "Unary op is : " << op_;
			assert(atom_);
			atom_->print(os);
		}
		else {
			os << "Binary op is : " << op_;
			assert(left_ && right_);
			right_->print(os);
			left_->print(os);
		}
	}
	void pcodegen(ostream& os) {

		assert(op_);
		extern int took_a_number;
		extern int inc_dec;
		took_a_number = 0;
		inc_dec = 0;
		if (unary_) {
			assert(atom_);
			atom_->pcodegen(os);
			if (op_ == 286)
			{
				//os << "add" << endl;
			}
			else if (op_ == 287)
			{
				os << "neg" << endl;
			}
			else if (op_ == 300)
			{
				os << "not" << endl;
			}
		}
		else {
			assert(left_ && right_);
			right_->pcodegen(os);

			inc_dec = op_;
			left_->pcodegen(os);
			if (took_a_number == 1)
			{
				op_ = 0;
				took_a_number = 0;

			}
			//op_ = inc_dec;
			if (op_ == 286)
			{
				os << "add" << endl;
			}
			else if (op_ == 287)
			{
				os << "sub" << endl;
			}
			else if (op_ == 289)
			{
				os << "div" << endl;
			}
			else if (op_ == 288)
			{
				os << "mul" << endl;
			}

			else if (op_ == 292)
			{
				os << "les" << endl;
			}
			else if (op_ == 293)
			{
				os << "leq" << endl;
			}
			else if (op_ == 294)
			{
				os << "equ" << endl;
			}
			else if (op_ == 295)
			{
				os << "neq" << endl;
			}
			else if (op_ == 296)
			{
				os << "grt" << endl;
			}
			else if (op_ == 297)
			{
				os << "geq" << endl;
			}
			else if (op_ == 298)
			{
				os << "and" << endl;
			}
			else if (op_ == 299)
			{
				os << "or" << endl;
			}

		}
		took_a_number = 0;
		inc_dec = 0;

	}
	virtual Object* clone() const { return new Expr(*this); }

private:
	bool unary_;
	int op_;
	Object* left_;
	Object* right_;
	Object* atom_;
};

class ExprList : public Object {
public:
	ExprList(Object* expr) : expr_(expr), expr_list_(NULL) { assert(expr_); }
	ExprList(Object* expr, Object* expr_list) : expr_(expr), expr_list_(expr_list) { assert(expr_ && expr_list_); }

	ExprList(const ExprList& exp) {
		expr_ = exp.expr_->clone();
		expr_list_ = exp.expr_list_->clone();
	}

	virtual ~ExprList() {
		if (expr_) delete expr_;
		if (expr_list_) delete expr_list_;
	}

	void print(ostream& os) {
		os << "Node name : ExprList";
		assert(expr_);
		expr_->print(os);
		if (expr_list_) {
			expr_list_->print(os);
		}
	}
	void pcodegen(ostream& os) {/*exprlist*/
		assert(expr_);
		extern int current_parameter;
		extern int* by_ref_parameter;
		extern int ind_requered;
		extern int* parameter_for_function2;
		extern int* space;
		extern int this_is_a_function;
		extern string** types;
		int ccc = ind_requered;
		if (current_parameter != -1)
		{
			if (by_ref_parameter[current_parameter] == 1 || (space[current_parameter] > 1))
				ind_requered = 0;
			else
				ind_requered = 1;
		}
		int pre = this_is_a_function;
		this_is_a_function = 0;
		expr_->pcodegen(os);
		if (current_parameter != -1)
		{
			if (space[current_parameter] > 1 && this_is_a_function == 0)
			{
				os << "movs " << space[current_parameter] << endl;
			}
			current_parameter++;
		}
		this_is_a_function = pre;
		ind_requered = ccc;
		if (expr_list_) {
			expr_list_->pcodegen(os);
		}
	}
	virtual Object* clone() const { return new ExprList(*this); }

private:
	Object* expr_;
	Object* expr_list_;
};

class Dim : public Object {
public:
	Dim(Object* exp) : exp_(exp), dim_(NULL) { assert(exp_); }
	Dim(Object* exp, Object* dim) : exp_(exp), dim_(dim) { assert(exp_ && dim_); }

	Dim(const Dim& d) {
		exp_ = d.exp_->clone();
		dim_ = d.dim_->clone();
	}

	virtual ~Dim() {
		if (exp_) delete exp_;
		if (dim_) delete dim_;
	}

	void print(ostream& os) {
		os << "Node name : Dim" << endl;
		assert(exp_);
		exp_->print(os);
		if (dim_) {
			dim_->print(os);
		}
	}
	void pcodegen(ostream& os) {/*Dim*/
		assert(exp_);
		extern int ind_requered;
		int current_ind = ind_requered;
		ind_requered = 1;
		exp_->pcodegen(os);
		ind_requered = current_ind;
		extern int current_Dim;
		extern int current_size_of_type;
		extern string* current_Type_Name;
		extern int last_bond_order;
		int  i = 0;
		int ixa = 1;
		extern int current_sub_part;
		extern int* upper_bonds;
		extern int* lower_bonds;
		extern int* space;
		/*os << "current_Dim = " << current_Dim;
		os << " size_of_type = " << current_size_of_type;
		os << " current_Type_Name  =  " << *current_Type_Name;
		os << "last_bond = " << current_sub_part;*/
		for (i = current_Dim + 1; i <= last_bond_order; i++)
		{
			ixa = ixa * (upper_bonds[i] - lower_bonds[i] + 1);
		}
		os << "ixa " << ixa * current_size_of_type << endl;
		if (current_Dim == last_bond_order)
			os << "dec " << current_sub_part << endl;
		current_Dim++;

		if (dim_) {
			if (current_Dim == last_bond_order + 1)
			{

				extern string** names;
				extern int current_i;
				current_Dim = -1;

				for (i = 0; i < current_i; i++)
				{
					if (names[i] != NULL && (*current_Type_Name) == (*(names[i])))
					{
						current_Dim = i;
						int j = i;
						extern int* address;
						int current_add = address[j];

						int sub_part = 0;


						extern string** types;
						current_Type_Name = types[current_Dim];
						current_size_of_type = 1;
						int k = 0;
						for (k = 0; k < current_Dim; k++)
						{
							if (names[k] != NULL && (*current_Type_Name) == (*(names[k])))
							{
								current_size_of_type = space[k];
							}
						}
						while (address[j] == current_add)
						{
							int y = 1;
							int x = j + 1;
							while (address[x] == current_add)
							{

								y = y * (upper_bonds[x] - lower_bonds[x] + 1);
								x++;
							}
							sub_part += lower_bonds[j] * current_size_of_type * y;
							j++;
						}
						current_sub_part = sub_part;
						last_bond_order = j - 1;

					}

				}

			}
			dim_->pcodegen(os);
		}
	}
	virtual Object* clone() const { return new Dim(*this); }

private:
	Object* exp_;
	Object* dim_;
};

class Atom : public Object {
};

class IntConst : public Atom {
public:
	IntConst(const int i) : i_(i) {}
	IntConst(const IntConst& in) : i_(in.i_) {}

	void print(ostream& os) {
		os << "Node name : IntConst. Value is :" << i_ << endl;
	}
	void pcodegen(ostream& os)/*IntConst*/
	{
		extern int took_a_number;
		extern int inc_dec;
		if (inc_dec == 286)
		{
			os << "inc " << i_ << endl;
			inc_dec = 0;
			took_a_number = 1;

		}
		else if (inc_dec == 287)
		{
			os << "dec " << i_ << endl;
			inc_dec = 0;
			took_a_number = 1;


		}
		else
			os << "ldc " << this->i_ << endl;
	}
	virtual Object* clone() const { return new IntConst(*this); }

private:
	const int i_;
};

class RealConst : public Atom {
public:
	RealConst(const double r) : r_(r) {}
	RealConst(const RealConst& in) : r_(in.r_) {}

	void print(ostream& os) {
		os << "Node name : RealConst. Value is :" << r_ << endl;
	}
	void pcodegen(ostream& os) {/*RealConst*/
		extern int took_a_number;
		extern int inc_dec;
		if (inc_dec == 286)
		{
			os << "inc " << r_;
			if ((int)r_ == r_)
			{
				os << ".0";
			}
			os << endl;
			inc_dec = 0;
			took_a_number = 1;
		}
		else if (inc_dec == 287)
		{
			os << "dec " << r_;
			if ((int)r_ == r_)
			{
				os << ".0";
			}
			os << endl;
			inc_dec = 0;
			took_a_number = 1;
		}
		else
		{
			os << "ldc " << this->r_;
			if ((int)r_ == r_)
			{
				os << ".0";
			}
			os << endl;
		}



	}
	virtual Object* clone() const { return new RealConst(*this); }

private:
	const double r_;
};

class True : public Atom {
public:
	void print(ostream& os) {
		os << "Node name : trueConst. Value is true" << endl;
	}
	void pcodegen(ostream& os) {/*True*/
		os << "ldc 1" << endl;
	}
	virtual Object* clone() const { return new True(); }

};

class False : public Atom {
public:
	void print(ostream& os) {
		os << "Node name : trueConst. Value is false" << endl;
	}
	void pcodegen(ostream& os) {/*false*/
		os << "ldc 0" << endl;

	}
	virtual Object* clone() const { return new False(); }
};

class Var : public Atom {
};

class ArrayRef : public Var {
public:
	ArrayRef(Object* var, Object* dim) : var_(var), dim_(dim) { assert(var_ && dim_); }
	ArrayRef(const ArrayRef& arr) {
		var_ = arr.var_->clone();
		dim_ = arr.dim_->clone();
	}

	virtual ~ArrayRef() {
		if (var_) delete var_;
		if (dim_) delete dim_;
	}

	void print(ostream& os) {
		os << "Node name : ArrayRef" << endl;
		assert(var_ && dim_);
		var_->print(os);
		dim_->print(os);
	}
	void pcodegen(ostream& os) {/*ArrayRef*/
		assert(var_ && dim_);
		/*extern int current_Dim;ok
		extern int current_size_of_type;ok
		extern string* current_Type_Name;ok
		extern int last_bond_order;*/
		extern string* current_Type;  //htis is also the name of the array;
		extern string** names;
		extern int current_Dim;
		extern int current_i;
		extern int ind_requered;
		extern int current_size_of_type;
		extern string* current_Type_Name;
		extern int* space;
		extern int last_bond_order;
		extern int* upper_bonds;
		extern int* lower_bonds;
		extern int current_sub_part;
		int current_ind_rec = ind_requered;
		ind_requered = 0;
		var_->pcodegen(os);
		int  i = 0;
		int sub_part = 0;
		//string* current_name = current_Type;
		//os << "current_type "<< *current_Type<<endl;
		for (i = 0; i < current_i; i++)
		{
			if (names[i] != NULL && (*current_Type) == (*(names[i])))
			{
				current_Dim = i;
				int j = 0;
				extern string** types;
				current_Type_Name = types[i];
				current_size_of_type = 1;
				for (j = 0; j < i; j++)
				{
					if (names[j] != NULL && (*current_Type_Name) == (*(names[j])))
					{
						current_size_of_type = space[j];
					}
				}
				j = i;
				extern int* address;
				int current_add = address[j];
				while (address[j] == current_add)
				{

					int y = 1;
					int x = j + 1;
					while (address[x] == current_add)
					{

						y = y * (upper_bonds[x] - lower_bonds[x] + 1);
						x++;
					}
					sub_part += lower_bonds[j] * current_size_of_type * y;
					j++;
				}
				current_sub_part = sub_part;

				last_bond_order = j - 1;
			}

		}
		dim_->pcodegen(os);

		current_Dim = 0;
		int ccc = current_ind_rec;
		while (current_ind_rec >= 1)
		{
			os << "ind" << endl;
			current_ind_rec--;
		}
		if (ccc == 1)
			ind_requered = 1;

	}
	virtual Object* clone() const { return new ArrayRef(*this); }

private:
	Object* var_;
	Object* dim_;
};

class RecordRef : public Var {
public:
	RecordRef(Object* varExt, Object* varIn) : varExt_(varExt), varIn_(varIn) { assert(varExt_ && varIn_); }
	RecordRef(const RecordRef& rec) {
		varExt_ = rec.varExt_->clone();
		varIn_ = rec.varIn_->clone();
	}

	virtual ~RecordRef() {
		if (varExt_) delete varExt_;
		if (varIn_) delete varIn_;
	}

	void print(ostream& os) {
		os << "Node name : RecordRef" << endl;
		assert(varExt_ && varIn_);
		varExt_->print(os);
		//os << "do_var_in" << endl;
		varIn_->print(os);
	}
	void pcodegen(ostream& os) {/*Record ref*/
		assert(varExt_ && varIn_);
		extern int ind_requered;
		int current_ind_rec = ind_requered;

		ind_requered = 0;
		varExt_->pcodegen(os);
		extern int record_ref;
		extern int refs;

		refs = 0;
		record_ref = 1;
		varIn_->pcodegen(os);
		record_ref = 0;
		int ccc = current_ind_rec;
		while (current_ind_rec >= 1)
		{
			os << "ind" << endl;
			current_ind_rec--;
		}
		if (ccc == 1)
			ind_requered = 1;
	}
	virtual Object* clone() const { return new RecordRef(*this); }

private:
	Object* varExt_;
	Object* varIn_;
};

class AddressRef : public Var {
public:
	AddressRef(Object* var) : var_(var) { assert(var_); }
	AddressRef(const AddressRef& addre) {
		var_ = addre.var_->clone();
	}

	virtual ~AddressRef() {
		if (var_) delete var_;
	}

	void print(ostream& os) {
		os << "Node name : AddressRef" << endl;
		assert(var_);
		var_->print(os);
	}
	void pcodegen(ostream& os) {/*addressRef*/
		assert(var_);
		extern int refs_for_new;
		extern int ind_requered;
		int current_ind_rec = ind_requered;
		ind_requered = 0;

		extern int refs;
		refs++;
		var_->pcodegen(os);
		os << "ind" << endl;
		if (refs_for_new == 0)
		{
			refs = 0;
		}
		int ccc = current_ind_rec;
		while (current_ind_rec >= 1)
		{
			os << "ind" << endl;
			current_ind_rec--;
		}
		if (ccc == 1)
			ind_requered = 1;
	}
	virtual Object* clone() { return new AddressRef(*this); }

private:
	Object* var_;
};

class Statement : public Object {
};

class NewStatement : public Statement {
public:
	NewStatement(Object* var) : var_(var) { assert(var_); }
	NewStatement(const NewStatement& ns) {
		var_ = ns.var_->clone();
	}

	virtual ~NewStatement() {
		if (var_) delete var_;
	}

	void print(ostream& os) {
		os << "Node name : NewStatement";
		assert(var_);
		var_->print(os);
	}
	void pcodegen(ostream& os) {/*New statement*/
		assert(var_);
		extern int refs;
		extern int refs_for_new;
		extern string* current_Type_for_ref;
		extern string** names;
		extern string** types;
		extern int* space;
		extern int* how_many_refs;
		extern int current_i;
		extern int ind_requered;
		ind_requered = 0;
		refs_for_new = 1;
		refs = 0;
		var_->pcodegen(os);
		string* current_Type = current_Type_for_ref;

		int i = 0;
		for (i = 0; i < current_i; i++)
		{
			string* Type_of_ref;
			if (names[i] != NULL)
				if ((*current_Type) == *(names[i]))
				{

					Type_of_ref = types[i];
					if (refs == how_many_refs[i] - 1)
					{
						int j = 0;
						int space_c = 1;
						for (j = 0; j < i; j++)
						{
							if (names[j] != NULL)
								if ((*Type_of_ref) == *(names[j]))
								{
									space_c = space[j];
								}
						}
						os << "ldc " << space_c << endl;
					}
					else
					{
						os << "ldc " << 1 << endl;
					}
				}

		}
		os << "new" << endl;
		refs_for_new = 0;
		refs = 0;
		ind_requered = 1;
	}
	virtual Object* clone() { return new NewStatement(*this); }

private:
	Object* var_;
};

class WriteStrStatement : public Statement {
public:
	WriteStrStatement(const char* str) {
		str_ = new string(str);
	}

	WriteStrStatement(const WriteStrStatement& ns) {
		str_ = new string(*ns.str_);
	}

	virtual ~WriteStrStatement() {
		if (str_) delete str_;
	}
	void print(ostream& os) {
		os << "Node name : WriteStrStatement";
		assert(str_);
		os << "Str statement is: " << str_ << endl;
	}
	void pcodegen(ostream& os) {/*WriteStrStatement*/
		assert(str_);
		/*os << "ldc 5" << "\n";*/

	}
	virtual Object* clone() { return new WriteStrStatement(*this); }

private:
	string* str_;
};

class WriteVarStatement : public Statement {
public:
	WriteVarStatement(Object* exp) : exp_(exp) { assert(exp_); }

	WriteVarStatement(const WriteVarStatement& ex) {
		exp_ = ex.clone();
	}

	virtual ~WriteVarStatement() {
		if (exp_) delete exp_;
	}

	void print(ostream& os) {
		os << "Node name : WriteVarStatement";
		assert(exp_);
		exp_->print(os);
	}
	void pcodegen(ostream& os) {/*WriteVarStatement*/
		assert(exp_);
		exp_->pcodegen(os);
		os << "print" << endl;
	}
	virtual Object* clone() const { return new WriteVarStatement(*this); }

private:
	Object* exp_;
};

class ProcedureStatement : public Statement {
public:
	ProcedureStatement(const char* str) {
		str_ = new string(str);
	}

	ProcedureStatement(Object* expr_list, const char* str) : expr_list_(expr_list) {
		assert(expr_list_);
		str_ = new string(str);
	}

	ProcedureStatement(const ProcedureStatement& ps) {
		expr_list_ = ps.expr_list_->clone();
		str_ = new string(*ps.str_);
	}

	virtual ~ProcedureStatement() {
		if (str_) delete str_;
		if (expr_list_) delete expr_list_;
	}

	void print(ostream& os) {
		os << "Node name : ProcedureStatement. Proc name : " << *str_ << endl;
		if (expr_list_) {
			expr_list_->print(os);
		}
	}
	void pcodegen(ostream& os) {/*ProcedureStatement*/
		int mst = 0;
		extern int* function_static_link;
		extern string** function_name;
		extern int* parmeter_size;
		extern int current_dec;
		extern int current_f;
		extern int current_i;
		extern string** names;
		extern int current_parameter;
		extern int* function_lower_bond;
		int i = current_dec;
		int j = 0;
		int find = 0;
		int f_is_parameter = 0;
		extern int* address;
		extern int* function_parameter;
		extern int in_parameter_list;
		int current_for_parameter = current_parameter;
		//cout << "current_dec " << i << "functioncall " << *str_ << endl;
		for (j = 0; j < current_f; j++)
		{
			if (function_name[j] != NULL && *function_name[j] == *str_)
			{
				current_parameter = function_lower_bond[j] + 1;

				f_is_parameter = function_parameter[j];
				break;
			}
		}
		int current_function = j;
		int parameter_index = 0;
		int father_index = 0;
		if (f_is_parameter != 1)
		{

			while (i != -1 && i != function_static_link[j])
			{
				mst++;

				i = function_static_link[i];
			}
			os << "mst " << mst << endl;
		}
		else
		{
			extern int* parameter_for_function;
			j = parameter_for_function[j];
			while (i != -1 && i != j)
			{
				mst++;
				i = function_static_link[i];
			}
			for (int l = 0; l < current_i; l++)
			{
				if (names[l] != NULL && *names[l] == *str_)
				{
					parameter_index = l;
				}
				if (names[l] != NULL && *names[l] == *function_name[j])
				{
					father_index = l;
				}
			}
			os << "mstf " << mst << " " << address[parameter_index] - address[father_index] << endl;
		}
		int ccccccc = in_parameter_list;
		in_parameter_list = 1;
		if (expr_list_) {
			expr_list_->pcodegen(os);
		}
		in_parameter_list = ccccccc;
		if (f_is_parameter != 1)
			os << "cup " << parmeter_size[j] << " " << *str_ << endl;
		else
		{
			os << "smp " << parmeter_size[current_function] << endl;
			os << "cupi " << mst << " " << address[parameter_index] - address[father_index] << endl;
		}
		current_parameter = current_for_parameter;
	}
	virtual Object* clone() const { return new ProcedureStatement(*this); }

private:
	Object* expr_list_;
	string* str_;
};

class Case : public Object {
public:
	Case(Object* stat_list, int val) : leafChild_(NULL), stat_list_(stat_list) {
		// note the special treatment in miny.y for this case (makenode)
		leafChild_ = new IntConst(val);
		assert(stat_list_);
	}

	Case(const Case& c) {
		stat_list_ = c.stat_list_->clone();
		leafChild_ = c.leafChild_->clone();
	}

	virtual ~Case() {
		if (stat_list_) delete stat_list_;
		if (leafChild_) delete leafChild_;
	}

	void print(ostream& os) {
		os << "Node name : Case";
		assert(stat_list_);
		stat_list_->print(os);
	}
	void pcodegen(ostream& os) {/*case*/
		extern int switch_to_the_current_list;
		//extern int switch_counter;
		extern int* cases_counter;
		int current_switch = switch_to_the_current_list;
		assert(stat_list_);
		os << "case_" << cases_counter[current_switch - 1]++ << "_" << current_switch << ":" << endl;
		stat_list_->pcodegen(os);
		switch_to_the_current_list = current_switch;
		//	case_counter = current_case;
		os << "ujp end_switch_" << current_switch << endl;
	}
	virtual Object* clone() const { return new Case(*this); }

private:
	Object* stat_list_;
	Object* leafChild_;
};


class CaseList : public Object {
public:
	CaseList(Object* ccase) : case_(ccase), case_list_(NULL) { assert(case_); }
	CaseList(Object* ccase, Object* case_list) : case_(ccase), case_list_(case_list) { assert(case_ && case_list_); }

	CaseList(const CaseList& cl) {
		case_ = cl.case_->clone();
		case_list_ = cl.case_list_->clone();
	}

	virtual ~CaseList() {
		if (case_) delete case_;
		if (case_list_) delete case_list_;
	}

	void print(ostream& os) {
		os << "Node name : CaseList";
		assert(case_);
		case_->print(os);
		if (case_list_) {
			case_list_->print(os);
		}
	}
	void pcodegen(ostream& os) {/*caselist*/
		assert(case_);
		case_->pcodegen(os);
		if (case_list_) {
			case_list_->pcodegen(os);
		}
	}
	virtual Object* clone() const { return new CaseList(*this); }

private:
	Object* case_;
	Object* case_list_;
};

class CaseStatement : public Statement {
public:
	CaseStatement(Object* exp, Object* case_list) : exp_(exp), case_list_(case_list) { assert(exp_ && case_list_); }

	CaseStatement(const CaseStatement& cs) {
		exp_ = cs.exp_->clone();
		case_list_ = cs.case_list_->clone();
	}

	virtual ~CaseStatement() {
		if (exp_) delete exp_;
		if (case_list_) delete case_list_;
	}

	void print(ostream& os) {
		os << "Node name : CaseStatement";
		assert(exp_ && case_list_);
		exp_->print(os);
		case_list_->print(os);
	}
	void pcodegen(ostream& os) {/*CaseStatement*/
		int i;
		extern int switch_to_the_current_list;
		extern int case_max;
		extern int* cases_counter;
		extern int switch_counter;
		if (switch_counter == case_max)
		{
			int* new_cases = new int[2 * case_max];
			for (i = 0; i < case_max; i++)
			{
				new_cases[i] = cases_counter[i];
			}
			delete[]cases_counter;
			cases_counter = new_cases;
		}
		cases_counter[switch_counter] = 1;
		//switch_counter++;
		switch_counter++;
		int current = switch_counter;
		extern int all_cases;
		//extern int case_counter;
		assert(exp_ && case_list_);
		exp_->pcodegen(os);
		os << "neg" << endl;
		os << "ixj end_switch_" << current << endl;
		//case_counter = 1;
		switch_to_the_current_list = current;
		case_list_->pcodegen(os);
		for (i = cases_counter[current - 1] - 1; i >= 1; i--)
		{
			os << "ujp case_" << i << "_" << current << endl;
		}
		os << "end_switch_" << current << ":" << endl;
	}
	virtual Object* clone() const { return new CaseStatement(*this); }

private:
	Object* exp_;
	Object* case_list_;
};

class LoopStatement : public Statement {
public:
	LoopStatement(Object* exp, Object* stat_list) : exp_(exp), stat_list_(stat_list) { assert(exp_ && stat_list_); }

	LoopStatement(const LoopStatement& ls) {
		exp_ = ls.exp_->clone();
		stat_list_ = ls.stat_list_->clone();
	}

	virtual ~LoopStatement() {
		if (exp_) delete exp_;
		if (stat_list_) delete stat_list_;
	}

	void print(ostream& os) {
		os << "Node name : LoopStatement";
		assert(exp_ && stat_list_);
		exp_->print(os);
		stat_list_->print(os);
	}
	void pcodegen(ostream& os) {/*LoopStatement*/
		extern int loop_counter;
		int current = loop_counter;
		loop_counter++;
		assert(exp_ && stat_list_);
		os << "loop_" << current << ":" << endl;
		exp_->pcodegen(os);
		os << "fjp end_loop_" << current << endl;
		stat_list_->pcodegen(os);
		os << "ujp loop_" << current << endl;
		os << "end_loop_" << current << ":" << endl;

	}
	virtual Object* clone() const { return new LoopStatement(*this); }

private:
	Object* exp_;
	Object* stat_list_;
};


class ConditionalStatement : public Statement {
public:
	ConditionalStatement(Object* exp, Object* stat_list_if) : exp_(exp), stat_list_if_(stat_list_if), stat_list_else_(NULL) { assert(exp_ && stat_list_if_); }
	ConditionalStatement(Object* exp, Object* stat_list_if, Object* stat_list_else) : exp_(exp), stat_list_if_(stat_list_if), stat_list_else_(stat_list_else) { assert(exp_ && stat_list_if_ && stat_list_else_); }

	ConditionalStatement(const ConditionalStatement& cs) {
		exp_ = cs.exp_->clone();
		stat_list_if_ = cs.stat_list_if_->clone();
		stat_list_else_ = cs.stat_list_else_->clone();
	}

	virtual ~ConditionalStatement() {
		if (exp_) delete exp_;
		if (stat_list_if_) delete stat_list_if_;
		if (stat_list_else_) delete stat_list_else_;
	}

	void print(ostream& os) {
		os << "Node name : ConditionalStatement";
		assert(exp_ && stat_list_if_);
		exp_->print(os);
		stat_list_if_->print(os);
		if (stat_list_else_) {
			stat_list_else_->print(os);
		}
	}
	void pcodegen(ostream& os) {/*Conditional*/
		extern int if_counter;
		int current = if_counter;
		if_counter++;


		assert(exp_ && stat_list_if_);
		exp_->pcodegen(os);
		if (stat_list_else_ == NULL)
		{
			os << "fjp " << "end_if_" << current << endl;
		}
		else
		{
			os << "fjp " << "else_if_" << current << endl;

		}
		stat_list_if_->pcodegen(os);
		if (stat_list_else_ == NULL)
		{
			os << "end_if_" << current << ":" << endl;
		}
		else
		{
			os << "ujp " << "end_if_" << current << endl;
			os << "else_if_" << current << ":" << endl;
		}
		if (stat_list_else_) {
			stat_list_else_->pcodegen(os);
		}
		if (stat_list_else_ != NULL)
		{
			os << "end_if_" << current << ":" << endl;

		}

	}
	virtual Object* clone() const { return new ConditionalStatement(*this); }

private:
	Object* exp_;
	Object* stat_list_if_;
	Object* stat_list_else_;
};


class Assign : public Statement {
public:
	Assign(Object* var, Object* exp) : var_(var), exp_(exp) { assert(var_ && exp_); }

	Assign(const Assign& as) {
		var_ = as.var_->clone();
		exp_ = as.exp_->clone();
	}

	virtual ~Assign() {
		if (exp_) delete exp_;
		if (var_) delete var_;
	}

	void print(ostream& os) {
		os << "Node name : Assign";
		assert(var_ && exp_);
		exp_->print(os);
		var_->print(os);
	}
	void pcodegen(ostream& os) {/*assigen*/
		extern int ind_requered;
		extern int from_left_assigen;
		assert(var_ && exp_);
		int cc = from_left_assigen;
		from_left_assigen = 1;
		ind_requered = 0;
		exp_->pcodegen(os);
		from_left_assigen = cc;
		ind_requered = 1;
		var_->pcodegen(os);

		os << "sto" << endl;
	}
	virtual Object* clone() const { return new Assign(*this); }

private:
	Object* var_;
	Object* exp_;
};

class StatementList : public Object {
public:
	StatementList(Object* stat) : stat_list_(NULL), stat_(stat) { assert(stat_); }
	StatementList(Object* stat_list, Object* stat) : stat_list_(stat_list), stat_(stat) { assert(stat_list_ && stat); }

	StatementList(const StatementList& sl) {
		stat_ = sl.stat_->clone();
		stat_list_ = sl.stat_list_->clone();
	}

	virtual ~StatementList() {
		if (stat_) delete stat_;
		if (stat_list_) delete stat_list_;
	}

	void print(ostream& os) {
		os << "Node name : StatementList" << endl;
		if (stat_list_) {
			stat_list_->print(os);
		}
		assert(stat_);
		stat_->print(os);
	}
	void pcodegen(ostream& os) {/*StatementList*/
		if (stat_list_) {
			stat_list_->pcodegen(os);
		}
		assert(stat_);
		stat_->pcodegen(os);
	}
	virtual Object* clone() const { return new StatementList(*this); }

private:
	Object* stat_;
	Object* stat_list_;
};

class RecordList : public Object {
public:
	RecordList(Object* var_decl) : record_list_(NULL), var_decl_(var_decl) { assert(var_decl_); }
	RecordList(Object* record_list, Object* var_decl) : record_list_(record_list), var_decl_(var_decl) { assert(record_list_ && var_decl); }

	RecordList(const RecordList& li) {
		var_decl_ = li.var_decl_->clone();
		record_list_ = li.record_list_->clone();
	}

	virtual ~RecordList() {
		if (var_decl_) delete var_decl_;
		if (record_list_) delete record_list_;
	}

	void print(ostream& os) {
		os << "Node name : RecordList" << endl;
		if (record_list_) {
			record_list_->print(os);
		}
		assert(var_decl_);
		var_decl_->print(os);
	}
	void pcodegen(ostream& os) {/*RecordList*/
		if (record_list_) {
			record_list_->pcodegen(os);
		}
		assert(var_decl_);
		var_decl_->pcodegen(os);
	}
	virtual Object* clone() const { return new RecordList(*this); }

private:
	Object* var_decl_;
	Object* record_list_;
};

class Type : public Object {
};

class SimpleType : public Type {/*doesnt print any anything*/
public:
	SimpleType(const char* name) {
		name_ = new string(name);
	}

	virtual ~SimpleType() {
		if (name_)delete name_;
	}

	SimpleType(const SimpleType& s) {
		name_ = new string(*s.name_);
	}

	void print(ostream& os) {
		os << "Node name : SimpleType" << endl;
	}
	void pcodegen(ostream& os)/*SimpleType doesnt print anything*/
	{
		extern int current_i;
		extern string** types;
		extern int* address;
		extern int* space;
		extern string* current_Type;
		extern int first_rec_var;
		current_Type = name_;
		if (current_i != 0)
		{
			if (first_rec_var == 1)
			{
				address[current_i] = address[current_i - 1];
				first_rec_var = 0;
			}
			else
				address[current_i] = address[current_i - 1] + space[current_i - 1];
		}
		else
		{
			address[0] = 5;
		}
		space[current_i] = 1;
		extern int size_of_rec;
		size_of_rec++;
		types[current_i] = name_;

	}
	virtual Object* clone() const { return new SimpleType(*this); }

private:
	string* name_;
};

class IdeType : public Type {
public:
	IdeType(const char* name) {
		name_ = new string(name);
	}

	virtual ~IdeType() {
		if (name_) delete name_;
	}

	IdeType(const IdeType& s) {
		name_ = new string(*s.name_);
	}

	void print(ostream& os) {
		os << "Node name : IdeType name = " << *name_ << endl;
	}
	void pcodegen(ostream& os)/*IdeType*/
	{
		extern int current_parameter;
		extern int ind_requered;
		extern int current_i;
		extern string** names;
		extern int* address;
		int i;
		extern string* current_Type_for_ref;
		extern string* current_Type;
		extern int for_def;
		current_Type_for_ref = name_;
		current_Type = name_;
		extern  int record_ref;
		extern int* address;
		extern string** types;
		extern int* space;
		extern int remember_Type;
		extern int this_is_a_function;
		if (record_ref == 1)
		{
			/*for (i = 0; i < current_i; i++)
			{
				if (names[i]!=NULL &&(*name_) == *(names[i]))
				{
					break;
				}
			}*/
			i = 0;

			while (names[i] == NULL || (*name_) != *(names[i]))
			{
				i++;
			}
			int current_i_up = i;
			//extern string** types; 
			while (types[current_i_up] == NULL || (*(types[current_i_up])) != "RECORD")
			{
				current_i_up--;
			}
			os << "inc " << address[i] - address[current_i_up] << endl;
			//record_ref = 0;
		}
		else
		{
			int x;
			int j = 0;
			int static_link_length = 0;
			int new_address = 0;
			for (i = 0; i < current_i; i++)
			{
				if (names[i] != NULL)
					if ((*name_) == *(names[i]))
					{
						if (for_def == 0)
						{
							extern int current_f;
							extern int from_left_assigen;
							extern int* function_lower_bond;
							extern string** function_name;
							extern int* function_upper_bond;
							extern int* function_static_link;
							extern int current_dec;
							extern int* function_parameter;
							int real_j = 0;
							extern int in_parameter_list;
							int min = 2147483647;
							if (from_left_assigen != 1)
							{
								for (j = 0; j <= current_f; j++)
								{
									if (i >= function_lower_bond[j] + 1 && i <= function_upper_bond[j])
									{
										if (min >= function_upper_bond[j] - function_lower_bond[j] - 1)
										{
											real_j = j;
											min = function_upper_bond[j] - function_lower_bond[j] - 1;
										}
									}
								}
							}
							else
							{
								for (j = 0; j <= current_f; j++)
								{
									if (i >= function_lower_bond[j] && i <= function_upper_bond[j])
									{
										if (min >= function_upper_bond[j] - function_lower_bond[j])
										{
											real_j = j;
											min = function_upper_bond[j] - function_lower_bond[j];
										}
									}
								}
							}
							j = real_j;
							new_address = address[i] - address[function_lower_bond[j]];
							int cur = current_dec;
							//	os << "current_dec = " << current_dec << " call for " << *name_ << endl;
							while (cur != -1 && cur != j)
							{
								cur = function_static_link[cur];
								static_link_length++;
							}

							int function_index = -1;
							for (int m = 0; m < current_f; m++)
							{
								if (function_name[m] != NULL && *function_name[m] == *name_)
								{
									function_index = m;
									break;
								}
							}
							if (in_parameter_list == 0 || function_index == -1)
								os << "lda " << static_link_length << " " << new_address << endl;
							else if (function_parameter[function_index] == 1)
							{
								os << "lda " << static_link_length << " " << new_address << endl;
							}
							else
							{
								os << "ldc " << *name_ << endl;
								os << "lda " << static_link_length << " 0" << endl;
								this_is_a_function = 1;
							}
							extern int* by_ref_parameter;
							if (by_ref_parameter[i] == 1)
							{
								os << "ind" << endl;
							}
							int current_ind = ind_requered;
							while (function_index == -1 && ind_requered >= 1)
							{
								os << "ind" << endl;
								ind_requered--;
							}
							if (current_ind == 1)
								ind_requered = 1;
							return;

						}
						else if ((address[current_i] == x && remember_Type == 1) || (address[current_i] == 0 && remember_Type == 1))
						{
							extern int first_rec_var;
							extern int size_of_rec;
							size_of_rec += space[i];
							if (first_rec_var == 1)
							{
								address[current_i] = address[current_i - 1];
								first_rec_var = 0;
							}
							else
							{
								address[current_i] = address[current_i - 1] + space[current_i - 1];
							}
							space[current_i] = space[i];
							types[current_i] = name_;
						}
					}
			}
		}
	}
	virtual Object* clone() const { return new IdeType(*this); }

private:
	string* name_;
};

class ArrayType : public Type {
public:
	ArrayType(int l, int u, Object* type) : low_(l), up_(u), type_(type) { assert(type_); }

	ArrayType(const ArrayType& a) : low_(a.low_), up_(a.up_) {
		type_ = a.type_->clone();
	}

	virtual ~ArrayType() {
		if (type_) delete type_;
	}

	void print(ostream& os) {
		os << "Node name : ArrayType: low bound is: " << low_ << ", up bound is: " << up_ << endl;
		assert(type_);
		type_->print(os);
	}
	void pcodegen(ostream& os) {/*ArrayType*/
		assert(type_);
		extern int temp;
		extern int remember;
		int current_remember = remember;
		extern int* space;
		extern string** types;
		extern int current_i;
		extern int* address;
		int current_bond = current_i;
		extern int* upper_bonds;
		extern int* lower_bonds;
		extern int remember_Type;
		int current_remeber_Type = remember_Type;
		remember_Type = 0;
		remember = 0;
		upper_bonds[current_i] = up_;
		lower_bonds[current_i] = low_;
		space[current_i] = 0;
		address[current_i] = -1;
		types[current_i] = NULL;
		current_i++;
		temp++;
		if (current_remember == 1)
		{
			if (current_bond != 0)
			{
				extern int first_rec_var;
				if (first_rec_var == 1)
				{
					address[current_bond] = address[current_bond - 1];
					first_rec_var = 0;
				}
				else
					address[current_bond] = address[current_bond - 1] + space[current_bond - 1];
			}
			else
			{
				address[0] = 5;
			}
		}
		type_->pcodegen(os);
		if (current_remember == 1)
		{
			current_i--;
			temp = 0;
			extern string* current_Type;
			extern string** names;

			int size_of_Type = 1;
			int i;
			types[current_bond] = current_Type;

			for (i = 0; i < current_bond; i++)
			{
				if (names[i] != NULL && (*current_Type) == (*(names[i])))
				{
					size_of_Type = space[i];
				}
			}
			for (i = current_bond; i <= current_i; i++)
			{

				size_of_Type = size_of_Type * (upper_bonds[i] - lower_bonds[i] + 1);

			}
			space[current_bond] = size_of_Type;
			extern int size_of_rec;
			size_of_rec += size_of_Type;
			for (i = current_bond + 1; i <= current_i; i++)
			{

				address[i] = address[current_bond];
				space[i] = space[current_bond];

			}
			remember = 1;
			remember_Type = current_remeber_Type;
			//current_i++;
		}
	}
	virtual Object* clone() const { return new ArrayType(*this); }

private:
	Object* type_;
	int low_, up_;
};

class RecordType : public Type {
public:
	RecordType(Object* record_list) : record_list_(record_list) { assert(record_list_); }

	RecordType(const RecordType& y) {
		record_list_ = y.record_list_->clone();
	}

	virtual ~RecordType() {
		if (record_list_) delete record_list_;
	}

	void print(ostream& os) {
		os << "Node name : RecordType" << endl;
		assert(record_list_);
		record_list_->print(os);
	}
	void pcodegen(ostream& os) {/*RecordType*/
		assert(record_list_);
		extern int* address;
		extern int* space;
		extern int current_i;
		extern string** types;
		extern int first_rec_var;
		extern int size_of_rec;
		first_rec_var = 1;
		size_of_rec = 0;
		types[current_i] = new string("RECORD");
		int current_place = current_i;
		if (current_i == 0)
		{
			address[0] = 5;
		}
		else
		{
			address[current_place] = address[current_place - 1] + space[current_place - 1];
		}
		current_i++;
		record_list_->pcodegen(os);
		current_i--;

		space[current_place] = address[current_i] - address[current_place] + space[current_i];
	}
	virtual Object* clone() const { return new RecordType(*this); }

private:
	Object* record_list_;
};


class AddressType : public Type {
public:
	AddressType(Object* type) : type_(type) { assert(type_); }

	AddressType(const AddressType& t) {
		type_ = t.type_->clone();
	}

	virtual ~AddressType() {
		if (type_) delete type_;
	}

	void print(ostream& os) {
		os << "Node name : AddressType" << endl;
		assert(type_);
		type_->print(os);
	}
	void pcodegen(ostream& os) {/*AddressType*/
		assert(type_);
		extern string* current_Type;
		extern int current_i;
		extern string** types;
		extern int* how_many_refs;
		extern int* address;
		extern int* space;
		extern int size_of_rec;//heheheh
		extern int first_rec_var;
		extern int ref_number;
		extern int remember_Type;
		int current_remember_Type = remember_Type;
		remember_Type = 0;
		int current_ref_number = ref_number;
		ref_number++;
		if (current_ref_number == 0)
		{
			if (first_rec_var == 1)
			{
				address[current_i] = address[current_i - 1];
				first_rec_var = 0;
			}
			else
			{
				if (current_i == 0)
				{
					address[0] = 5;
				}
				else
				{
					address[current_i] = address[current_i - 1] + space[current_i - 1];
				}
			}
		}
		type_->pcodegen(os);
		if (current_ref_number == 0)
		{


			size_of_rec++;
			types[current_i] = current_Type;
			how_many_refs[current_i] = ref_number;
			space[current_i] = 1;
			remember_Type = current_remember_Type;
			ref_number = 0;
		}
	}
	virtual Object* clone() const { return new AddressType(*this); }

private:
	Object* type_;
};


class Declaration : public Object {
};

class VariableDeclaration : public Declaration {
public:
	VariableDeclaration(Object* type, const char* str) : type_(type) {
		assert(type_);
		name_ = new string(str);
	}

	VariableDeclaration(const VariableDeclaration& p) {
		type_ = p.type_->clone();
		name_ = new string(*p.name_);
	}

	virtual ~VariableDeclaration() {
		if (type_) delete type_;
		delete name_;
	}

	void print(ostream& os) {
		os << "Node name : VariableDeclaration. Var name is: " << *name_ << endl;
		assert(type_);
		type_->print(os);
	}
	void pcodegen(ostream& os) {/*VariableDeclaration doesnt print anything*/
		int i;
		assert(type_);
		extern int current_i;
		extern string** names;

		names[current_i] = name_;
		extern int declare_var;
		declare_var = 1;
		type_->pcodegen(os);
		declare_var = 0;
		extern  int current_max;
		extern string** types;
		extern int* address;
		extern int* space;
		if (current_i + 100 >= current_max)
		{
			int* new_address = new int[2 * current_max];
			string** new_names = new string * [2 * current_max];
			string** new_types = new string * [2 * current_max];
			int* new_space = new int[2 * current_max];
			for (i = 0; i < current_max; i++)
			{
				new_address[i] = address[i];
				new_types[i] = types[i];
				new_names[i] = names[i];
				new_space[i] = space[i];
			}
			delete[]address;
			delete[]names;
			delete[]types;
			delete[] space;
			address = new_address;
			types = new_types;
			names = new_names;
			space = new_space;
			current_max *= 2;
		}
		current_i++;
	}
	virtual Object* clone() const { return new VariableDeclaration(*this); }

private:
	Object* type_;
	string* name_;
};

class Parameter : public Object {
public:
	Parameter(Object* type, const char* name) : type_(type) {
		assert(type_);
		name_ = new string(name);
	}

	Parameter(const Parameter& p) {
		type_ = p.type_->clone();
		name_ = new string(*p.name_);
	}

	virtual ~Parameter() {
		if (type_) delete type_;
		delete name_;
	}

	void print(ostream& os) {
		printWayOfPassing(os);
		os << "Parameter name :" << name_ << endl;
		assert(type_);
		type_->print(os);
	}
	void pcodegen(ostream& os) {/*Parameter*/
		//printWayOfPassing(os);
		int i;
		assert(type_);
		extern int current_i;
		extern string** names;
		extern int* space;
		extern int* by_ref_parameter;
		extern int current_f;
		extern string** function_name;
		by_ref_parameter[current_i] = by_ref();
		names[current_i] = name_;
		extern int declare_var;
		declare_var = 1;
		type_->pcodegen(os);
		extern int* function_parameter;
		extern string* current_Type;
		extern int* function_lower_bond;
		extern int* parmeter_size;
		extern int* parameter_for_function;
		extern int* parameter_for_function2;
		extern int current_dec;
		for (i = 0; i < current_f; i++)
		{
			if (*current_Type == *function_name[i])
			{
				parameter_for_function2[current_i] = 1;
				//by_ref_parameter[current_i] = 1;
				function_parameter[current_f] = 1;
				function_name[current_f] = name_;
				parmeter_size[current_f] = parmeter_size[i];
				function_lower_bond[current_f] = function_lower_bond[i];
				parameter_for_function[current_f] = current_dec;
				space[current_i] = 2;
				current_f++;
			}

		}
		declare_var = 0;
		extern  int current_max;
		extern string** types;
		extern int* address;
		if (current_i + 100 >= current_max)
		{
			int* new_address = new int[2 * current_max];
			string** new_names = new string * [2 * current_max];
			string** new_types = new string * [2 * current_max];
			int* new_space = new int[2 * current_max];
			for (i = 0; i < current_max; i++)
			{
				new_address[i] = address[i];
				new_types[i] = types[i];
				new_names[i] = names[i];
				new_space[i] = space[i];
			}
			delete[]address;
			delete[]names;
			delete[]types;
			delete[] space;
			address = new_address;
			types = new_types;
			names = new_names;
			space = new_space;
			current_max *= 2;
		}
		current_i++;
	}
protected:
	virtual int by_ref() = 0;
	virtual void printWayOfPassing(ostream& os) const = 0;

private:
	Object* type_;
	string* name_;
};

class ByReferenceParameter : public Parameter {
public:
	ByReferenceParameter(Object* type, const char* name) : Parameter(type, name) {}
	virtual Object* clone() const { return new ByReferenceParameter(*this); }
protected:
	void printWayOfPassing(ostream& os) const {
		os << "Node name : ByReferenceParameter ";
	}
	int by_ref()
	{
		return 1;
	}
};

class ByValueParameter : public Parameter {
public:
	ByValueParameter(Object* type, const char* name) : Parameter(type, name) {}
	virtual Object* clone() const { return new ByValueParameter(*this); }
protected:
	void printWayOfPassing(ostream& os) const {
		os << "Node name : ByValueParameter ";
	}
	int by_ref()
	{
		return 0;
	}
};

class ParameterList : public Object {
public:
	ParameterList(Object* formal) : formal_(formal), formal_list_(NULL) { assert(formal_); }
	ParameterList(Object* formal, Object* formal_list) : formal_(formal), formal_list_(formal_list) { assert(formal_ && formal_list_); }

	ParameterList(const ParameterList& pl) {
		formal_ = pl.formal_->clone();
		formal_list_ = pl.formal_list_->clone();
	}

	virtual ~ParameterList() {
		if (formal_) delete formal_;
		if (formal_list_) delete formal_list_;
	}

	void print(ostream& os) {
		os << "Node name : ParameterList" << endl;
		assert(formal_);
		formal_->print(os);
		if (formal_list_) {
			formal_list_->print(os);
		}

	}
	void pcodegen(ostream& os) {/*ParameterList*/
		assert(formal_);
		formal_->pcodegen(os);
		if (formal_list_) {
			formal_list_->pcodegen(os);
		}

	}
	virtual Object* clone() const { return new ParameterList(*this); }

private:
	Object* formal_;
	Object* formal_list_;
};

class FunctionDeclaration : public Declaration {
public:
	FunctionDeclaration(Object* type, Object* block, const char* name) : type_(type), block_(block), formal_list_(NULL) {
		assert(type_ && block_);
		name_ = new string(name);
	}

	FunctionDeclaration(Object* type, Object* formal_list, Object* block, const char* name) : type_(type), formal_list_(formal_list), block_(block) {
		assert(type_ && formal_list_ && block_);
		name_ = new string(name);
	}

	virtual ~FunctionDeclaration() {
		if (type_) delete type_;
		if (block_) delete block_;
		if (formal_list_) delete formal_list_;
		if (name_) delete name_;
	}

	FunctionDeclaration(const FunctionDeclaration& fd) {
		type_ = fd.type_->clone();
		block_ = fd.block_->clone();
		formal_list_ = fd.formal_list_->clone();
		name_ = new string(*fd.name_);
	}

	void print(ostream& os) {
		os << "Node name : FunctionDeclaration. Func name is: " << *name_ << endl;
		assert(type_ && block_);
		type_->print(os);
		if (formal_list_) {
			formal_list_->print(os);
		}
		block_->print(os);
	}
	void pcodegen(ostream& os) {/*FunctionDeclaration*/
		assert(type_ && block_);
		type_->pcodegen(os);
		extern int* space;
		extern string** names;
		extern string** types;
		extern int* address;
		extern int current_f;
		extern int current_i;
		extern int current_dec;
		extern int* function_lower_bond;
		extern int* wrote_ssp;
		extern string** function_name;
		extern int* function_upper_bond;
		extern int* space;
		extern int* address;
		extern int* function_static_link;
		names[current_i] = name_;
		space[current_i] = 5;
		address[current_i] = address[current_i - 1] + space[current_i - 1];
		types[current_i] = new string("FUNCTION");
		current_i++;
		function_upper_bond[current_dec] = current_i - 1;
		int ccd = current_dec;
		if (wrote_ssp[current_dec] == 0)
		{

			wrote_ssp[current_dec] = 1;
			os << *function_name[current_dec] << ":" << endl;
			if (current_i != function_lower_bond[current_dec])
				os << "ssp " << address[current_i - 2] + space[current_i - 2] - address[function_lower_bond[current_dec]] << endl; // dont know
			else
				os << "ssp 5" << endl;
			os << "sep 50" << endl;
			os << "ujp " << *function_name[current_dec] << "_begin" << endl;
		}

		function_static_link[current_f] = current_dec;
		function_name[current_f] = name_;
		function_lower_bond[current_f] = current_i - 1;
		wrote_ssp[current_f] = 0;
		current_dec = current_f;
		current_f++;
		if (formal_list_) {
			formal_list_->pcodegen(os);
		}
		extern int* parmeter_size;
		if (current_i != function_lower_bond[current_dec])
			parmeter_size[current_dec] = address[current_i - 1] + space[current_i - 1] - address[function_lower_bond[current_dec] + 1]; // dont know
		else
			parmeter_size[current_dec] = 0;
		extern int for_def;
		for_def = 0;
		block_->pcodegen(os);
		current_dec = ccd;
		for_def = 1;
		os << "retf" << endl;
	}
	virtual Object* clone() const { return new FunctionDeclaration(*this); }

private:
	Object* type_;
	Object* block_;
	Object* formal_list_;
	string* name_;
};

class ProcedureDeclaration : public Declaration {
public:
	ProcedureDeclaration(Object* block, const char* name) : formal_list_(NULL), block_(block) {
		assert(block_);
		name_ = new string(name);
	}

	ProcedureDeclaration(Object* formal_list, Object* block, const char* name) : formal_list_(formal_list), block_(block) {
		assert(formal_list_ && block_);
		name_ = new string(name);
	}

	virtual ~ProcedureDeclaration() {
		if (block_) delete block_;
		if (formal_list_) delete formal_list_;
		if (name_) delete name_;
	}

	ProcedureDeclaration(const ProcedureDeclaration& pd) {
		block_ = pd.block_->clone();
		formal_list_ = pd.formal_list_->clone();
		name_ = new string(*pd.name_);
	}

	void print(ostream& os) {
		os << "Node name : ProcedureDeclaration. Proc name is: " << *name_ << endl;
		assert(block_);
		if (formal_list_) {
			formal_list_->print(os);
		}
		block_->print(os);
	}
	void pcodegen(ostream& os) {/*ProcedureDeclaration*/
		assert(block_);
		extern int* space;
		extern string** names;
		extern string** types;
		extern int* address;
		extern int current_f;
		extern int current_i;
		extern int current_dec;
		extern int* function_lower_bond;
		extern int* wrote_ssp;
		extern string** function_name;
		extern int* function_upper_bond;
		extern int* space;
		extern int* address;
		extern int* function_static_link;
		names[current_i] = name_;
		space[current_i] = 5;
		address[current_i] = address[current_i - 1] + space[current_i - 1];
		types[current_i] = new string("PROCEDURE");
		function_upper_bond[current_dec] = current_i;
		int ccd = current_dec;
		if (wrote_ssp[current_dec] == 0)
		{

			wrote_ssp[current_dec] = 1;
			os << *function_name[current_dec] << ":" << endl;
			if (current_i != function_lower_bond[current_dec])
				os << "ssp " << address[current_i - 1] + space[current_i - 1] - address[function_lower_bond[current_dec]] << endl; // dont know
			else
				os << "ssp 5" << endl;
			os << "sep 50" << endl;
			os << "ujp " << *function_name[current_dec] << "_begin" << endl;
		}
		current_i++;
		function_static_link[current_f] = current_dec;
		function_name[current_f] = name_;
		function_lower_bond[current_f] = current_i - 1;
		wrote_ssp[current_f] = 0;
		current_dec = current_f;
		current_f++;
		if (formal_list_) {
			formal_list_->pcodegen(os);
		}
		extern int* parmeter_size;
		if (current_i != function_lower_bond[current_dec])
			parmeter_size[current_dec] = address[current_i - 1] + space[current_i - 1] - address[function_lower_bond[current_dec] + 1]; // dont know
		else
			parmeter_size[current_dec] = 0;
		extern int for_def;
		for_def = 0;
		block_->pcodegen(os);
		for_def = 1;
		current_dec = ccd;
		os << "retp" << endl;
	}
	virtual Object* clone() const { return new ProcedureDeclaration(*this); }

private:
	Object* block_;
	Object* formal_list_;
	string* name_;
};

class DeclarationList : public Object {
public:
	DeclarationList(Object* decl) : decl_(decl), decl_list_(NULL) { assert(decl_); }
	DeclarationList(Object* decl_list, Object* decl) : decl_list_(decl_list), decl_(decl) { assert(decl_list_ && decl); }

	DeclarationList(const DeclarationList& dl) {
		decl_ = dl.decl_->clone();
		decl_list_ = dl.decl_list_->clone();
	}

	virtual ~DeclarationList() {
		if (decl_) delete decl_;
		if (decl_list_) delete decl_list_;
	}

	void print(ostream& os) {
		os << "Node name : DeclarationList" << endl;
		if (decl_list_) {
			decl_list_->print(os);
		}
		assert(decl_);
		decl_->print(os);
	}
	void pcodegen(ostream& os) {/*DeclarationList*/
		if (decl_list_) {
			decl_list_->pcodegen(os);
		}
		assert(decl_);
		extern int for_def;
		for_def = 1;
		decl_->pcodegen(os);
		for_def = 0;
	}
	virtual Object* clone() const { return new DeclarationList(*this); }

private:
	Object* decl_;
	Object* decl_list_;
};

class Block : public Object {
public:
	Block(Object* stat_seq) : stat_seq_(stat_seq), decl_list_(NULL) { assert(stat_seq_); }
	Block(Object* decl_list, Object* stat_seq) : decl_list_(decl_list), stat_seq_(stat_seq) { assert(decl_list_ && stat_seq_); }

	Block(const Block& b) {
		decl_list_ = b.decl_list_->clone();
		stat_seq_ = b.stat_seq_->clone();
	}

	virtual ~Block() {
		if (stat_seq_) delete stat_seq_;
		if (decl_list_) delete decl_list_;
	}

	void print(ostream& os) {
		os << "Node name : Begin" << endl;


		if (decl_list_) {
			decl_list_->print(os);
		}

		assert(stat_seq_);
		stat_seq_->print(os);


	}
	void pcodegen(ostream& os) {/*Begin*///not finshed yet
		extern int current_f;
		extern int current_i;
		extern int current_dec;
		extern int* function_lower_bond;
		extern int* wrote_ssp;
		extern string** function_name;
		extern int* function_upper_bond;
		extern int* space;
		extern int* address;
		extern int* function_static_link;
		int ccc = current_dec;
		if (decl_list_) {
			decl_list_->pcodegen(os);
		}
		function_upper_bond[current_dec] = current_i - 1;
		if (wrote_ssp[current_dec] == 0)
		{
			wrote_ssp[current_dec] = 1;
			os << *function_name[current_dec] << ":" << endl;
			if (current_i != function_lower_bond[current_dec])
				os << "ssp " << address[current_i - 1] + space[current_i - 1] - address[function_lower_bond[current_dec]] << endl; // dont know
			else
				os << "ssp 5" << endl;
			os << "sep 50" << endl;
			os << "ujp " << *function_name[current_dec] << "_begin" << endl;
		}
		os << *function_name[ccc] << "_begin:" << endl;
		extern int ind_requered;
		assert(stat_seq_);
		int ccccccc = ind_requered;
		ind_requered = 1;
		stat_seq_->pcodegen(os);
		ind_requered = ccccccc;

		//current_dec = function_static_link[current_dec];
	}

	virtual Object* clone() const { return new Block(*this); }

private:
	Object* decl_list_;
	Object* stat_seq_;
};

class Program : public Object {
public:
	Program(Object* block, const char* str) : block_(NULL) {
		block_ = dynamic_cast<Block*>(block);
		assert(block_);
		name_ = new string(str);
	}

	Program(const Program& prog) {
		block_ = dynamic_cast<Block*>(prog.block_->clone());
		assert(block_);
		name_ = new string(*prog.name_);
	}

	virtual ~Program() {
		if (block_) delete block_;
		delete name_;
	}

	void print(ostream& os) {
		os << "Node name : Root/Program. Program name is: " << *name_ << endl;
		assert(block_);
		block_->print(os);
	}
	void pcodegen(ostream& os) {/*Program*/
		extern int* space;
		extern string** names;
		extern string** types;
		extern int* address;
		extern int current_f;
		extern int current_i;
		extern int current_dec;
		extern int* function_lower_bond;
		extern int* wrote_ssp;
		extern string** function_name;
		names[0] = name_;
		space[0] = 5;
		address[0] = 0;
		types[0] = new string("PROGRAM");
		current_i++;
		function_name[current_f] = name_;
		extern int* function_static_link;
		function_static_link[current_f] = -1;
		function_lower_bond[current_f] = current_i - 1;
		wrote_ssp[current_f] = 0;
		current_dec = 0;
		current_f++;
		assert(block_);
		block_->pcodegen(os);
		os << "stp" << endl;
		//os << "ujp " << *name_ << "_begin";
	}
	virtual Object* clone() const { return new Program(*this); }

private:
	Block* block_;
	string* name_;
};





#endif //AST_H