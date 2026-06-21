#ifndef ASSERT_HH
#define ASSERT_HH

#include <cstdio>
#include <exception>

class AssertError : public std::exception {
    const char *expr_;
    char *message_;

 public:
    AssertError(const char *expr, const char *file, int line, const char *reason) : expr_(expr) {
        message_ = new char[4096];
#ifdef DOCTEST_LIBRARY_INCLUDED
        sprintf(message_, "%s", reason);
#else
        sprintf(message_, "%s:%d: -> %s <-", file, line, reason);
#endif
    }

    ~AssertError() {
        delete message_;
    }

    const char *what() const noexcept {
        return message_;
    }
};

#endif

#ifdef assert
#undef assert
#endif

#define assert(expr, reason) \
    (static_cast<bool>(expr) ? void(0) : throw AssertError(#expr, __FILE__, __LINE__, reason))
