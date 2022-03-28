#include "pch.h"

TEST(TText, ReadLinearFile) {
	TText text;
	ASSERT_NO_THROW(text.Read("testText.txt"));
	
	std::ostringstream out;
	ASSERT_NO_THROW(text.Print(out));
	ASSERT_EQ(out.str(), std::string("first item\nsecond item\nlast item"));
}

TEST(TText, NavTest) {
	TText text;
	ASSERT_NO_THROW(text.Read("testText.txt"));

	ASSERT_EQ(text.GoFirstLink(), DataOK);
	ASSERT_EQ(text.GetLine(), std::string("first item"));

	ASSERT_EQ(text.GoNextLink(), DataOK);
	ASSERT_EQ(text.GetLine(), std::string("second item"));

	ASSERT_EQ(text.GoPrevLink(), DataOK);
	ASSERT_EQ(text.GetLine(), std::string("first item"));

	ASSERT_EQ(text.GoNextLink(), DataOK);
	ASSERT_EQ(text.GoNextLink(), DataOK);
	ASSERT_EQ(text.GetLine(), std::string("last item"));
}

TEST(TText, SetLine) {
	TText text;
	ASSERT_NO_THROW(text.Read("testText.txt"));

	ASSERT_NO_THROW(text.SetLine("new first line"));
	ASSERT_EQ(text.GetLine(), std::string("new first line"));
}

TEST(TText, WriteLinearFile) {
	TText text;

	ASSERT_NO_THROW(text.Read("testText.txt"));
	ASSERT_NO_THROW(text.SetLine("new first line"));
	ASSERT_NO_THROW(text.Write("newTestText.txt"));

	std::ifstream t("file.txt");
	std::stringstream buffer;
	buffer << t.rdbuf();

	ASSERT_EQ(buffer.str(), std::string("new first item\nsecond item\nlast item"));

}