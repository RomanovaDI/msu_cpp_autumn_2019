#pragma once

/*template <class T>
class Allocator
{
public:
    //using value_type = T;
    //using pointer = T*;
    //using size_type = size_t;

    T* allocate(size_t size){
        return (T*) malloc(sizeof(T) * size);
    }

    void construct(T* pointer, size_t size){
        for (size_t i = 0; i < size; ++i) {
            new (pointer + i) T();
        }
    }

    void construct(T* pointer, T value){
        new (pointer) T*(value);
    }

    void deallocate(T* pointer, size_t size){
        free(pointer); 
    }

    void destructor(T* pointer, size_t size){
        for(size_t i = 0; i < size; ++i){
            pointer[i].~T();
        }
    }

    //size_t max_size(){
    //    return std::numeric_limits<size_t>::max();
    //}
};*/

template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_type count){
        return static_cast<pointer>(operator new(count * sizeof(value_type)));
    }

    void deallocate(pointer ptr){
        operator delete(ptr);
    }

    size_type max_size() const noexcept{
        return std::numeric_limits<size_type>::max();
    }

    template<class... Args>
    void constructor(pointer p, Args&&... args){
        new(p) T(std::forward<Args>(args)...);
    }

    void destructor(pointer p){
        p->~value_type();
    }
};

template <class T>
class Iterator
{
    T* elem;
    bool rev;
public:

    Iterator(T* ptr, bool reverse=true){
        elem = ptr;
        rev = reverse;
    }

    T& operator* () const{
        return *elem;
    }

    Iterator& operator ++(){
        if (rev) {
            elem++;
        } else{
            elem--;
        }
        return *this;
    }

    Iterator& operator --(){
        if (rev) {
            elem--;
        } else{
            elem++;
        }
        return *this;
    }

    Iterator& operator +(size_t count){
        if (rev) {
            elem += count;
        } else{
            elem -= count;
        }
        return *this;
    }

    Iterator& operator -(size_t count){
        if (rev) {
            elem -= count;
        } else{
            elem += count;
        }
        return *this;
    }

    bool operator == (const Iterator<T>& other) const{
        return elem == other.elem;
    }

    bool operator !=(const Iterator<T>& other) const{
        return !(*this == other);
    }
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using iterator = Iterator<T>;
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using pointer = T*;
    using reverse_iterator = std::reverse_iterator<iterator>;

private:
    Alloc alloc_;
    pointer data_;
    size_type size_;
    size_type capacity_;
public:
    Vector() : data_(nullptr), size_(0), capacity_(0){};
    explicit Vector(size_type count): data_(alloc_.allocate(2 * count)), size_(count), capacity_(2 * count){
        for (size_type i; i < size_; i++){
            alloc_.construct(data_ + i);
        }
    };

    bool empty() const{
        return (size_ == 0);
    }

    size_type size() const{
        return size_;
    }

    iterator begin() const {
        return iterator(data_);
    }

    iterator end() const {
        return iterator(data_ + size_);
    }

    reverse_iterator rbegin() const {
        return reverse_iterator(this-> end());
    }

    reverse_iterator rend() const {
        return reverse_iterator(this-> begin());
    }

    reference operator[](size_type n){
        if (n >= size_){
            throw std::out_of_range("Out of range");
        }
        return data_[n];
    }

    void reserve(size_type size){
        if (size < capacity_){
            return;
        }
        pointer new_data = alloc_.allocate(size);
        for (size_type i = 0; i < size_; i++){
            alloc_.construct(new_data + i, std::forward<value_type>(*(data_ + i)));
            alloc_.destroy(data_ + i);
        }
        alloc_.deallocate(data_);
        data_ = new_data;
        capacity_ = size;

    }

    void resize(size_type count){
        if (size_ > count){
            for (size_type i = count; i < size_; i++){
                alloc_.destroy(data_ + i);
            }
            size_ = count;
        }
        else{
            reserve(count);
            for (size_type i = size_; i < count; i++){
                alloc_.construct(data_ + i);
            }
            size_ = count;
        }
    }

    void pop_back(){
        if (size_ > 0){
            alloc_.destroy(data_ + size_);
            size_--;
        }
    }

    void push_back(const reference value){
        if (size_ == capacity_){
            reserve(capacity_ * 2 + 1);
        }

        alloc_.construct(data_ + size_, value);
        size_++;
    }

    void push_back(value_type&& value){
        if (size_ == capacity_){
            reserve(capacity_ * 2 + 1);
        }
        alloc_.construct(data_ + size_, std::forward<value_type>(value));
        size_++;
    }

    void clear(){
        resize(0);
    }
};