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
    const int space_size = 1;

    PTTtextLink pLink = ptl;
    std::stringstream ss;
    while (pLink != nullptr) {

        ss << std::string(space_size * level, '\t');

        //pLink->Print(ss);
        ss << pLink->str;
        
        if (pLink->PNext != nullptr || pLink->PDown != nullptr) {
            ss << std::endl;
        }

        if (pLink->PDown != nullptr) {
            ss << std::string(space_size * level, '\t') << '{' << std::endl;
            level++;
            ss << PrintText(pLink->PDown).rdbuf();
            level--;
            ss << std::string(space_size * level, '\t') << '}'/* << std::endl */ ;
            if (pLink->PNext != nullptr) {
                ss << std::endl;
            }
        }

        pLink = pLink->PNext;
    }

    if (level > 0) {
        ss << std::endl;
    }

    return ss;

}

std::stringstream TText::PrintTextInline(PTTtextLink ptl) {
    static int level = 0;
    const int space_size = 1;

    PTTtextLink pLink = ptl;
    std::stringstream ss;
    while (pLink != nullptr) {

        //pLink->Print(ss);
        ss << pLink->str;

        if (pLink->PDown != nullptr) {
            ss << " { ";
            level++;
            ss << PrintTextInline(pLink->PDown).rdbuf();
            level--;
            ss << " } ";
        }

        pLink = pLink->PNext;
    }

    return ss;
}

PTTtextLink TText::ReadText(std::ifstream& Txtfile)
{
    char Strbuff[TextLineLength];
    int Buflength = TextLineLength;
    int textlevel = 0;
    PTTtextLink PHead, ptl;
    PHead = ptl = new TTextLink();
    while (Txtfile.eof() == 0) {
        Txtfile.getline(Strbuff, Buflength, '\n');

        std::string buf = Strbuff;
        size_t count = 0;
        for (size_t i = 0; i < buf.size(); i++) {
            if (buf.at(i) != ' ') {
                break;
            }

            count++;
        }

        buf.erase(0, count);
        
        strcpy(Strbuff, buf.c_str());

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
    if (PHead->PDown == nullptr) {
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

    while (!PrevSt.empty()) {
        PrevSt.pop();
    }
    pCurrent = pFirst;
    if (pCurrent != nullptr) {
        //St.push(pCurrent);
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
        PrevSt.push(pCurrent);
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

void TText::GoPrev() {
    if (PrevSt.size() != 0) {
        pCurrent = PrevSt.top();
        PrevSt.pop();

        while (!St.empty()) {
            St.pop();
        }

        //if (pCurrent != nullptr) {
        //    St.push(pCurrent);
        //    if (pCurrent->PNext != nullptr) {
        //        St.push(pCurrent->PNext);
        //    }
        //    if (pCurrent->PDown != nullptr) {
        //        St.push(pCurrent->PDown);
        //    }
        //}

        PTTtextLink ptr = pFirst;

        if (ptr != nullptr) {
            //St.push(ptr);
            if (ptr->PNext != nullptr) {
                St.push(ptr->PNext);
            }
            if (ptr->PDown != nullptr) {
                St.push(ptr->PDown);
            }
        }

        while (ptr != pCurrent) {
            ptr = St.top();
            St.pop();
            if (ptr != pFirst) {
                if (ptr->PNext != nullptr) {
                    St.push(ptr->PNext);
                }
                if (ptr->PDown != nullptr) {
                    St.push(ptr->PDown);
                }
            }
        }
    }
}

void TText::Read(const char* pFileName) {
    std::ifstream file(pFileName);

    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }

    pFirst = ReadText(file);
}

void TText::Write(const char* pFileName) {
	throw std::runtime_error("No implementation");
}

void TText::Print(std::ostream& stream) {
    stream << PrintText(pFirst).str();
}

void TText::PrintInline(std::ostream& stream) {
    stream << PrintTextInline(pFirst).str();
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
    SetRetCode(TextError);

    if (Path.size() != 0) {
        pCurrent = Path.top();
        Path.pop();
        SetRetCode(DataOK);
    }

    return RetCode;
}

std::string TText::GetLine(void) {
    return pCurrent->str;
}

void TText::SetLine(std::string s) {
    strcpy(pCurrent->str, s.c_str());
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
        pCurrent->PNext = pl;

    }
}

void TText::InsNextSection(std::string s) {
    if (pCurrent == nullptr) {
        SetRetCode(TextError);
    } else {
        PTTtextLink pn = pCurrent->PNext;
        pCurrent->PNext;
        PTTtextLink pl = new TTextLink("", pn, nullptr);
        strncpy(pl->str, s.c_str(), TextLineLength);
        pCurrent->PNext = pl;

    }
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

void TText::DelCurrent() {
    PTTtextLink cur = pCurrent;

    if (cur == pFirst && pFirst->PNext == nullptr) {
        return;
    }

    if (cur == pFirst) {
        pFirst = cur->PNext;
        return;
    }

    Reset();

    while (pCurrent->PNext != cur && pCurrent->PDown != cur) {
        GoNext();
    }


    if (pCurrent->PDown == cur) {
        pCurrent->PDown = cur->PNext;
        return;
    }

    pCurrent->PNext = cur->PNext;
}
