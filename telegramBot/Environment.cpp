#include <iostream>
#include "Environment.h"

using namespace std;

Environment E;

Environment::Environment() {
	m_curl = curl_easy_init();
}

Environment::~Environment() {
	curl_easy_cleanup(m_curl);
}

Environment& Environment::setOption(CURLoption optionMacro, const char* url) {
	curl_easy_setopt(m_curl, optionMacro, url);
	return *this;
}

const CURLcode& Environment::execute() {
	m_result = curl_easy_perform(m_curl);
	return m_result;
}

const CURLcode& Environment::fetchCode() const {
	return m_result;
}
