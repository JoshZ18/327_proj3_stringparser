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
		//Creates char array pointers for the start and end tag
		pStartTag = new char[20];
		pEndTag = new char[20];
		//Sets areTagsSet to false as the arrays are empty
		areTagsSet = false;
	}

	//call cleanup to release any allocated memory
	StringParserClass::~StringParserClass(void) {
		//Calls cleanup to delete the pointers
		//Is called when StringParserClass object goes out of scope
		cleanup();
	}

	//these are the start tag and the end tags that we want to find,
	//presumably the data of interest is between them, please make a
	//COPY of what pStartTag and pEndTag point to.  In other words
	//DO NOT SET pStartTag = pStart
	//returns:
	//SUCCESS
	//ERROR_TAGS_NULL if either pStart or pEnd is null
	int StringParserClass::setTags(const char *pStart, const char *pEnd) {
		//Checks to see if the pStart or pEnd pointer is null
		if (pStart == 0 || pEnd == 0) {
			return ERROR_TAGS_NULL;
		}

		//Does a deep copy of the pStart and pEnd tags
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

		//Sets areTagsSet to true as the tags are now set
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
		//Checks to see if the tags are set before getting the data
		if (!areTagsSet) {
			return ERROR_TAGS_NULL;
		}

		//Checks to see if pDataToSearchThru is null before parsing
		if (pDataToSearchThru == NULL) {
			return ERROR_DATA_NULL;
		}

		myVector.clear();

		//Creates a deep copy of the end tag to see if the tags are in the data
		char *pEndCopy = new char[20];
		int i = 0;
		while (*(pEndTag + i) != '>') {
			*(pEndCopy + i) = *(pEndTag + i);
			i++;
		}
		*(pEndCopy + i) = *(pEndTag + i);

		//If the tags are not in the data then avoids parsing
		if (findTag(pStartTag, pDataToSearchThru, pEndCopy) == SUCCESS) {
			std::string line(pDataToSearchThru);

			//Makes a string of the tag and makes sure there is no irrelevant data
			//In the start tag
			std::string start(pStartTag);
			std::string str;
			int startLen = start.length();

			for (int i = 0; i < startLen; i++) {
				std::string character = start.substr(i,1);
				if (character == ">") {
					str += character;
					break;
				}
				str += character;
			}
			start = str;

			//Makes a string of the tag and makes sure there is no irrelevant data
			//In the end tag
			std::string end(pEndTag);
			std::string en = "";
			int endLen = end.length();
			for (int i = 0; i < endLen; i++) {
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

			//Searches for the data between the tags
			std::string search = "";
			int lineLen = line.length();
			for (int i = 0; i < lineLen; i++) {
				std::string character = line.substr(i,1);

				//If the character of the line is the start of a tag
				//Resets the search string
				if (character == "<") {
					search = "";
				}

				//Adds the character of the line to the search string
				search += line.substr(i, 1);

				//If the search equals the start tag then start extracting
				//the data
				if (search == start) {
					search = "";
					i++;
					std::string nextChar = "";

					//Keeps getting data until the next char is the start
					//Of the end tag
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

				//If character is equal to the end of a tag
				//Then reset the search for finding the start tag
				if (character == ">") {
					search = "";
				}
			}
		}

	return SUCCESS;
	}

	void StringParserClass::cleanup() {
		//If the pStartTag is not null
		//Then delete the pStartTag pointer
		if (pStartTag != 0) {
			delete[] pStartTag;
		}

		//If the pStartTag is not null
		//Then delete the pEndTag pointer
		if (pEndTag != 0) {
			delete[] pEndTag;
		}

	}

	//Searches a string starting at pStart for pTagToLookFor
	//returns:
	//SUCCESS  found pTagToLookFor, pStart points to beginning of tag and pEnd points to end of tag
	//FAIL did not find pTagToLookFor and pEnd points to 0
	//ERROR_TAGS_NULL if either pStart or pEnd is null
	int StringParserClass::findTag(char *pTagToLookFor, char *&pStart, char *&pEnd) {
		//If the pStart and pEnd is not null then look for the tag
		if (pStart != 0 && pEnd != 0) {
			std::string line(pStart);
			std::string tag(pTagToLookFor);
			bool foundTag = false;

			//Makes the string of a tag without erroneous data
			std::string tg = "";
			int tagLength = tag.length();
			for (int i = 0; i < tagLength; i++) {
				std::string character = tag.substr(i,1);
				if (character == ">") {
					tg += character;
					break;
				}
				tg += character;
			}
			tag = tg;

			//Makes a string of the end tag
			std::string endTag = "</" + tag.substr(1, tag.length() - 2) + ">";

			std::string searchTag = "";

			int count = 0;
			int length = line.length();

			//Starts looking for the start tag
			for (int i = 0; i < length; i++) {
				std::string character = line.substr(i,1);

				//If the next character of the data is the start of a tag
				//Reset the searchTag
				if (character == "<") {
					searchTag = "";
				}

				searchTag += line.substr(i, 1);

				//If the searchTag is equal to the desired tag then make pStart
				//Point to the start tag and break out of the loop
				if (searchTag == tag) {
					foundTag = true;
					char * start = new char[line.size() - i];
					searchTag += line.substr(i+1, line.length() - i);
					std::copy(searchTag.begin(), searchTag.end(), start);
					start[searchTag.size()] = '\0';
					pStart = start;
					break;
				}

				//If the next character in the data is the end of a tag
				//Reset the searchTag
				if (character == ">") {
					searchTag = "";
				}

				count++;
			}

			//If the start tag is not found then return fail
			if (!foundTag) {
				pEnd = 0;
				return FAIL;
			}

			//Looks for the end tag in the data
			for (int i = count; i < length; i++) {
				std::string character = line.substr(i,1);

				//If the next character equals the start of a tag
				//Reset the searchTag
				if (character == "<") {
					searchTag = "";
				}

				searchTag += line.substr(i, 1);

				//If the searchTag equals the end tag then
				//Make the pEnd point to the tag
				if (searchTag == endTag) {

					char * end = new char[searchTag.size() + 1];
					std::copy(searchTag.begin(), searchTag.end(), end);
					end[searchTag.size()] = '\0';

					pEnd = end;
					break;
				}

				//If the next character equals the end of a tag
				//Then reset the searchTag
				if (character == ">") {
					searchTag = "";
				}
			}

			return SUCCESS;
		}

		return ERROR_TAGS_NULL;
	}
}
