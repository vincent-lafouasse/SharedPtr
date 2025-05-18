#pragma once

#include <cstddef>

#define LOGGING 1
#if LOGGING
#include <iostream>
#define LOG() std::clog << __PRETTY_FUNCTION__ << std::endl;
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

    ~RefCounter() {
        if (*this) {
            this->decrement();
        }
    }

    void init() {
#if LOGGING
        LOG();
#endif
        this->count = new SizeType(1);
    }

    void increment() {
#if LOGGING
        LOG();
#endif
        assert(this->count != NULL);
        *(this->count) += 1;
    }

    void decrement() {
#if LOGGING
        LOG();
#endif
        assert(this->count != NULL);
        *(this->count) -= 1;
        if (*(this->count) == 0) {
            delete this->count;
            this->count = NULL;
        }
    }

    SizeType get() const {
        return this->count ? *this->count : 0;
    }

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
    LOG();
#endif
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
#if LOGGING
    LOG();
#endif

    if (this == &other) {
        return *this;
    }

    if (*this && other && this->self == other.self) {
        return *this;
    }

    if (this->count.get() == 1) {
        // last reference about to get destroyed
        delete this->self;
    }

    this->self = other.self;
    this->count = other.count;

    return *this;
}

template <typename T>
SharedPtr<T>::SharedPtr(ElementType* p) : self(NULL), count() {
#if LOGGING
    LOG();
#endif

    if (p != NULL) {
        this->self = p;
        this->count.init();
    }
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) : self(NULL), count() {
#if LOGGING
    LOG();
#endif

    if (other) {
        *this = other;
    }
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
#if LOGGING
    LOG();
#endif

    if (this->self == NULL) {
        assert(!this->count);
        return;
    }

    this->count.decrement();
    if (!this->count) {
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
    std::clog << "\tself:\t\t" << (void*)this->self << ",\n";
    if (self) {
        std::clog << "\tcontains:\t" << *this->self << ",\n";
    }
    std::clog << "\tref count:\t" << this->count.get() << "\n";
    std::clog << "}\n";
}
