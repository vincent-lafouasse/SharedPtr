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
        self = NULL;
        count = NULL;
        return *this;
    }

    self = other.self;
    count = other.count;
    *count += 1;
    return *this;
}

template <typename T>
SharedPtr<T>::SharedPtr(ElementType* p) {
#if LOGGING
    LOG();
#endif

    if (p == NULL) {
        *this = SharedPtr();
    } else {
        self = p;
        count = new SizeType(1);
    }
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) {
#if LOGGING
    LOG();
#endif

    if (other.self == NULL) {
        *this = SharedPtr();
    } else {
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
