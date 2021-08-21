#include <iostream>

namespace code
{
    template <typename T>
    class shared_ptr
    {
    private:
        struct control_block
        {
            int count{};
        };
        void decrement_ref_count_and_delete_if_needed()
        {
            if (ref_count_->count != 0)
            {
                --(ref_count_->count);
                if (ref_count_->count == 0)
                {
                    delete _ptr;
                    delete ref_count_;
                    _ptr = nullptr;
                    ref_count_ = new control_block();
                }
            }
        }
        T *_ptr;
        control_block *ref_count_;

    public:
        constexpr shared_ptr<T>() noexcept : _ptr{nullptr}, ref_count_{new control_block()} {}
        constexpr shared_ptr<T>(nullptr_t) noexcept : shared_ptr() {}
        shared_ptr(const shared_ptr<T> &other) noexcept;
        shared_ptr(shared_ptr<T> &&other) noexcept;
        explicit shared_ptr(T *ptr);
        ~shared_ptr() noexcept
        {
            decrement_ref_count_and_delete_if_needed();
        }
        T *get() const noexcept;
        long use_count() const;
        explicit operator bool() const noexcept;
        T &operator*() const noexcept;
        T *operator->() const noexcept;
        shared_ptr<T> &operator=(const shared_ptr<T> &other) noexcept;
        shared_ptr<T> &operator=(shared_ptr<T> &&other) noexcept;
        void reset() noexcept;
        void reset(T *ptr);
        void swap(shared_ptr &other);
        bool unique() const;
        
        // template <typename Deleter>
        // shared_ptr(T *ptr, Deleter deleter) {}
        // template <typename Delerter>
        // shared_ptr(nullptr_t ptr, Delerter deleter);
        // template <typename T>
        // explicit shared_ptr(const weak_ptr<T> &wp);
        // template <typename T, typename Deleter>
        // shared_ptr &operator=(unique_ptr<T, Deleter> &&other);
    };
};

template <typename T>
code::shared_ptr<T>::shared_ptr(T *ptr)
{
    _ptr = ptr;
    ref_count_ = new control_block{1};
}

template <typename T>
code::shared_ptr<T>::shared_ptr(const shared_ptr<T> &other) noexcept
{
    _ptr = other._ptr;
    ref_count_ = other.ref_count_;
    if (ref_count_->count)
    {
        ++(ref_count_->count);
    }
}

template <typename T>
code::shared_ptr<T>::shared_ptr(shared_ptr<T> &&other) noexcept
{
    if (other.ref_count_->count != 0)
    {
        ref_count_ = other.ref_count_;
        _ptr = other._ptr;
        other._ptr = nullptr;
        other.ref_count_ = new control_block();
    }
}

template <typename T>
T *code::shared_ptr<T>::get() const noexcept
{
    return this->_ptr;
}

template <typename T>
long code::shared_ptr<T>::use_count() const
{
    return this->ref_count_->count;
}

template <typename T>
code::shared_ptr<T>::operator bool() const noexcept
{
    if (get())
    {
        return true;
    }
    return false;
}

template <typename T>
T &code::shared_ptr<T>::operator*() const noexcept
{
    return *_ptr;
}

template <typename T>
T *code::shared_ptr<T>::operator->() const noexcept
{
    return _ptr;
}

template <typename T>
code::shared_ptr<T> &code::shared_ptr<T>::operator=(const shared_ptr<T> &other) noexcept
{
    if (this != &other)
    {
        decrement_ref_count_and_delete_if_needed();
        _ptr = other._ptr;
        ref_count_ = other.ref_count_;
        if (ref_count_->count)
        {
            ref_count_->count++;
        }
    }
    return *this;
}

template <typename T>
code::shared_ptr<T> &code::shared_ptr<T>::operator=(shared_ptr<T> &&other) noexcept
{
    this->ref_count_ = other.ref_count_;
    this->_ptr = other._ptr;
    other._ptr = nullptr;
    other.ref_count_ = new control_block();
    return *this;
}

template <typename T>
void code::shared_ptr<T>::reset() noexcept
{
    decrement_ref_count_and_delete_if_needed();
    this->_ptr = nullptr;
    this->ref_count_ = new control_block();
}

template <typename T>
void code::shared_ptr<T>::reset(T *ptr)
{
    decrement_ref_count_and_delete_if_needed();
    this->_ptr = ptr;
    this->ref_count_->count++;
}

template <typename T>
void code::shared_ptr<T>::swap(shared_ptr<T> &other)
{
    shared_ptr<T> tmp;
    tmp._ptr = this->_ptr;
    tmp.ref_count_ = this->ref_count_;
    this->ref_count_ = other.ref_count_;
    this->_ptr = other._ptr;
    other._ptr = tmp._ptr;
    other.ref_count_ = tmp.ref_count_;
}

template <typename T>
bool code::shared_ptr<T>::unique() const
{
    if(this->ref_count_->count == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}