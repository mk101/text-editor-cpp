#include "TText.h"

TText::TText(PTTtextLink pl) {}

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
	return DataErr;
}

int TText::GoNextLink(void) {
	return DataErr;
}

int TText::GoPrevLink(void) {
	return DataErr;
}

std::string TText::GetLine(void) {
	throw std::runtime_error("No implementation");
}

void TText::SetLine(std::string s) {
	throw std::runtime_error("No implementation");
}
