//
// Created by professor on 8/11/21.
//

#ifndef COURSERA_BROWN_BELT_C_INI_H
#define COURSERA_BROWN_BELT_C_INI_H

#include <string>
#include <unordered_map>
#include <istream>

using namespace std;

namespace Ini {

	using Section = unordered_map<string, string>;

	class Document {
	public:
		Section &AddSection(string name);

		const Section &GetSection(const string &name) const;

		size_t SectionCount() const;

	private:
		unordered_map <string, Section> sections;
	};

	Document Load(istream &input);

}
#endif //COURSERA_BROWN_BELT_C_INI_H
