//
// Created by professor on 8/11/21.
//

#include "ini.h"

namespace Ini {
	Section &Document::AddSection(string name) {
		return sections[move(name)];
	}

	const Section &Document::GetSection(const string& name) const {
		return sections.at(name);
	}

	size_t Document::SectionCount() const {
		return sections.size();
	}

	Document Load(istream &input) {
		Document doc;
		for (char c; input >> c; ) {
			if (c == '[') {
				string section_name;
				//get section name
				getline(input, section_name, ']');
				Section &section = doc.AddSection(section_name);
				//skip whitespaces
				while (!input.eof() && isspace(input.peek())) {
					input.get();
				}
				//get variables
				while (!input.eof() && input.peek() != '[') {
					string name, val;
					getline(input, name, '=');
					getline(input, val);
					section.insert({name, val});
					//skip whitespaces
					while (!input.eof() && isspace(input.peek())) {
						input.get();
					}
				}

			}
		}

		return doc;
	}
}