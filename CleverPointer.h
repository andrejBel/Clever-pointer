#pragma once
using namespace std;
template<typename T>
class CleverPointer
{
private:

	class ReferenceCount
	{
	public:
		ReferenceCount() : referenceCount_(1) {};

		inline int getReferenceCount()
		{
			return referenceCount_;
		}

		inline void addReferenceCount()
		{
			++referenceCount_;
		}

		inline int decreaseReferenceCount()
		{
			return --referenceCount_;
		}

	private:
		int referenceCount_;

	};

	void reset();

	T* pointer_;
	ReferenceCount* referenceCount_;

public:

	CleverPointer(): pointer_(nullptr), referenceCount_(new ReferenceCount())
	{}

	CleverPointer(const T* pointer): pointer_(const_cast<T *> (pointer) ), referenceCount_(new ReferenceCount())
	{
	}

	CleverPointer(const T& object) : pointer_(new T(object)), referenceCount_(new ReferenceCount())
	{}

	CleverPointer(const CleverPointer& other) : pointer_(other.pointer_), referenceCount_(other.referenceCount_)
	{
		referenceCount_->addReferenceCount();
	}

	CleverPointer<T>& operator=(const CleverPointer& other);

	CleverPointer<T>& operator=(const T* otherPointer);

	CleverPointer<T>& operator=(const T& otherObject);

	bool operator==(const CleverPointer& other);

	T& operator*() 
	{
		if (!pointer_)
			throw std::logic_error("Dereferencing null pointer!");
		return *pointer_;
	}

	T* operator->() 
	{
		if (!pointer_)
			throw std::logic_error("Dereferencing null pointer!");
		return pointer_;
	}

	~CleverPointer()
	{
		if (referenceCount_->decreaseReferenceCount() == 0)
		{
			reset();
		}
	}

};

template<typename T>
inline void CleverPointer<T>::reset()
{
	delete referenceCount_;
	referenceCount_ = 0;
	delete pointer_;
	pointer_ = 0;
}

template<typename T>
inline CleverPointer<T>& CleverPointer<T>::operator=(const CleverPointer & other)
{
	if (this != &other)
	{
		if (referenceCount_->decreaseReferenceCount() == 0)
		{
			reset();
		}
		pointer_ = other.pointer_;
		referenceCount_ = other.referenceCount_;
		referenceCount_->addReferenceCount();
	}
	return *this;
}

template<typename T>
inline CleverPointer<T>& CleverPointer<T>::operator=(const T * otherPointer)
{
	if (referenceCount_->decreaseReferenceCount() == 0)
	{
		reset();
	}
	pointer_ = const_cast<T *>( otherPointer );
	referenceCount_ = new ReferenceCount();
	return *this;
}

template<typename T>
inline CleverPointer<T>& CleverPointer<T>::operator=(const T & otherObject)
{
	if (referenceCount_->decreaseReferenceCount() == 0)
	{
		reset();
	}
	pointer_ = new T(otherObject);
	referenceCount_ = new ReferenceCount();
	return *this;
}

template<typename T>
inline bool CleverPointer<T>::operator==(const CleverPointer & other)
{
	return *pointer_ == *other.pointer_;
}

template<typename T, typename... Parameters>
inline CleverPointer<T> makeClever(Parameters&&... param) 
{
	return CleverPointer<T>(new T(std::forward<Parameters>(param)...) );
}
