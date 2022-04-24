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

std::stringstream TText::PrintText(PTTtextLink ptl)
{
    static int level = 0;
    const int space_size = 4;

    PTTtextLink pLink = ptl;
    std::stringstream ss;
    while (pLink != nullptr) {

        ss << std::string(space_size * level, ' ');
        pLink->Print(ss);
        ss << std::endl;

        if (pLink->pDown != nullptr) {
            ss << std::string(space_size * level, ' ') << '{' << std::endl;
            level++;
            ss << PrintText(pLink->PDown).rdbuf();
            level--;
            ss << std::string(space_size * level, ' ') << '}' << std::endl;
        }

        pLink = pLink->pNext;
    }


    return ss;

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

PTText TText::getCopy()
{
    return PTText();
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

int TText::GoDownLink(void)
{
    return 0;
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

void TText::InsDownLine(std::string s)
{
    if (pCurrent == nullptr)
    {
        SetRetCode(TextError);
    } 
    else
    {
        PTTtextLink pd = pCurrent->PDown;
        PTTtextLink pl = new TTextLink("", pd, nullptr);
        strncpy(pl->str, s.c_str(),TextLineLength);
        pl->str[TextLineLength - 1] = '\0';
        pCurrent->PDown = pl;
        SetRetCode(TextOK);

    }
}

void TText::InsDownSection(std::string s)
{
    if (pCurrent == nullptr)
    {
        SetRetCode(TextError);
    }
    else {
        PTTtextLink pd = pCurrent->PDown;
        PTTtextLink pl = new TTextLink("", nullptr, pd);
        strncpy(pl->str, s.c_str(),TextLineLength);
        pl->str[TextLineLength];
        pCurrent->PDown = pl;
        SetRetCode(TextOK);

    }
}

void TText::InsNextLine(std::string s)
{
    if (pCurrent == nullptr)
    {
        SetRetCode(TextError);
    }
    else {
        PTTtextLink pn = pCurrent->PNext;
        pCurrent->PNext;
        PTTtextLink pl = new TTextLink("", nullptr, pn);
        strncpy(pl->str, s.c_str(), TextLineLength);
        pl->str[TextLineLength];
        pCurrent->PNext = pl;

    }
}

void TText::InsNextSection(std::string s)
{

}

void TText::DelDownLine(void)
{
}

void TText::DelDownSection(void)
{
    SetRetCode(TextOK);
    if (pCurrent == nullptr) {
        SetRetCode(TextError);
    }
    else if (pCurrent->PDown!=nullptr) {
        PTTtextLink pd = pCurrent->PDown;
        PTTtextLink pn = pd->PNext;
        pCurrent->PDown = pn;
    }

}

void TText::DelNextLine(void)
{
    SetRetCode(TextOK);
    if (pCurrent == nullptr) {
        SetRetCode(TextError);
    }
    else if (pCurrent->PNext != nullptr) {
        PTTtextLink pn = pCurrent->PNext;
        PTTtextLink pnn = pn->PNext;
        if (pn->PDown == nullptr) {
            pCurrent->PNext = pnn;
        }



    }

}

void TText::DelNextSection(void)
{
}
