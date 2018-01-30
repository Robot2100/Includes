#pragma once

#ifndef ERROR_H
#define ERROR_H

#if defined(_CONSOLE)
#include <iostream>
#include <string>
namespace [[deprecated]] nsError {
	const int Warn = 1;
	const int Error = 2;
	const int Enter = 4;
	const int Exit = 8;
};
class [[deprecated]] MyError { 
#pragma warning(push)
#pragma warning( disable : 4996 )   
private:
    static void Enter() noexcept {
        std::cerr << "Press \'Enter\' to continue." << std::endl;
		std::cin.clear();
#ifdef max
#undef max
#define _undefedMax
#endif
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

#ifdef _undefedMax
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#undef _undefedMax
#endif
    }
    static void General(bool warn, bool err) noexcept {
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
    MyError(const std::string & mess, int in_flags) noexcept {
        mes = mess;
        flags = in_flags;
        w = (flags & nsError::Warn)!=0;
        e = (flags & nsError::Error)!=0;
        en = (flags & nsError::Enter)!=0;
        ex = (flags & nsError::Exit)!=0;
    }
    void Execute () noexcept {
        if(ex) Hard(mes,w,e,en);
		else Lite(mes, w, e, en);
    }
    static void BaseError(const std::string & mess, int flags) noexcept {
		using namespace nsError;
        if((flags & Exit) == 0) Lite(mess, ((flags & nsError::Warn)!=0), ((flags & nsError::Error)!=0), ((flags & nsError::Enter)!=0));
        else Hard(mess, ((flags & Warn)!=0), ((flags & nsError::Error)!=0), ((flags & nsError::Enter)!=0));
    }
    static void Lite(const std::string & mess, bool warn, bool err, bool enter) noexcept {
        General(warn, err);
        std::cerr << mess << std::endl;
        if(enter) Enter();
    }
    static void Hard(const std::string & mess, bool warn, bool err, bool enter) noexcept {
        General(warn, err);
        std::cerr << mess << std::endl;
        std::cerr << "Program will close." << std::endl;
        if(enter) Enter();
        exit(-1);
    }
    inline static void Warning(const std::string & mess) noexcept {
        Lite(mess, true, false, false);
    }
    inline static void LError(const std::string & mess) noexcept {
        Lite(mess, false, true, false);
    }
    inline static void FatalError(const std::string & mess) noexcept {
        Hard(mess, false, true, false);
    }
    inline static void Stop() noexcept {
        Enter();
    }
    inline static void StopWarning(const std::string & mess) noexcept {
        Lite(mess, true, false, true);
    }
    inline static void StopError(const std::string & mess) noexcept {
        Lite(mess, false, true, true);
    }
    inline static void StopFatalError(const std::string & mess) noexcept {
        Hard(mess, false, true, true);
    }

#pragma warning( pop ) 
};

#else // ndef _CONSOLE
#pragma message ("Warning! Error.h is usable only for console applications.")
#endif // def _CONSOLE
#endif /* end of includeonce guard: ERROR_H */
