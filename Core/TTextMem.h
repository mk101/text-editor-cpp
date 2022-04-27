#pragma once
#include <iostream>
#include "TDatValue.h"
#include"TText.h"
#define TextOK 0
#define TextNoDown 101
#define TextNoNext 102
#define TextNoPrev 103

#define TextNoMem -101
#define TextError -102
#define TextLineLength 100
#define Memsize 20
class TText;
class TTextLink;

typedef TTextLink* PTTtextLink;
typedef char Tstr[(TextLineLength)];//хранит строчку
class TTextMem {
    PTTtextLink pfirst;
    PTTtextLink plast;
    PTTtextLink pFree;//первый свобоный эл-т
    friend class TTextLink;

};
typedef TTextMem* PTTextMem;
class TTextLink : public TDatValue {
protected:
    PTTtextLink PNext, PDown;
    Tstr str;
    inline static TTextMem MemHeader;//кусок оперативной памяти для работы
    friend class TText;//для приватов и протектов указывать дружественный класс
public:

    static void InitMemSystem(int size = Memsize) {
        MemHeader.pfirst = (PTTtextLink)new char[sizeof(TTextLink) * size];
        MemHeader.pFree = MemHeader.pfirst;
        MemHeader.plast = MemHeader.pfirst + (size - 1);
        PTTtextLink pLink = MemHeader.pfirst;
        for (int i = 0; i < size - 1; pLink++, i++) {
            pLink->PNext = pLink + 1;
        }
        pLink->PNext = nullptr;


    }
    static void printFreeLink(void) {
        PTTtextLink Plink = MemHeader.pFree;
        for (; Plink != nullptr; Plink = Plink->PNext) {
            std::cout << Plink->str << std::endl;
        }
    }
    void* operator new(size_t size) {
        PTTtextLink Plink = MemHeader.pFree;
        if (MemHeader.pFree != nullptr) {
            MemHeader.pFree = Plink->PNext;
        }
        return Plink;
    }
    void operator delete(void* pM) {
        PTTtextLink pLink = (PTTtextLink)pM;
        pLink->PNext = MemHeader.pFree;
        MemHeader.pFree = pLink;
    }
    void MemCleaner( TText& txt) {
        for (txt.Reset(); !txt.IsTextEnded(); txt.GoNext()) {
            if (txt.GetLine().find("&&&")!=0) {
                txt.SetLine("&&&" + txt.GetLine());
            }
        }
        PTTtextLink Plink = MemHeader.pFree;
        for (; Plink <= MemHeader.plast; Plink++) {
            if (strstr(Plink->str, "&&&") != NULL) {
                strcpy(Plink->str, Plink->str + 3);;
            }
            else {
                delete Plink;
            }
        }
    }

    TTextLink(const Tstr s = NULL, PTTtextLink pn = NULL, PTTtextLink pd = NULL) {
        PNext = pn;
        PDown = pd;
        if (s != NULL) {
            strcpy(str, s);
        } else {
            str[0] = '\0';
        }
    }
    ~TTextLink() {

    }
    int IsAtom() {
        return PDown == NULL;
    }
    PTTtextLink GetNext() {
        return PNext;
    }
    PTTtextLink GetDown() {
        return PDown;
    }
    PTTtextLink GetCopy() override {
        return new TTextLink(str, PNext, PDown);
    }

};

