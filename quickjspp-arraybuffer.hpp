#pragma once

/* Include main header if not included */
#include "quickjs/quickjspp.hpp"

/* Extension to support ArrayBuffer and typed buffer classes in QuickJS */
namespace qjs {

/** Special allocator to "move" the original ArrayBuffer pointer into the 
 *  vector memory space without allocating any new memory
 */
template<typename T>
class ArrayBufferMoveAllocator
{
private:
    T* memory_ptr;
    size_t memory_size;

public:
    using size_type = size_t;
    using pointer = T*;
    using value_type = T;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    ArrayBufferMoveAllocator(T* memory_ptr, std::size_t memory_size) noexcept : memory_ptr(memory_ptr), memory_size(memory_size) {}

    ArrayBufferMoveAllocator(const ArrayBufferMoveAllocator& other) noexcept : memory_ptr(other.memory_ptr), memory_size(other.memory_size) {};

    template<typename U>
    ArrayBufferMoveAllocator(const ArrayBufferMoveAllocator<U>& other) noexcept : memory_ptr(other.memory_ptr), memory_size(other.memory_size) {};

    template<typename U>
    ArrayBufferMoveAllocator& operator = (const ArrayBufferMoveAllocator<U>& other) { return *this; }
    ArrayBufferMoveAllocator<T>& operator = (const ArrayBufferMoveAllocator& other) { return *this; }
    ~ArrayBufferMoveAllocator() = default;


    pointer allocate(size_type n, const void* hint = 0) {return memory_ptr;}
    void deallocate(T* ptr, size_type n) {}

    size_type max_size() const {return memory_size;}
};

/** Type wrapper to use for ArrayBuffer object
 */
template <typename Allocator = ArrayBufferMoveAllocator<uint8_t>>
struct NonTypedArray : std::vector<uint8_t, Allocator>
{
    NonTypedArray(const Allocator& alloc) : std::vector<uint8_t, Allocator>(alloc) {}
};
using ArrayBuffer = NonTypedArray<ArrayBufferMoveAllocator<uint8_t>>;

/** Type wrapper to use for TypedArray object
 */
template <typename T, typename Allocator = ArrayBufferMoveAllocator<T>>
struct TypedArray : std::vector<T, Allocator> {};

template <typename Allocator>
struct TypedArray<uint8_t, Allocator> : std::vector<uint8_t, Allocator> {
    TypedArray(const Allocator& alloc) : std::vector<uint8_t, Allocator>(alloc) {}

    static constexpr size_t bytesPerElement = sizeof(uint8_t);
    static constexpr const char* className = "Uint8Array";
};
using Uint8Array = TypedArray<uint8_t, ArrayBufferMoveAllocator<uint8_t>>;
using Uint8ClampedArray = TypedArray<uint8_t, ArrayBufferMoveAllocator<uint8_t>>;

template <typename Allocator>
struct TypedArray<int8_t, Allocator> : std::vector<int8_t, Allocator> {
    TypedArray(const Allocator& alloc) : std::vector<int8_t, Allocator>(alloc) {}

    static constexpr size_t bytesPerElement = sizeof(int8_t);
    static constexpr const char* className = "Int8Array";
};
using Int8Array = TypedArray<int8_t, ArrayBufferMoveAllocator<int8_t>>;

template <typename Allocator>
struct TypedArray<int16_t, Allocator> : std::vector<int16_t, Allocator> {
    TypedArray(const Allocator& alloc) : std::vector<int16_t, Allocator>(alloc) {}

    static constexpr size_t bytesPerElement = sizeof(int16_t);
    static constexpr const char* className = "Int16Array";
};
using Int16Array = TypedArray<int16_t, ArrayBufferMoveAllocator<int16_t>>;

template <typename Allocator>
struct TypedArray<uint16_t, Allocator> : std::vector<uint16_t, Allocator> {
    TypedArray(const Allocator& alloc) : std::vector<uint16_t, Allocator>(alloc) {}

    static constexpr size_t bytesPerElement = sizeof(uint16_t);
    static constexpr const char* className = "Uint16Array";
};
using Uint16Array = TypedArray<uint16_t, ArrayBufferMoveAllocator<uint16_t>>;

template <typename Allocator>
struct TypedArray<int32_t, Allocator> : std::vector<int32_t, Allocator> {
    TypedArray(const Allocator& alloc) : std::vector<int32_t, Allocator>(alloc) {}
    
