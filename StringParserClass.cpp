/*
 * StringParserClass.cpp
 *
 *  Created on: Oct 8, 2017
 *      Author: keith
 */

#include <iostream>
#include <string>
#include <string.h>

#include "../327_proj3_test/includes/StringParserClass.h"
#include "../327_proj3_test/includes/constants.h"

namespace KP_StringParserClass{

	//dont forget to initialize member variables
	StringParserClass::StringParserClass(void) {
		pStartTag = new char;
		pEndTag = new char;
		areTagsSet = false;
	}

	//call cleanup to release any allocated memory
	StringParserClass::~StringParserClass(void) {
		delete pStartTag;
		delete pEndTag;
	}

	//these are the start tag and the end tags that we want to find,
	//presumably the data of interest is between them, please make a
	//COPY of what pStartTag and pEndTag point to.  In other words
	//DO NOT SET pStartTag = pStart
	//returns:
	//SUCCESS
	//ERROR_TAGS_NULL if either pStart or pEnd is null
	int StringParserClass::setTags(const char *pStart, const char *pEnd) {
		if (pStart == 0 || *pStart == '\0') {
			return ERROR_TAGS_NULL;
		}

		if (pEnd == 0 || *pEnd == '\0') {
			return ERROR_TAGS_NULL;
		}

		int i = 0;
		while (*(pStart + i) != '>') {
			*(pStartTag + i) = *(pStart + i);
			i++;
		}
		*(pStartTag + i) = *(pStart + i);

		i = 0;
		while (*(pEnd + i) != '>') {
			*(pEndTag + i) = *(pEnd + i);
			i++;
		}
		*(pEndTag + i) = *(pEnd + i);

		areTagsSet = true;

		return SUCCESS;
	}

	//First clears myVector
	//going to search thru pDataToSearchThru, looking for info bracketed by
	//pStartTag and pEndTag, will add that info only to myVector
	//returns
	//SUCCESS  finished searching for data between tags, results in myVector (0 or more entries)
	//ERROR_TAGS_NULL if either pStart or pEnd is null
	//ERROR_DATA_NULL pDataToSearchThru is null
	int StringParserClass::getDataBetweenTags(char *pDataToSearchThru, std::vector<std::string> &myVector) {
		if (!areTagsSet) {
			return ERROR_TAGS_NULL;
		}

		myVector.clear();

		if (pDataToSearchThru == NULL) {
			return ERROR_DATA_NULL;
		}

		std::string line(pDataToSearchThru);
		std::string start(pStartTag);
		std::string str;

		for (int i = 0; i < start.length(); i++) {
			std::string character = start.substr(i,1);
			if (character == ">") {
				str += character;
				break;
			}
			str += character;
		}

		start = str;

		std::string end(pEndTag);
		std::string en = "";

		for (int i = 0; i < start.length(); i++) {
			std::string character = start.substr(i,1);

			if (character == "<") {
				en += character + "/";
			}

			if (character == ">") {
				en += character;
				break;
			}

			if (character != "<") {
				en += character;
			}
		}
		end = en;

		std::string search = "";

		for (int i = 0; i < line.length(); i++) {
			std::string character = line.substr(i,1);

			if (character == "<") {
				search = "";
			}

			search += line.substr(i, 1);

			if (search == start) {
				search = "";
				i++;
				std::string nextChar = "";

				while (nextChar != "<") {
					search += line.substr(i, 1);
					i++;

					nextChar = line.substr(i,1);

					if (nextChar == "<") {
						nextChar = line.substr(i+1, 1);
						if (nextChar == "/") {
							myVector.push_back(search);
							search = "";
							break;
						}
						break;
					}
				}
			}

			if (character == ">") {
				search = "";
			}
		}

		return SUCCESS;
	}

	void StringParserClass::cleanup() {
	}

	//Searches a string starting at pStart for pTagToLookFor
	//returns:
	//SUCCESS  found pTagToLookFor, pStart points to beginning of tag and pEnd points to end of tag
	//FAIL did not find pTagToLookFor and pEnd points to 0
	//ERROR_TAGS_NULL if either pStart or pEnd is null
	int StringParserClass::findTag(char *pTagToLookFor, char *&pStart, char *&pEnd) {
		if (pStart != 0 && pEnd != 0) {
			std::string line(pStart);
			std::string tag(pTagToLookFor);
			bool foundTag = false;

			std::string tg = "";
			for (int i = 0; i < line.length(); i++) {
				std::string character = line.substr(i,1);
				if (character == ">") {
					tg += character;
					break;
				}
				tg += character;
			}
			tag = tg;

			std::string endTag = "</" + tag.substr(1, tag.length() - 2) + ">";

			std::string searchTag = "";

			int count = 0;

			for (int i = 0; i < line.length(); i++) {
				std::string character = line.substr(i,1);

				if (character == "<") {
					searchTag = "";
				}

				searchTag += line.substr(i, 1);

				if (searchTag == tag) {
					foundTag = true;
					char * start = new char[searchTag.size() + 1];
					std::copy(searchTag.begin(), searchTag.end(), start);
					start[searchTag.size()] = '\0';

					pStart = start;
				}

				if (character == ">") {
					searchTag = "";
				}

				count++;
			}

			if (!foundTag) {
				pEnd = 0;
				return FAIL;
			}

			for (int i = count; i < line.length(); i++) {
				std::string character = line.substr(i,1);

				if (character == "<") {
					searchTag = "";
				}

				searchTag += line.substr(i, 1);

				if (searchTag == endTag) {
					char * end = new char[searchTag.size() + 1];
					std::copy(searchTag.begin(), searchTag.end(), end);
					end[searchTag.size()] = '\0';

					pEnd = end;
				}

				if (character == ">") {
					searchTag = "";
				}
			}

			return SUCCESS;
		}

		return ERROR_TAGS_NULL;
	}
}
