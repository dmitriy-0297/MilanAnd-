#include "parser.h"
#include <sstream>
Parser::Parser(const string& fileName, istream& input):
    output_(cout),
	error_(false),
	lastVar_(0)
	{
		scanner_ = new Scanner(fileName, input);
		codegen_ = new CodeGen(output_);
		next();
	}

Parser::~Parser()
{
	delete codegen_;
	delete scanner_;
}

void Parser::program()
{
	check(T_BEGIN);
	statement_list();
	check(T_END);
	codegen_->emit(STOP);
}

void Parser::statement_list()
{
    statement();
	while(current(T_SEMICOLON))
	{
		check(T_SEMICOLON);
		statement();
	}
}

void Parser::statement()
{
	if(current(T_IF))
	{
		if_statement();
	}
	else if(current(T_WHILE))
	{
		while_statement();
	}
	else if(current(T_WRITE))
	{
		write_statement();
	}
	else if(current(T_IDENTIFIER))
	{
		assigment_statement();
	}
	else if(current(T_ILLEGAL))
	{
		reportError("Unexpected token in statement");
	}
}

void Parser::if_statement()
{
	check(T_IF);
	logic_expression();
	check(T_THEN);
	
	int jumpNoAddress = codegen_->reserve();
	statement_list();
	
	if(current(T_ELSE))
	{
		int jumpAddress = codegen_->reserve();
		check(T_ELSE);
		codegen_->emitAt(jumpNoAddress, JUMP_NO, codegen_->getCurrentAddress());
		statement_list();
		codegen_->emitAt(jumpAddress, JUMP, codegen_->getCurrentAddress());
	}
	else
	{
		codegen_->emitAt(jumpNoAddress, JUMP_NO, codegen_->getCurrentAddress());
	}	
	check(T_FI);
}

void Parser::while_statement()
{
	check(T_WHILE);
	int conditionAddress = codegen_->getCurrentAddress();
	logic_expression();
	int jumpNoAddress = codegen_->reserve();
	check(T_DO);
	statement_list();	
	codegen_->emit(JUMP, conditionAddress);
	codegen_->emitAt(jumpNoAddress, JUMP_NO, codegen_->getCurrentAddress());
	check(T_OD);
}

void Parser::write_statement()
{
	check(T_WRITE);
	check(T_LPAREN);
	ternary_expression();
	check(T_RPAREN);
	codegen_->emit(PRINT);	
}

void Parser::assigment_statement()
{
	int varAddress = findOrAddVariable(scanner_->getStringValue());
	check(T_IDENTIFIER);
	check(T_ASSIGN);
	ternary_expression();
	codegen_->emit(STORE, varAddress);
}

void Parser::ternary_expression()
{
	logic_expression();
	if(current(T_TERNARY))
	{
		check(T_TERNARY);
		int jumpNoAddress = codegen_->reserve();
		ternary_expression();
		check(T_TERNARY_VAR);   
		int jumpAddress = codegen_->reserve();
		codegen_->emitAt(jumpNoAddress, JUMP_NO, codegen_->getCurrentAddress());
		ternary_expression();
		codegen_->emitAt(jumpAddress, JUMP, codegen_->getCurrentAddress());
	}
}

void Parser::logic_expression()
{
	logic_term();
	while(current(T_CMP))
	{
		Cmp cmp = scanner_->getCmpValue();
		check(T_CMP);
		logic_term();
		codegen_->emit(COMPARE, cmp);
	}
}

void Parser::logic_term()
{
	arithmetic_term();
	while(current(T_ADDOP))
	{
		Arithmetic op = scanner_->getArithmeticValue();
		check(T_ADDOP);
		arithmetic_term();

		if(op == A_PLUS)
		{
			codegen_->emit(ADD);
		}
		else if (op == A_MINUS)
		{
			codegen_->emit(SUB);		
		}
	}
}

void Parser::arithmetic_term()
{
	arithmetic_factor();
	while(current(T_MULOP))
	{
		Arithmetic op = scanner_->getArithmeticValue();
		check(T_MULOP);
		arithmetic_factor();

		if(op == A_MULTIPLY)
		{
			codegen_->emit(MULT);
		}
		else if(op == A_DIVIDE)
		{
			codegen_->emit(DIV);
		}
	}
}

void Parser::arithmetic_factor()
{
	if(current(T_ADDOP))
	{
		Arithmetic op = scanner_->getArithmeticValue();
		check(T_ADDOP);
		arithmetic_factor();
		if(op == A_MINUS)
		{
			codegen_->emit(INVERT);
		}
	}
	else if(current(T_IDENTIFIER))
	{
		int varAddress = findOrAddVariable(scanner_->getStringValue());
		check(T_IDENTIFIER);
		codegen_->emit(LOAD, varAddress);
	}
	else if(current(T_NUMBER))
	{
		int value = scanner_->getIntValue();
		check(T_NUMBER);
		codegen_->emit(PUSH, value);		
	}
	else if(current(T_LPAREN))
	{
		check(T_LPAREN);
		ternary_expression();
		check(T_RPAREN);
	}
	else if(current(T_READ))
	{
		check(T_READ);
		codegen_->emit(INPUT);
	}
	else if(current(T_ILLEGAL))
	{
		reportError("Unexpected symbol while expression element expected");
	}
}

void Parser::parse()
{
	program(); 
	if(!error_)
	{
		codegen_->flush();
	}
}

int Parser::findOrAddVariable(const string& var)
{
	VarTable::iterator it = variables_.find(var);
	if(it == variables_.end())
	{
		variables_[var] = lastVar_;
		return lastVar_++;
	}
	else
	{
		return it->second;
	}
}

void Parser::recover(Token t)
{
	while(!current(t) && !current(T_EOF))
	{
		next();
	}
	
	if(current(t))
	{
		next();
	}
}

void Parser::next()
{
	scanner_->nextToken();
}

void Parser::reportError(const string& message)
{
	cerr << "Line " << scanner_->getLineNumber() << ": " << message << endl;
	error_ = true;
}

bool Parser::check(const Token expected)
{
	if(scanner_->token() != expected)
	{
		std::stringstream msg;
		msg << tokenToString(scanner_->token()) << " while expected ";
		msg	<< tokenToString(expected);
		reportError(msg.str());
		error_ = true;
		recover(expected);
		return false;
	}
	else
	{
		next();
		return true;
	}
}

bool Parser::current(const Token variant)
{
	return scanner_->token() == variant;
}
