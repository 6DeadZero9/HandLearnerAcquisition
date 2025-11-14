#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <memory>

template<typename T> class Singleton {
protected:
    Singleton() {}

    static std::unique_ptr<T>& get_instance_ptr() {
        static std::unique_ptr<T> m_instance_ptr;
        return m_instance_ptr;
    }

public:
    static T& instance() {
        if (get_instance_ptr() == nullptr) get_instance_ptr() = std::unique_ptr<T> { new T };
        return *get_instance_ptr();
    }

    static void reset() {
        if (get_instance_ptr() != nullptr) get_instance_ptr().reset();
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;
    Singleton& operator=(const Singleton) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

#endif /* SINGLETON_H_ */
