#pragma once

#define CHARBUFF 124


bool readChar(const char *section, const char *keyword, const char *defaultValue, char *returnValue, const char *filePath);
double readDouble(const char *section, const char *keyword, double defaultValue, const char *filePath);
void getGurrentDirectory(char *currentDirectory);


bool writeChar(const char *section, const char *keyword, const char *returnValue, const char *filePath);
bool writeInt(const char *section, const char *keyword, int returnValue, const char *filePath);
bool writeDouble(const char *section, const char *keyword, double returnValue, const char *filePath);



int readInt(const char* section, const char* keyword, int defaultValue, const char* filePath) {
	char valueString[CHARBUFF];
	if (readChar(section, keyword, "", valueString, filePath)) {
		return atof(valueString);//文字列をdouble型で返す
	}
	else {
		return defaultValue;
	}
}

double readDouble(const char* section, const char* keyword, double defaultValue, const char* filePath) {
	char valueString[CHARBUFF];
	char* endptr;
	if (readChar(section, keyword, "", valueString, filePath)) {
		return strtod(valueString, &endptr);
	}
	else {
		return defaultValue;
	}
}
bool writeInt(const char* section, const char* keyword, int returnValue, const char* filePath) {
	char valueChar[CHARBUFF];
	sprintf_s(valueChar, "%d", returnValue);
	return writeChar(section, keyword, valueChar, filePath);
}
bool writeDouble(const char* section, const char* keyword, double returnValue, const char* filePath) {
	char valueChar[CHARBUFF];
	sprintf_s(valueChar, "%lf", returnValue);
	return writeChar(section, keyword, valueChar, filePath);
}


bool  readChar(const char* section, const char* keyword, const char* defaultValue, char* returnValue, const char* filePath) {

	if (GetPrivateProfileString(TEXT(section), TEXT(keyword), TEXT(defaultValue), TEXT(returnValue), CHARBUFF, TEXT(filePath)) != 0) {
		//セクション名、キー名、既定の文字列（キー名が未定義のときにこの文字列が格納され）、バッファのサイズ、ファイル名
		return true;
	}
	else {
		return false;
	}
}

bool writeChar(const char* section, const char* keyword, const char* returnValue, const char* filePath) {
	return WritePrivateProfileString(section, keyword, returnValue, filePath);
}

void getGurrentDirectory(char* currentDirectory) {
	GetCurrentDirectory(CHARBUFF, currentDirectory);
}