#include <iostream>
using namespace std;

template <typename T>
class UniquePointer
{
private:
    T* ptr;

public:
    explicit UniquePointer(T* p = nullptr) : ptr(p) {}

    ~UniquePointer()
    {
        delete ptr;
    }

    UniquePointer(const UniquePointer&) = delete;
    UniquePointer& operator=(const UniquePointer&) = delete;

    UniquePointer(UniquePointer&& other) noexcept : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    UniquePointer& operator=(UniquePointer&& other) noexcept
    {
        if (this != &other)
        {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const
    {
        return *ptr;
    }

    T* operator->() const
    {
        return ptr;
    }

    T* get() const
    {
        return ptr;
    }

    void reset(T* p = nullptr)
    {
        delete ptr;
        ptr = p;
    }

    T* release()
    {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }
};

template <typename T>
class SharedPointer
{
private:
    T* ptr;
    size_t* ref_count;

    void release()
    {
        if (ref_count)
        {
            if (--(*ref_count) == 0)
            {
                delete ptr;
                delete ref_count;
            }
        }
    }

public:
    explicit SharedPointer(T* p = nullptr)
        : ptr(p), ref_count(p ? new size_t(1) : nullptr) {}

    ~SharedPointer()
    {
        release();
    }

    SharedPointer(const SharedPointer& other)
        : ptr(other.ptr), ref_count(other.ref_count)
    {
        if (ref_count)
        {
            ++(*ref_count);
        }
    }

    SharedPointer& operator=(const SharedPointer& other)
    {
        if (this != &other)
        {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            if (ref_count)
            {
                ++(*ref_count);
            }
        }
        return *this;
    }

    SharedPointer(SharedPointer&& other) noexcept
        : ptr(other.ptr), ref_count(other.ref_count)
    {
        other.ptr = nullptr;
        other.ref_count = nullptr;
    }

    SharedPointer& operator=(SharedPointer&& other) noexcept
    {
        if (this != &other)
        {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            other.ptr = nullptr;
            other.ref_count = nullptr;
        }
        return *this;
    }

    T& operator*() const
    {
        return *ptr;
    }

    T* operator->() const
    {
        return ptr;
    }

    T* get() const
    {
        return ptr;
    }

    size_t use_count() const
    {
        return ref_count ? *ref_count : 0;
    }
};

/*
int main()
{
    cout << "Testing UniquePointer:" << endl;
    {
        UniquePointer<int> uptr1(new int(10));
        cout << "Value: " << *uptr1 << endl;

        UniquePointer<int> uptr2 = move(uptr1);
        if (!uptr1.get()) {
            cout << "uptr1 is now null." << endl;
        }
        cout << "Value of uptr2: " << *uptr2 << endl;

        uptr2.reset(new int(20));
        cout << "New value of uptr2: " << *uptr2 << endl;
    }

    cout << "\nTesting SharedPointer:" << endl;
    {
        SharedPointer<int> sptr1(new int(100));
        cout << "Value: " << *sptr1 << ", References: " << sptr1.use_count() << endl;

        {
            SharedPointer<int> sptr2 = sptr1;
            cout << "After copying:" << endl;
            cout << "References sptr1: " << sptr1.use_count() << ", References sptr2: " << sptr2.use_count() << endl;
        }

        cout << "After destroying sptr2: " << sptr1.use_count() << endl;

        SharedPointer<int> sptr3 = move(sptr1);
        cout << "After moving sptr1 to sptr3:" << endl;
        cout << "References sptr3: " << sptr3.use_count() << endl;
    }

    return 0;
}
*/