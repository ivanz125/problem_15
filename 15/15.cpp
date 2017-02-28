#include "stdafx.h"
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
using namespace std;

struct Article {
	
private:
	string title;
	string authors;
	string text;
	string hash;
	static const int indentSize = 4;

	void writeTag(ostringstream& oss, int tab, const string& name, const string& value){
		string tabStr(tab * indentSize, ' ');
		oss << tabStr << "<" << name << ">\n";
		oss << tabStr << tabStr << value << "\n";
		oss << tabStr << "</" << name << ">\n";
	}

public:
	Article(){}

	void setTitle(string title){
		this->title = title;
	}

	void setAuthors(string authors){
		this->authors = authors;
	}

	void setText(string text){
		this->text = text;
	}

	void setHash(){
		std::hash<string> hasher;
		size_t hash = hasher(text);
		this->hash = to_string(hash);
	}

	string toString(){
		ostringstream oss;
		oss << "<article>\n";
		string singleTab(indentSize, ' ');
		string doubleTab(indentSize*2, ' ');
		writeTag(oss, 1, "title", title);
		writeTag(oss, 1, "authors", authors);
		writeTag(oss, 1, "text", text);
		writeTag(oss, 1, "hash", hash);
		oss << "</article>";
		return oss.str();
	}
};

struct ArticleBuilder {
	
	Article* article;

	ArticleBuilder() {
		article = new Article();
	}

	ArticleBuilder& setTitle(string title){
		article->setTitle(title);
		return *this;
	}

	ArticleBuilder& setAuthors(string authors){
		article->setAuthors(authors);
		return *this;
	}

	ArticleBuilder& setText(string text){
		article->setText(text);
		return *this;
	}

	Article* build(){
		article->setHash();
		return article;
	}
};

int main(int argc, char* argv[])
{
	Catch::Session().run(argc, argv);
	system("pause");
	return 0;
}

// Check all lines match except hash (line 12)
TEST_CASE("toString() method (from file)"){
	// White info to file
	ifstream inp;
	inp.open("article.txt");
	string title;
	string authors;
	string text;
	getline(inp, title);
	getline(inp, authors);
	getline(inp, text);
	inp.close();

	// Builder
	ArticleBuilder builder;
	Article* article = builder
		.setTitle(title)
		.setAuthors(authors)
		.setText(text)
		.build();

	string articleStr = article->toString();

	ofstream out;
	out.open("article.xml");
	out << articleStr;
	out.close();

	// Open example (etalon)
	ifstream inpEx;
	inpEx.open("article_ex.xml");
	// Open written file
	ifstream inpRes;
	inpRes.open("article.xml");
	// Check lines
	for (int i = 0; i < 14; i++) {
		string lRes;
		string lEx;
		getline(inpRes, lRes);
		getline(inpEx, lEx);
		// i == 11 - hash
		//cout << i << endl << lRes << endl << lEx << endl << endl;
		if (i != 11) CHECK(lRes.compare(lEx) == 0);
	}
}