    static constexpr size_t bytesPerElement = sizeof(int32_t);
    static constexpr const char* className = "Int32Array";
};
using Int32Array = TypedArray<int32_t, ArrayBufferMoveAllocator<int32_t>>;

template <typename Allocator>
struct TypedArray<uint32_t, Allocator> : std::vector<uint32_t, Allocator> {
    TypedArray(const Allocator& alloc) : std::vector<uint32_t, Allocator>(alloc) {}

    static constexpr size_t bytesPerElement = sizeof(uint32_t);
    static constexpr const char* className = "Uint32Array";
};
using Uint32Array = TypedArray<uint32_t, ArrayBufferMoveAllocator<uint32_t>>;

template <typename Allocator>
struct TypedArray<float, Allocator> : std::vector<float, Allocator> {
    TypedArray(const Allocator& alloc) : std::vector<float, Allocator>(alloc) {}

    static constexpr size_t bytesPerElement = sizeof(float);
    static constexpr const char* className = "Float32Array";
};
using Float32Array = TypedArray<float, ArrayBufferMoveAllocator<float>>;

template <typename Allocator>
struct TypedArray<double, Allocator> : std::vector<double, Allocator> {
    TypedArray(const Allocator& alloc) : std::vector<double, Allocator>(alloc) {}

    static constexpr size_t bytesPerElement = sizeof(double);
    static constexpr const char* className = "Float64Array";
};
using Float64Array = TypedArray<double, ArrayBufferMoveAllocator<double>>;

template <typename Allocator>
struct TypedArray<int64_t, Allocator> : std::vector<int64_t, Allocator> {
    TypedArray(const Allocator& alloc) : std::vector<int64_t, Allocator>(alloc) {}

    static constexpr size_t bytesPerElement = sizeof(int64_t);
    static constexpr const char* className = "BigInt64Array";
};
using BigInt64Array = TypedArray<int64_t, ArrayBufferMoveAllocator<int64_t>>;

template <typename Allocator>
struct TypedArray<uint64_t, Allocator> : std::vector<uint64_t, Allocator> {
    TypedArray(const Allocator& alloc) : std::vector<uint64_t, Allocator>(alloc) {}

    static constexpr size_t bytesPerElement = sizeof(uint64_t);
    static constexpr const char* className = "BigUint64Array";
};
using BigUint64Array = TypedArray<uint64_t, ArrayBufferMoveAllocator<uint64_t>>;

template <typename T>
struct memory_traits {};

template <typename T, typename Allocator>
struct memory_traits<TypedArray<T, Allocator>>
{
    using in = TypedArray<T, ArrayBufferMoveAllocator<T>>;
    using out = TypedArray<T, std::allocator<T>>;
};

template <typename Allocator>
struct memory_traits<NonTypedArray<Allocator>>
{
    using in = NonTypedArray<ArrayBufferMoveAllocator<uint8_t>>;
    using out = NonTypedArray<std::allocator<uint8_t>>;
};

/* In and out wrappers for ArrayBuffer types */
template <typename T>
using in = typename memory_traits<T>::in;

template <typename T>
using out = typename memory_traits<T>::out;

/** Special conversion from TypedArray to std::vector<T>. If Array holds objects that are non-convertible to T throws qjs::exception */
template <typename T>
struct js_traits<TypedArray<T, ArrayBufferMoveAllocator<T>>>
{
    using InTypedArray = TypedArray<T, ArrayBufferMoveAllocator<T>>;

    static InTypedArray unwrap(JSContext * ctx, JSValueConst jsarr)
    {
        size_t psize;
        size_t pbytes_per_element;
        JSValue val = JS_GetTypedArrayBuffer(ctx, jsarr, NULL, NULL, &pbytes_per_element);
        if(JS_IsException(val))
            JS_ThrowTypeError(ctx, "js_traits<%s>::unwrap expects %s object", TypedArray<T>::className, TypedArray<T>::className);
        if(pbytes_per_element != TypedArray<T>::bytesPerElement) {
            JS_FreeValue(ctx, val);
            JS_ThrowTypeError(ctx, "js_traits<%s>::unwrap expects a buffer of size %d", TypedArray<T>::className, TypedArray<T>::bytesPerElement);
        }
        T* rawBuffer = reinterpret_cast<T*>(JS_GetArrayBuffer(ctx, &psize, val));
        auto len = static_cast<int32_t>(psize / pbytes_per_element);
        JS_FreeValue(ctx, val);
        InTypedArray arr{ArrayBufferMoveAllocator<T>(rawBuffer, len)};
        arr.assign(rawBuffer, rawBuffer + len);
        return arr;
    }

