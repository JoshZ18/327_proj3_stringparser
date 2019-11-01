/*
 * StringParserClass.cpp
 *
 *  Created on: Oct 8, 2017
 *      Author: keith
 */

#include <iostream>
#include <string>

#include "../327_proj3_test/includes/StringParserClass.h"
#include "../327_proj3_test/includes/constants.h"

namespace KP_StringParserClass{

	//dont forget to initialize member variables
	StringParserClass::StringParserClass(void) {
		//Creates char array pointers for the start and end tag
		pStartTag = new char;
		pEndTag = new char;

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
		//Runs until no start and end tag are found
		while (findTag(pStartTag, pDataToSearchThru, pEndCopy) == SUCCESS) {
			pDataToSearchThru++;
			std::string data = "";
			//Attempts to add data from char pointer until it is at the end tag
			while (*pDataToSearchThru != '<') {
				pDataToSearchThru++;

				//If the *pDataToSearchThru equals the start of the end tag
				if (*pDataToSearchThru == '<') {
					pDataToSearchThru++;
					//Makes sure that it is an end tag before adding data
					if (*pDataToSearchThru == '/') {
						myVector.push_back(data);
						break;
					}
					break;
				}

				data += *pDataToSearchThru;

				//If the *pDataToSearchThru equals the end of a tag
				//Resets the data
				if (*pDataToSearchThru == '>') {
					data = "";
				}
			}
		}

		return SUCCESS;
	}

	void StringParserClass::cleanup() {
		//If the pStartTag is not null
		//Then delete the pStartTag pointer
		if (pStartTag != 0) {
			delete pStartTag;
		}

		//If the pStartTag is not null
		//Then delete the pEndTag pointer
		if (pEndTag != 0) {
			delete pEndTag;
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
			char *tag = pTagToLookFor;
			char *pStartCopy = pStart;
			bool foundTag = false;

			while (!foundTag) {
				if (*pStart == *pTagToLookFor) {
					pStartCopy = pStart;

					while (*pTagToLookFor != '>') {
						if (*pStart != *pTagToLookFor) {
							pTagToLookFor = tag;
							break;
						}
						pStart++;
						pTagToLookFor++;
					}

					if (*pStart == '>') {
						foundTag = true;
						pStart = pStartCopy;
						pTagToLookFor = tag;
						break;
					}

					tag = pTagToLookFor;
				}

				if (*pStart == '\0') {
					pEnd = 0;
					return FAIL;
				}

				pStart++;
			}

			char *searchEnd = pStart;
			bool foundEnd = false;

			while (!foundEnd) {
				if (*searchEnd != '<') {
					char *searchEndCopy = searchEnd;
					searchEnd++;
					if (*searchEnd != '/') {
						break;
					}

					searchEnd++;
					while (*searchEnd != '>') {
						if (*searchEnd != *pTagToLookFor) {
							pTagToLookFor = tag;
							break;
						}
						searchEnd++;
						pTagToLookFor++;
					}

					if (*searchEnd == '>') {
						pEnd = searchEndCopy;
						foundEnd = true;
						break;
					}

					tag = pTagToLookFor;
				}

				searchEnd++;
			}

			return SUCCESS;
		}

		return ERROR_TAGS_NULL;
	}
}
