#pragma once

#include <cstddef>

#define LOGGING 1
#if LOGGING
#include <iostream>

#define LOG() std::clog << __PRETTY_FUNCTION__ << std::endl;

#endif

template <typename T>
class SharedPtr {
   public:
    typedef T ElementType;
    typedef std::size_t SizeType;

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
    SizeType* count;
};

template <typename T>
SharedPtr<T>::SharedPtr() {
#if LOGGING
    LOG();
#endif

    self = NULL;
    count = NULL;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
#if LOGGING
    LOG();
#endif

    if (this == &other) {
        return *this;
    }

    if (other.self == NULL) {
        if (this->self == NULL) {
            // this should already be zero'd
            // this is a no-op
            return *this;
        }

        assert(this->count != NULL);
        *(this->count) -= 1;

        if (*(this->count) == 0) {
            delete self;
            delete count;
        }

        this->self = NULL;
        this->count = NULL;
        return *this;
    }

    assert(other.self != NULL);
    assert(other.count != NULL);

    if (this->self == other.self) {
        return *this;
    }

    if (this->self != NULL) {
        // optionally overwrite this if active
        assert(this->count != NULL);
        *(this->count) -= 1;

        if (*(this->count) == 0) {
            delete self;
            delete count;
        }
    }

    this->self = other.self;
    this->count = other.count;
    *(this->count) += 1;

    return *this;
}

template <typename T>
SharedPtr<T>::SharedPtr(ElementType* p) {
#if LOGGING
    LOG();
#endif

    if (p == NULL) {
        this->self = NULL;
        this->count = NULL;
    } else {
        this->self = p;
        this->count = new SizeType(1);
    }
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) {
#if LOGGING
    LOG();
#endif

    this->self = NULL;
    this->count = NULL;

    if (other.self) {
        *this = other;
    }
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
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
    std::clog << "\tref count:\t" << (this->count ? *this->count : 0) << "\n";
    std::clog << "}\n";
}
