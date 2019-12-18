#pragma once
#include <limits>
#include <iterator>
#include <cstddef>

template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    Allocator noexept() {}
    ~Allocator() {}
    pointer allocate(size_type);
    void deallocate(pointer, size_type);
    template<class... Args>
    void construct(pointer, Args&&...);
    void destroy(pointer);
    size_type max_size();
};

template<class T>
typename Allocator<T>::pointer Allocator<T>::allocate(size_type n)
{
    return static_cast<pointer>(operator new [](n*sizeof(value_type)));
}

template<class T>
void Allocator<T>::deallocate(pointer ptr, size_type n)
{
    operator delete[](ptr);
}

template<class T>
template<class... Args>
void Allocator<T>::construct(pointer ptr, Args&&... args)
{
    new(ptr) value_type(std::forward<Args>(args)...);
}

template<class T>
void Allocator<T>::destroy(pointer ptr)
{
    static_cast<pointer>(ptr)->~T();
}

template<class T>
typename Allocator<T>::size_type Allocator<T>::max_size()
{
    return std::numeric_limits<size_type>::max();
}

template <class T>
class Iterator
    : public std::iterator<std::random_access_iterator_tag, T>
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = Iterator<T>;
    using difference = std::ptrdiff_t;

private:
    pointer ptr_;

public:
    Iterator(pointer ptr) : ptr_(ptr) {}
    operator pointer() {return ptr_;}
    operator const_pointer() const {return ptr_;}
    reference operator* () {return *ptr_;}
    const_reference operator* () const {return *ptr_;}

    reference operator[] (difference n) {return *(ptr_ + n);}
    const_reference operator[] (difference n) const {return *(ptr_ + n);}
    
    iterator& operator++()
    {
        ptr_++;
        return *this;
    }
    iterator operator++(int)
    {
        iterator temp(ptr_);
        ptr_++;
        return temp;
    }
    iterator& operator--()
    {
        ptr_--;
        return *this;
    }
    iterator operator--(int)
    {
        iterator temp(ptr_);
        ptr_--;
        return temp;
    }

    iterator& operator+= (difference n)
    {
        ptr_ += n;
        return *this;
    }
    iterator& operator-= (difference n)
    {
        ptr_ -= n;
        return *this;
    }
    
    iterator operator+ (difference n) const {return iterator(ptr_ + n);}
    iterator operator- (difference n) const {return iterator(ptr_ - n);}
    iterator::difference operator+ (const iterator& it) const {return ptr_ + it.ptr_;}
    iterator::difference operator- (const iterator& it) const {return ptr_ - it.ptr_;}
    bool operator== (const iterator& it) const {return ptr_ == it.ptr_;}
    bool operator!= (const iterator& it) const {return ptr_ != it.ptr_;}
    bool operator< (const iterator& it) const {return ptr_ < it.ptr_;}
    bool operator> (const iterator& it) const {return ptr_ > it.ptr_;}
    bool operator<= (const iterator& it) const {return ptr_ <= it.ptr_;}
    bool operator>= (const iterator& it) const {return ptr_ >= it.ptr_;}
};

template <class T, class Alloc=Allocator<T>>
class Vector
{
    const static std::size_t base_size = 32;

    Alloc alloc_;
    std::size_t mem_size_;
    T* data_;
public:
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;

private:
    iterator begin_, end_;

public:
    Vector();
    Vector(std::size_t, const T& =T());
    ~Vector();
    T& operator[] (std::ptrdiff_t);
    const T& operator[] (std::ptrdiff_t) const;
    void push_back(const T&);
    void push_back(T&&);
    void pop_back();
    bool empty() const;
    std::size_t size() const;
    std::size_t capacity() const;
    void clear();
    iterator begin();
    const iterator begin() const;
    iterator end();
    const iterator end() const;
    reverse_iterator rbegin();
    const reverse_iterator rbegin() const;
    reverse_iterator rend();
    const reverse_iterator rend() const;
    void resize(std::size_t, const T& =T());
    void reserve(std::size_t);
};

template<class T, class Alloc>
Vector<T, Alloc>::Vector()
    : mem_size_(base_size)
    , data_(alloc_.allocate(mem_size_))
    , begin_(data_), end_(data_)
{}

