#pragma once
#include <vector>
#include <stack>
#include <memory>
#include <cstddef>
#include <stdexcept>

template<typename T>
class ObjectPool;

template<typename T>
class PoolHandle
{
	friend class ObjectPool<T>;
	explicit PoolHandle(T* obj, ObjectPool<T>* pool)
		: obj_(obj), pool_(pool) {
	}
public:
	// コピー不可
	PoolHandle(const PoolHandle&) = delete;
	PoolHandle& operator=(const PoolHandle&) = delete;

	// ムーブのみ許可
	PoolHandle(PoolHandle&& other) noexcept
		: obj_(other.obj_), pool_(other.pool_) {
		other.obj_ = nullptr;
		other.pool_ = nullptr;
	}
	PoolHandle& operator=(PoolHandle&&) = delete;

	~PoolHandle() {
		if (obj_ != nullptr && pool_ != nullptr) {
			pool_->Release(obj_);
		}
	}

	// オブジェクトへのアクセス
	T* operator->() { return obj_; }
	const T* operator->() const { return obj_; }
	T& operator*() { return *obj_; }
	const T& operator*() const { return *obj_; }

private:
	T* obj_ = nullptr;
	ObjectPool<T>* pool_ = nullptr;
};

template <typename T>
class ObjectPool
{
	friend class PoolHandle<T>;
public:
	explicit ObjectPool(std::size_t capacity)
	{
		objects_.reserve(capacity);
		for (std::size_t i = 0; i < capacity; ++i)
		{
			objects_.emplace_back(std::make_unique<T>());
			free_.push(objects_.back().get());
		}
	}

	PoolHandle<T> Acquire()
	{
		if (free_.empty())
			throw std::runtime_error("ObjectPool is empty");
		T* obj = free_.top();
		free_.pop();
		return PoolHandle<T>(obj, this);
	}

private:
	void Release(T* obj) { free_.push(obj); }

	std::vector<std::unique_ptr<T>> objects_;
	std::stack<T*> free_;
};