#ifndef TELEGRAM_H_
#define TELEGRAM_H_
#include "Environment.h"
#include "./String/String.h"
#include "credential.h"

class Telegram {

public:
	Telegram ();
	Telegram (Telegram  &&) = default;
	Telegram (const Telegram  &) = default;
	Telegram  &operator=(Telegram  &&) = default;
	Telegram  &operator=(const Telegram  &) = default;
	~Telegram ();

	const CURLcode sendMsg(const String& msg);

private:
	
};



#endif // !TELEGRAM_H_
