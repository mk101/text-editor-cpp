#pragma once
#include "TDataCom.h"
#include"TTextMem.h"
#include<iostream>
#include<stdio.h>
#include <fstream>
#include<stack>
#include<string>
#include <sstream>
#include <exception>

typedef TText* PTText;

class TText :public TDataCom {
protected:
	PTTtextLink pFirst;// указатель корня дерева
	PTTtextLink pCurrent;//указатель текущей строки
	std::stack<PTTtextLink> Path;// стек траектории движения по тексту
	std::stack<PTTtextLink> St;// стек для итератора
	std::stack<PTTtextLink> PrevSt;
	PTTtextLink GetFirstAtom(PTTtextLink pl);// поиск первой строки
	std::stringstream PrintText(PTTtextLink ptl);// печать текста со звена ptl
	std::stringstream PrintTextInline(PTTtextLink ptl);
	PTTtextLink ReadText(std::ifstream& Txtfile); //чтение текста из файла

public:
	TText(PTTtextLink pl = NULL);
	~TText() {
		pFirst = NULL;
	}
	TText(const TText& t) = delete;
	TText& operator=(const TText& t) = delete;
	PTText getCopy();

	int GoFirstLink(void);// переход к первой строке
	int GoDownLink(void);// переход к следующей строке по Down
	int GoNextLink(void);// переход к следующей строке по Next
	int GoPrevLink(void);// переход к предыдущей позиции в тексте

	std::string GetLine(void); // чтение текущей строки
	void SetLine(std::string s);// замена текущей строки

	void InsDownLine(std::string s);// вставка строки в подуровень
	void InsDownSection(std::string s);// вставка раздела в подуровень
	void InsNextLine(std::string s);// вставка строки в том же уровне
	void InsNextSection(std::string s);// вставка раздела в том же уровне
	void DelDownLine(void);// удаление строки в подуровне
	void DelDownSection(void); // удаление раздела в подуровне
	void DelNextLine(void);// удаление строки в том же уровне
	void DelNextSection(void);// удаление раздела в том же уровне
	void DelCurrent();
	int Reset(void); // установить на первую звапись
	int IsTextEnded(void) const;
	int GoNext(void);// переход к следующей записи
	void GoPrev();
	void Read(const char* pFileName);// ввод текста из файла
	void Write(const char* pFileName);// вывод текста в файл
	void Print(std::ostream &stream);// печать текста
	void PrintInline(std::ostream &stream);
};

