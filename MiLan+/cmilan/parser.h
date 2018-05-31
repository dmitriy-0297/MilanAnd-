#ifndef CMILAN_PARSER_H
#define CMILAN_PARSER_H

#include "scanner.h"
#include "codegen.h"
#include <iostream>
#include <sstream>
#include <string>
#include <map>

/* Синтаксический анализатор.
 *
 * Задачи:
 * - проверка корректности программы,
 * - генерация кода для виртуальной машины в процессе анализа,
 * - простейшее восстановление после ошибок.
 *
 * Синтаксический анализатор языка Милан.
 * 
 * Парсер с помощью переданного ему при инициализации лексического анализатора
 * читает по одной лексеме и на основе грамматики Милана генерирует код для
 * стековой виртуальной машины. Синтаксический анализ выполняется методом
 * рекурсивного спуска.
 * 
 * При обнаружении ошибки парсер печатает сообщение и продолжает анализ со
 * следующего оператора, чтобы в процессе разбора найти как можно больше ошибок.
 * Поскольку стратегия восстановления после ошибки очень проста, возможна печать
 * сообщений о несуществующих ("наведенных") ошибках или пропуск некоторых
 * ошибок без печати сообщений. Если в процессе разбора была найдена хотя бы
 * одна ошибка, код для виртуальной машины не печатается.*/

class Parser 
{
public:
	Parser(const std::string& fileName, std::istream& input);
	~Parser();
	void parse();

private:
	void program();
	
	void statement_list();
	void statement();

	void if_statement();
	void while_statement();
	void write_statement();
	
	void assigment_statement();
	void ternary_expression();
	void logic_expression();
	void logic_term();
	void arithmetic_term();
	void arithmetic_factor();

	bool check(const Token expected);
	bool current(const Token expected);
   
	void next();
	void reportError(const std::string& message);
	void recover(Token t);
	int findOrAddVariable(const std::string&);

	typedef std::map<std::string, int> VarTable;
	Scanner* scanner_;
	CodeGen* codegen_;
	std::ostream& output_;
	bool error_; 
	VarTable variables_;
	int lastVar_;
};

#endif
