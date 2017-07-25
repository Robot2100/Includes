#pragma once

#ifndef ERROR_H
#define ERROR_H

#ifdef _CONSOLE
#include <iostream>
#include <string>

namespace nsError {
	const int WARN = 1;
	const int ERROR = 2;
	const int ENTER = 4;
	const int EXIT = 8;
}
class Error {
private:
    static void Enter() {
        std::cerr << "Press \'Enter\' to continue." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        //std::cin.get();
    }
    static void General(bool warn, bool err) {
        if(warn && err) Hard("Using BOTH flags of Error and Warning.",false,true,false);
        if(warn)
            std::cerr << "Warning! ";
        else if(err)
            std::cerr << "Error! ";
    }
public:
    bool w,e,en,ex;
    std::string mes;
    int flags;
    Error(const std::string & mess, int in_flags) {
		using namespace nsError;
        mes = mess;
        flags = in_flags;
        w = (flags & WARN)!=0;
        e = (flags & ERROR)!=0;
        en = (flags & ENTER)!=0;
        ex = (flags & EXIT)!=0;
    }
    void Execute () {
        if(ex) Hard(mes,w,e,en);
		else Lite(mes, w, e, en);
    }
    static void BaseError(const std::string & mess, int flags) {
		using namespace nsError;
        if((flags & EXIT) == 0) Lite(mess, ((flags & WARN)!=0), ((flags & ERROR)!=0), ((flags & ENTER)!=0));
        else Hard(mess, ((flags & WARN)!=0), ((flags & ERROR)!=0), ((flags & ENTER)!=0));
    }
    static void Lite(const std::string & mess, bool warn, bool err, bool enter) {
        General(warn, err);
        std::cerr << mess << std::endl;
        if(enter) Enter();
    }
    static void Hard(const std::string & mess, bool warn, bool err, bool enter) {
        General(warn, err);
        std::cerr << mess << std::endl;
        std::cerr << "Program will close." << std::endl;
        if(enter) Enter();
        exit(-1);
    }
    inline static void Warning(const std::string & mess) {
        Lite(mess, true, false, false);
    }
    inline static void LError(const std::string & mess) {
        Lite(mess, false, true, false);
    }
    inline static void FatalError(const std::string & mess) {
        Hard(mess, false, true, false);
    }
    inline static void Stop() {
        Enter();
    }
    inline static void StopWarning(const std::string & mess) {
        Lite(mess, true, false, true);
    }
    inline static void StopError(const std::string & mess) {
        Lite(mess, false, true, true);
    }
    inline static void StopFatalError(const std::string & mess) {
        Hard(mess, false, true, true);
    }
};

#else // ndef _CONSOLE
#pragma message ("Warning! Error.h is usable only for console applications.")
#endif // def _CONSOLE
#endif /* end of includeonce guard: ERROR_H */
