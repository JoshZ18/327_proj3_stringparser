/*
 * StringParserClass.cpp
 *
 *  Created on: Oct 8, 2017
 *      Author: keith
 */

#include <string>
#include <string.h>

#include "../327_proj3_test/includes/StringParserClass.h"
#include "../327_proj3_test/includes/constants.h"

namespace KP_StringParserClass{

	//dont forget to initialize member variables
	StringParserClass::StringParserClass(void) {
		pStartTag = 0;
		pEndTag = 0;
		areTagsSet = false;
	}

	//call cleanup to release any allocated memory
	StringParserClass::~StringParserClass(void) {
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

		*pStartTag = *pStart;
		*pEndTag = *pEnd;
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
		myVector.clear();

		if (pDataToSearchThru == 0) {
			return ERROR_DATA_NULL;
		}

		std::string line(pDataToSearchThru);
		std::string start(pStartTag);
		std::string end(pEndTag);
		std::string search = "";

		if (areTagsSet) {
			for (int i = 0; i < line.length(); i++) {
				search += line.substr(i, 1);

				if (search == start) {
					search = "";

					for (int j = 0; j < line.length() - i; j++) {
						search += line.substr(i, 1);
						i++;

						std::string nextChar = line.substr(i,1);

						if (nextChar == "<") {
							myVector.push_back(search);
							search = "";
						}

						int length = line.length();

						if (line[length - 1] == '>') {
							search = "";
							break;
						}
					}
				}
			}
			return SUCCESS;
		}

		return ERROR_TAGS_NULL;
	}

//	private:
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
			std::string searchTag = "";

			for (int i = 0; i < line.length(); i++) {
				searchTag += line.substr(i, 1);

				if (searchTag == tag) {
					char start = searchTag[0];
					int length = searchTag.length();
					char end = searchTag[length - 1];
					pStart = &start;
					pEnd = &end;

					return SUCCESS;
				}

				int length = searchTag.length();
				if (searchTag[length - 1] == '>') {
					searchTag = "";
				}

			}

			return SUCCESS;
		}

		return ERROR_TAGS_NULL;
	}

	char	*pStartTag;
	char	*pEndTag;
	bool	areTagsSet;
}