template<class T, class Alloc>
Vector<T, Alloc>::Vector(std::size_t size, const T& default_value)
    : mem_size_(base_size > size ? base_size : size)
    , data_(alloc_.allocate(mem_size_))
    , begin_(data_), end_(data_ + size)
{
    for(T* ptr = data_; ptr < data_ + size; ++ptr) {
        alloc_.construct(ptr, default_value);
    }
}

template<class T, class Alloc>
Vector<T, Alloc>::~Vector() {
    for(T* ptr = data_; ptr < data_ + size(); ++ptr) {
        alloc_.destroy(ptr);
    }
    alloc_.deallocate(data_, mem_size_);
}

template<class T, class Alloc>
T& Vector<T, Alloc>::operator[](std::ptrdiff_t i) {
    return data_[i];
}

template<class T, class Alloc>
const T& Vector<T, Alloc>::operator[](std::ptrdiff_t i) const {
    return data_[i];
}


template<class T, class Alloc>
void Vector<T, Alloc>::push_back(const T& value)
{
    if (size() >= mem_size_) {
        reserve(mem_size_*2 + 1);
    }
    alloc_.construct(end_, value);
    ++end_;
}

template<class T, class Alloc>
void Vector<T, Alloc>::push_back(T&& value)
{
    if (size() >= mem_size_) {
        reserve(mem_size_*2 + 1);
    }
    alloc_.construct(end_, std::forward<T>(value));
    ++end_;
}

template<class T, class Alloc>
void Vector<T, Alloc>::pop_back() {
    if (size() > 0)
    {
        --end_;
        alloc_.destroy(end_);
    }
}

template<class T, class Alloc>
bool Vector<T, Alloc>::empty() const {
    return begin_ == end_;
}

template<class T, class Alloc>
std::size_t Vector<T, Alloc>::size() const {
    return end_ - begin_;
}

template<class T, class Alloc>
std::size_t Vector<T, Alloc>::capacity() const
{
    return mem_size_;
}

template<class T, class Alloc>
void Vector<T, Alloc>::clear()
{
    resize(0);
}

template<class T, class Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::begin()
{
    return begin_;
}

template<class T, class Alloc>
const typename Vector<T, Alloc>::iterator Vector<T, Alloc>::begin() const
{
    return begin_;
}

template<class T, class Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::end()
{
    return end_;
}

template<class T, class Alloc>
const typename Vector<T, Alloc>::iterator Vector<T, Alloc>::end() const
{
    return end_;
}

template<class T, class Alloc>
typename Vector<T, Alloc>::reverse_iterator Vector<T, Alloc>::rbegin()
{
    return reverse_iterator(end_);
}

template<class T, class Alloc>
const typename Vector<T, Alloc>::reverse_iterator Vector<T, Alloc>::rbegin() const
{
    return reverse_iterator(end_);
}

template<class T, class Alloc>
typename Vector<T, Alloc>::reverse_iterator Vector<T, Alloc>::rend()
{
    return reverse_iterator(begin_);
}

template<class T, class Alloc>
const typename Vector<T, Alloc>::reverse_iterator Vector<T, Alloc>::rend() const
{
    return reverse_iterator(begin_);
}

template<class T, class Alloc>
void Vector<T, Alloc>::resize(std::size_t new_size, const T& default_value)
{
    if(mem_size_ < new_size) {
        reserve(new_size);
    }
    auto prev_end = end_;
    end_ = begin_ + new_size;
    while (prev_end > end_)
    {
        --prev_end;
        alloc_.destroy(prev_end);
    }
    while (prev_end < end_)
    {
        alloc_.construct(prev_end, default_value);
        ++prev_end;
    }
}

template<class T, class Alloc>
void Vector<T, Alloc>::reserve(std::size_t new_mem_size)
{
    if (new_mem_size > mem_size_)
    {
        std::size_t size_ = size();
        T* new_memory = alloc_.allocate(new_mem_size);
        for (std::size_t i = 0; i < size_; ++i)
        {
            alloc_.construct(new_memory + i, std::move(data_[i]));
            alloc_.destroy(data_ + i);
        }
        alloc_.deallocate(data_, mem_size_);
        mem_size_ = new_mem_size;
        data_ = new_memory;
        begin_ = iterator(data_);
        end_ = begin_ + size_;
    }
}