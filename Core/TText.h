#pragma once
#include "TDataCom.h"
#include"TTextMem.h"
#include<iostream>
#include<stdio.h>
#include <fstream>
#include<stack>
#include<string>
#include <exception>

typedef TText* PTText;

class TText :public TDataCom {
protected:
	PTTtextLink pFirst;
	PTTtextLink pCurrent;
	std::stack<PTTtextLink> Path;
	std::stack<PTTtextLink> St;
	PTTtextLink GetFirstAtom(PTTtextLink pl);
	void PrintText(PTTtextLink ptl);
	PTTtextLink ReadText(std::ifstream);
public:
	TText(PTTtextLink pl = NULL);
	~TText() {
		pFirst = NULL;
	}
	TText(const TText& t) = delete;
	TText& operator=(const TText& t) = delete;
	PTText getCopy();

	int GoFirstLink(void);
	int GoDownLink(void);
	int GoNextLink(void);
	int GoPrevLink(void);

	std::string GetLine(void);
	void SetLine(std::string s);

	void InsDownLine(std::string s);
	void InsDownSection(std::string s);
	void InsNextLine(std::string s);
	void InsNextSection(std::string s);
	void DelDownLine(void);
	void DelDownSection(void);
	void DelNextLine(void);
	void DelNextSection(void);
	int Reset(void);
	int IsTextEnded(void) const;
	int GoNext(void);
	void Read(char* pFileName);
	void Write(char* pFileName);
	void Print(std::ostream &stream);
};

