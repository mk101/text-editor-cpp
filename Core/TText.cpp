#include "TText.h"
#include"TDataCom.h"
#include<fstream>
#include<iostream>
#include"TTextMem.h"
PTTtextLink TText::GetFirstAtom(PTTtextLink pl)
{
    PTTtextLink value = pCurrent;
    while (value->PDown != nullptr) {
        value = value->PDown;
   }
    return value;
}

void TText::PrintText(PTTtextLink ptl, std::ostream& out)
{
    

}

PTTtextLink TText::ReadText(std::ifstream& Txtfile)
{
    char Strbuff[TextLineLength];
    int Buflength = TextLineLength;
    int textlevel;
    PTTtextLink PHead, ptl;
    PHead = ptl = new TTextLink();
    while (Txtfile.eof() == 0) {
        Txtfile.getline(Strbuff, Buflength, '\n');
        if (Strbuff[0] == '}') {
            textlevel--;
            break;
        }
        else if (Strbuff[0] == '{') {
            textlevel++;
            ptl->PDown = ReadText(Txtfile);
        }
        else {
            ptl->PNext = new TTextLink(Strbuff, nullptr, nullptr);
            ptl = ptl->PNext;
        }
    }
    ptl = PHead;
    if (PHead->PDown = nullptr) {
        PHead = PHead->PNext;
        delete ptl;
    }
    return PHead;
}
TText::TText(PTTtextLink pl) 
{
    if (pl == nullptr) {
        pl = new TTextLink();
    }
    pFirst = pl;

}

int TText::Reset(void)
{
    while (!St.empty()) {
        St.pop();
    }
    pCurrent = pFirst;
    if (pCurrent != nullptr) {
        St.push(pCurrent);
        if (pCurrent->PNext != nullptr) {
            St.push(pCurrent->PNext);
        }
        if (pCurrent->PDown != nullptr) {
            St.push(pCurrent->PDown);
        }
    }
    return IsTextEnded();
}

int TText::IsTextEnded(void) const
{
    return !St.size();
}

int TText::GoNext(void)
{
    if (!IsTextEnded()) {
        pCurrent = St.top();
        St.pop();
        if (pCurrent != pFirst) {
            if (pCurrent->PNext != nullptr) {
                St.push(pCurrent->PNext);
            }
            if (pCurrent->PDown != nullptr) {
                St.push(pCurrent->PDown);
            }
        }
    }
    return IsTextEnded();
}

void TText::Read(char* pFileName) {
	throw std::runtime_error("No implementation");
}

void TText::Write(char* pFileName) {
	throw std::runtime_error("No implementation");
}

void TText::Print(std::ostream& stream) {
	throw std::runtime_error("No implementation");
}

int TText::GoFirstLink(void) {
    while (!Path.empty()) {
        Path.pop();
    }
    pCurrent = pFirst;
    if (pCurrent == nullptr) {
        SetRetCode(TextError);
    }
    else {
        SetRetCode(DataOK);
    }
    return RetCode;
}

int TText::GoNextLink(void) {
    SetRetCode(TextError);
    if (pCurrent != nullptr) {
        if (pCurrent->PNext != nullptr) {
            Path.push(pCurrent);
            pCurrent = pCurrent->PNext;
            SetRetCode(DataOK);
        }
    }
    return RetCode;
}

int TText::GoPrevLink(void) {
	return DataErr;
}

std::string TText::GetLine(void) {
    return pCurrent->str;
}

void TText::SetLine(std::string s) {
	throw std::runtime_error("No implementation");
}
