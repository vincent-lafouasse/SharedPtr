#pragma once

#include <cstddef>

#define LOGGING 1
#if LOGGING
#include <iostream>

#define LOG() \
    std::clog << __PRETTY_FUNCTION__ << std::endl;

#endif

template <typename T>
class SharedPtr {
   public:
    SharedPtr() {
        #if LOGGING
        LOG();
        #endif

        self = NULL;
        count = NULL;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        #if LOGGING
        LOG();
        #endif

        if (this == &other) {
            return *this;
        }

        if (other.self == NULL) {
            self = NULL;
            count = NULL;
        } else {
            self = other.self;
            *count += 1;
        }
        return *this;
    }

    SharedPtr(T* p) {
        #if LOGGING
        LOG();
        #endif

        if (p == NULL) {
            *this = SharedPtr();
        } else {
            self = p;
            count = new std::size_t(1);
        }
    }

    SharedPtr(const SharedPtr& other) {
        #if LOGGING
        LOG();
        #endif

        if (other.self == NULL) {
            *this = SharedPtr();
        } else {
            *this = other;
        }
    }

    ~SharedPtr() {
        #if LOGGING
        LOG();
        #endif

        if (self == NULL) {
            return;
        }

        *count -= 1;

        if (*count == 0) {
            delete self;
            delete count;
        }
    }

   private:
    T* self;
    std::size_t* count;
};
