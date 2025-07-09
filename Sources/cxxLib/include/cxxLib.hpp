#ifndef CXX_LIB_HPP
#define CXX_LIB_HPP

#include <string>
#include <optional>
#include <iostream>
#include <cassert>

#include <swift/bridging>

template<class T>
concept SwiftSharable = requires(T t) {
    { t.swift_retain() } -> std::same_as<void>;
    { t.swift_release() } -> std::same_as<void>;
};

template<SwiftSharable T>
inline void swift_retain(T* object) {
    std::cout << std::format("Retaining object of type {}", typeid(T).name()) << std::endl;
    object->swift_retain();
}

template<SwiftSharable T>
inline void swift_release(T* object) {
    std::cout << std::format("Releasing object of type {}", typeid(T).name()) << std::endl;
    object->swift_release();
}

template<class T>
class SwiftSharedBase : public std::enable_shared_from_this<T> {
    std::atomic<unsigned> m_swift_ref_count = 0;
    std::optional<std::shared_ptr<T>> m_swift_lock = std::nullopt;
public:
    inline void swift_retain() {
        assert(m_swift_ref_count < std::numeric_limits<unsigned>::max() && "Ref count overflow");
        m_swift_ref_count += 1;
        
        if (!m_swift_lock) {
            m_swift_lock = this->shared_from_this();
        }
    }
    
    inline void swift_release() {
        if (m_swift_ref_count > 0) {
            m_swift_ref_count -= 1;
        }
        
        if (m_swift_ref_count == 0) {
            m_swift_lock = std::nullopt;
        }
    }
};

class TestClass: public SwiftSharedBase<TestClass> {
public:
    std::string message;
    
    TestClass(const std::string& message): message(message) {}
    TestClass(const TestClass&) = delete; // Disable copy constructor
    
    static TestClass* swift_create(const std::string& message) SWIFT_RETURNS_RETAINED {
        auto p = std::make_shared<TestClass>(message);
        p->swift_retain();
        return p.get();
    }
} SWIFT_SHARED_REFERENCE(swift_retain_test_class, swift_release_test_class);

void swift_retain_test_class(TestClass* object) {
    swift_retain(object);
}
void swift_release_test_class(TestClass* object) {
    swift_release(object);
}

#endif // CXX_LIB_HPP
