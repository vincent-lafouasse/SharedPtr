#pragma once

#include <cstddef>

#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define GREEN "\033[0;32m"

#define PURPLE "\033[0;35m"
#define YELLOW "\033[0;33m"

#define WHITE "\033[0;37m"
#define RESET "\033[0m"

#define LOGGING 1
#if LOGGING
#include <iostream>
#define LOG(color) \
    std::clog << color << __PRETTY_FUNCTION__ << RESET << std::endl;
#endif

#include <cassert>

class RefCounter {
   public:
    typedef std::size_t SizeType;

    RefCounter() : count(NULL) {}

    RefCounter& operator=(const RefCounter& other) {
        if (*this) {
            this->decrement();
        }

        this->count = other.count;
        if (other) {
            this->increment();
        }

        return *this;
    }

    ~RefCounter() {}

    void init() {
#if LOGGING
        LOG(WHITE);
#endif
        this->count = new SizeType(1);
    }

    void increment() const {
#if LOGGING
        LOG(PURPLE);
#endif
        assert(this->count != NULL);
        *(this->count) += 1;
    }

    void decrement() {
#if LOGGING
        LOG(YELLOW);
#endif
        assert(this->count != NULL);
        *(this->count) -= 1;
        if (*(this->count) == 0) {
            delete this->count;
#if LOGGING
            std::clog << RED << "-- Deleting ref counter\n" << RESET;
#endif
            this->count = NULL;
        }
    }

    SizeType get() const { return this->count ? *this->count : 0; }

    operator bool() const { return this->count != NULL; }

   public:
    SizeType* count;
};

template <typename T>
class SharedPtr {
   public:
    typedef T ElementType;
    typedef RefCounter::SizeType SizeType;

    SharedPtr();
    SharedPtr& operator=(const SharedPtr& other);
    SharedPtr(ElementType* p);
    SharedPtr(const SharedPtr& other);
    ~SharedPtr();

    ElementType& operator*() const;
    ElementType* operator->() const;
    operator bool() const;

   public:
    void log() const;

    ElementType* self;
    RefCounter count;
};

template <typename T>
SharedPtr<T>::SharedPtr() : self(NULL), count() {
#if LOGGING
    LOG(GREEN);
#endif
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
#if LOGGING
    LOG(BLUE);
#endif

    if (this == &other) {
        return *this;
    }

    if (*this && other && this->self == other.self) {
        return *this;
    }

    if (this->count.get() == 1) {
// last reference about to get destroyed
#if LOGGING
        std::clog << RED << "-- Deleting " << static_cast<void*>(this->self)
                  << "\n"
                  << RESET;
#endif
        delete this->self;
    }

    this->self = other.self;
    this->count = other.count;

    return *this;
}

template <typename T>
SharedPtr<T>::SharedPtr(ElementType* p) : self(NULL), count() {
#if LOGGING
    LOG(GREEN);
#endif

    if (p != NULL) {
        this->self = p;
        this->count.init();
    }
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) : self(NULL), count() {
#if LOGGING
    LOG(GREEN);
#endif

    if (other) {
        *this = other;
    }
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
#if LOGGING
    LOG(RED);
#endif

    if (this->self == NULL) {
        assert(!this->count);
        return;
    }

    this->count.decrement();
    if (!this->count) {
#if LOGGING
        std::clog << RED << "-- Deleting " << static_cast<void*>(this->self)
                  << "\n"
                  << RESET;
#endif
        delete this->self;
    }
}

template <typename T>
T& SharedPtr<T>::operator*() const {
    return *self;
}

template <typename T>
T* SharedPtr<T>::operator->() const {
    return self;
}

template <typename T>
SharedPtr<T>::operator bool() const {
    return self != NULL;
}

template <typename T>
void SharedPtr<T>::log() const {
    std::clog << "SharedPtr " << this << " {\n";
    std::clog << "\tself:\t\t" << static_cast<void*>(this->self) << ",\n";
    if (self) {
        std::clog << "\tcontains:\t" << *this->self << ",\n";
    }
    std::clog << "\tref count:\t" << this->count.get() << "\n";
    std::clog << "}\n";
}
