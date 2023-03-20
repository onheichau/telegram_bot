#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <curl/curl.h>

class Environment {
private:
	CURL* m_curl{};
	CURLcode m_result{};

public:
	Environment();
	Environment(const Environment& source) = delete;
	Environment operator=(const Environment& source) = delete;
	~Environment();
	Environment& setOption(CURLoption optionMacro, const char* url);
	const CURLcode& execute();

	const CURLcode& fetchCode() const;
};

extern Environment E;

#endif // !ENVIRONMENT_H
