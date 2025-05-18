#pragma once

#include <cstddef>

#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define GREEN "\033[0;32m"

#define PURPLE "\033[0;35m"
#define YELLOW "\033[0;33m"

#define WHITE "\033[0;37m"
#define RESET "\033[0m"

#define LOGGING 0
#if LOGGING
#include <iostream>
#endif

#if LOGGING
#define LOG_FUNCTION_NAME(color) \
    std::clog << color << __PRETTY_FUNCTION__ << RESET << std::endl;
#define LOG_DATA_DELETION(ptr)                                            \
    std::clog << RED << "-- Deleting " << static_cast<void*>(ptr) << "\n" \
              << RESET;
#define LOG_REFCOUNTER_DELETION() \
    std::clog << RED << "-- Deleting ref counter\n" << RESET;
#else  // !LOGGING
#define LOG_FUNCTION_NAME(color) ;
#define LOG_DATA_DELETION(ptr) ;
#define LOG_REFCOUNTER_DELETION() ;
#endif  // LOGGING

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
        LOG_FUNCTION_NAME(WHITE);

        this->count = new SizeType(1);
    }

    void increment() const {
        LOG_FUNCTION_NAME(PURPLE);

        assert(this->count != NULL);
        *(this->count) += 1;
    }

    void decrement() {
        LOG_FUNCTION_NAME(YELLOW);

        assert(this->count != NULL);
        *(this->count) -= 1;
        if (*(this->count) == 0) {
            LOG_REFCOUNTER_DELETION();
            delete this->count;
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
    explicit SharedPtr(ElementType* p);
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
    LOG_FUNCTION_NAME(GREEN);
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
    LOG_FUNCTION_NAME(BLUE);

    if (this == &other) {
        return *this;
    }

    if (*this && other && this->self == other.self) {
        return *this;
    }

    if (this->count.get() == 1) {
        // last reference about to get destroyed
        LOG_DATA_DELETION(this->self);
        delete this->self;
    }

    this->self = other.self;
    this->count = other.count;

    return *this;
}

template <typename T>
SharedPtr<T>::SharedPtr(ElementType* p) : self(NULL), count() {
    LOG_FUNCTION_NAME(GREEN);

    if (p != NULL) {
        this->self = p;
        this->count.init();
    }
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) : self(NULL), count() {
    LOG_FUNCTION_NAME(GREEN);

    if (other) {
        *this = other;
    }
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
    LOG_FUNCTION_NAME(RED);

    if (this->self == NULL) {
        assert(!this->count);
        return;
    }

    this->count.decrement();
    if (!this->count) {
        LOG_DATA_DELETION(this->self);
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
    (void)this;
#if LOGGING
    std::clog << "SharedPtr " << this << " {\n";
    std::clog << "\tself:\t\t" << static_cast<void*>(this->self) << ",\n";
    if (self) {
        std::clog << "\tcontains:\t" << *this->self << ",\n";
    }
    std::clog << "\tref count:\t" << this->count.get() << "\n";
    std::clog << "}\n";
#endif
}
