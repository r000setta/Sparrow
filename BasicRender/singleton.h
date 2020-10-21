#pragma once

#include <memory>


namespace sparrow {
	template<typename T>
	class Singleton {
	private:
		Singleton(const Singleton<T>&) = delete;
		Singleton& operator=(const Singleton<T>&) = delete;
	protected:
		static std::shared_ptr<T> _instance;
	public:
		Singleton() = default;
		~Singleton() = default;
		static std::shared_ptr<T> getSingleton() {
			if (_instance == nullptr)
				_instance = std::make_shared<T>();
			return _instance;
		}
	};
}