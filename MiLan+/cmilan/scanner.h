#ifndef CMILAN_SCANNER_H
#define CMILAN_SCANNER_H

#include "tokens.h"
#include <fstream>
#include <string>
#include <map>

// Лексический анализатор
class Scanner
{
public:
	// Конструктор. В качестве аргумента принимает имя файла и поток,
    // из которого будут читаться символы транслируемой программы.
	explicit Scanner(const std::string & fileName, std::istream& input);
	virtual ~Scanner();

	const std::string & getFileName() const;
	int getLineNumber() const;
	Token token() const;
	int getIntValue() const;
	std::string getStringValue() const;
	Cmp getCmpValue() const;
	Arithmetic getArithmeticValue() const;
	// Переход к следующей лексеме.
	// Текущая лексема записывается в token_ и изымается из потока.
	void nextToken();	

private:
	// Пропуск всех пробельные символы. 
	// Если встречается символ перевода строки, номер текущей строки
	// (lineNumber) увеличивается на единицу.
	void skipSpace();
	void skipComments();
	
	void catchEOF();
	void catchNumber();
	void catchOperation();	

	void nextChar(); //переходит к следующему символу

	const std::string fileName_; //входной файл
	int lineNumber_; //номер текущей строки кода 
	Token token_; //текущая лексема
	int intValue_; //значение текущего целого
	std::string stringValue_; //имя переменной
	Cmp cmpValue_; //значение оператора сравнения (>, <, =, !=, >=, <=)
	Arithmetic arithmeticValue_; //значение знака (+,-,*,/)
	
	std::map< std::string, Token > keywords_; //ассоциативный массив с лексемами и 
	//соответствующими им зарезервированными словами в качестве индексов

	std::istream& input_; //входной поток для чтения из файла.
	char ch_; //текущий символ
};

#endif