    static JSValue wrap(JSContext * ctx, const InTypedArray& arr) {
        /* Need for variant to wrap */
        return JS_EXCEPTION;
    }
};

/** Special conversion from ArrayBuffer to std::vector<T>. If Array holds objects that are non-convertible to T throws qjs::exception */
template <>
struct js_traits<NonTypedArray<ArrayBufferMoveAllocator<uint8_t>>>
{
    using InArrayBuffer = NonTypedArray<ArrayBufferMoveAllocator<uint8_t>>;
    using InAllocator = ArrayBufferMoveAllocator<uint8_t>;

    static InArrayBuffer unwrap(JSContext * ctx, JSValueConst jsarr)
    {
        size_t psize;
        uint8_t* rawBuffer = JS_GetArrayBuffer(ctx, &psize, jsarr);
        auto len = static_cast<int32_t>(psize);
        if(len == 0 && !rawBuffer) {
            JS_ThrowTypeError(ctx, "js_traits<ArrayBuffer>::unwrap expects ArrayBuffer object");
        }
        InArrayBuffer arr{InAllocator(rawBuffer, len)};
        arr.assign(rawBuffer, rawBuffer + len);
        return arr;
    }

    static JSValue wrap(JSContext * ctx, const InArrayBuffer& arr) {
        /* Need for variant to wrap */
        return JS_EXCEPTION;
    }
};

/** Special conversion from std::vector<T> ro ArrayBuffer. If Array holds objects that are non-convertible to T throws qjs::exception */
template <typename Allocator>
struct js_traits<NonTypedArray<Allocator>>
{
    using OutArrayBuffer = NonTypedArray<Allocator>;

    static OutArrayBuffer unwrap(JSContext * ctx, JSValueConst jsarr) {
        JS_ThrowTypeError(ctx, "Cannot transform out ArrayBuffer to native object");
    }

    static JSValue wrap(JSContext * ctx, const OutArrayBuffer& arr) noexcept {
        return JS_NewArrayBufferCopy(ctx, arr.data(), arr.size());;
    }
};

/** Traits for std::variant including an ArrayBuffer */
template <typename ... Ts>
struct variant_traits<NonTypedArray<ArrayBufferMoveAllocator<uint8_t>>, Ts...>
{
    using MyArrayBuffer = NonTypedArray<ArrayBufferMoveAllocator<uint8_t>>;

    /** Attempt to match class ID with type */
    template <typename U, typename ... uArgs>
    static std::optional<std::variant<U, uArgs...>> unwrapObj(JSContext * ctx, JSValueConst v, JSClassID class_id)
    {
        size_t psize;
        uint8_t* buffer = JS_GetArrayBuffer(ctx, &psize, v);
        if(buffer)
        {
            return js_traits<MyArrayBuffer>::unwrap(ctx, v);
        }

        if constexpr ((sizeof ... (Ts)) > 0)
        {
            return variant_traits<Ts...>::template unwrapObj<U, uArgs...>(ctx, v, class_id);
        }
        return std::nullopt;
    }
};

/** Traits for std::variant including an TypedArray */
template <typename T, typename ... Ts>
struct variant_traits<TypedArray<T, ArrayBufferMoveAllocator<T>>, Ts...>
{
    using VariantTypedArray = std::variant<TypedArray<T, ArrayBufferMoveAllocator<T>>, Ts...>;
    using MyTypedArray = TypedArray<T, ArrayBufferMoveAllocator<T>>;

    /** Attempt to match class ID with type */
    template <typename U, typename ... uArgs>
    static std::optional<std::variant<U, uArgs...>> unwrapObj(JSContext * ctx, JSValueConst v, JSClassID class_id)
    {
        size_t bytesPerElement;
        JSValue buffer = JS_GetTypedArrayBuffer(ctx, v, NULL, NULL, &bytesPerElement);
        if(!JS_IsException(buffer))
        {
            JS_FreeValue(ctx, buffer);
            if(bytesPerElement == MyTypedArray::bytesPerElement)
                return js_traits<MyTypedArray>::unwrap(ctx, v);
        }

        if constexpr ((sizeof ... (Ts)) > 0)
        {
            return variant_traits<Ts...>::template unwrapObj<U, uArgs...>(ctx, v, class_id);
        }
        return std::nullopt;
    }
};
}